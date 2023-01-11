//
// Created by vladimir on 07.01.23.
//

#include "GLTF/GLTFMeshComponent.h"

#include "stb_image.h"

namespace yny {
    void GLTFMeshComponent::update_vertices(Camera *scene_player) {
        MeshComponent::update_vertices(scene_player);
    }

    GLTFMeshComponent::GLTFMeshComponent(std::string model_path) : MeshComponent() {

        input_model = load_gltf(model_path);

        has_vbo = true;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, input_model.buffer.size(), input_model.buffer.data(), GL_STATIC_DRAW);

        auto setup_attribute = [](int index, gltf_model::accessor const & accessor, bool integer = false)
        {
            glEnableVertexAttribArray(index);
            if (integer)
                glVertexAttribIPointer(index, accessor.size, accessor.type, 0, reinterpret_cast<void *>(accessor.view.offset));
            else
                glVertexAttribPointer(index, accessor.size, accessor.type, GL_FALSE, 0, reinterpret_cast<void *>(accessor.view.offset));
        };

        for (auto const & mesh : input_model.meshes)
        {
            auto & result = meshes.emplace_back();
            glGenVertexArrays(1, &result.vao);
            glBindVertexArray(result.vao);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
            result.indices = mesh.indices;

            setup_attribute(0, mesh.position);
            setup_attribute(1, mesh.normal);
            setup_attribute(2, mesh.texcoord);
            setup_attribute(3, mesh.joints, true);
            setup_attribute(4, mesh.weights);

            result.material = mesh.material;
        }

        for (auto const & mesh : meshes)
        {
            if (!mesh.material.texture_path) continue;
            if (textures.contains(*mesh.material.texture_path)) continue;

            auto path = std::filesystem::path(model_path).parent_path() / *mesh.material.texture_path;

            int width, height, channels;
            auto data = stbi_load(path.c_str(), &width, &height, &channels, 4);
            assert(data);

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);

            textures[*mesh.material.texture_path] = texture;
        }
    }
} // yny