//
// Created by vladimir on 07.01.23.
//

#include "GLTFRenderComponent.h"

namespace yny {
    void GLTFRenderComponent::render(Camera* camera, LightSource* lightSource) {

        GLTFMeshComponent* mc = static_cast<GLTFMeshComponent *>(componentsObject->components[Mesh]);

        write_program_uniforms(camera, lightSource);

        auto draw_meshes = [&](bool transparent) {
            for (auto const &mesh: mc->meshes) {
                if (mesh.material.transparent != transparent)
                    continue;

//                if (mesh.material.two_sided)
//                    glDisable(GL_CULL_FACE);
//                else
//                    glEnable(GL_CULL_FACE);

//                if (transparent)
//                    glEnable(GL_BLEND);
//                else
//                    glDisable(GL_BLEND);

                if (mesh.material.texture_path) {
                    glBindTexture(GL_TEXTURE_2D, mc->textures[*mesh.material.texture_path]);
                    glUniform1i(material_type_location, 1);
                } else if (mesh.material.color) {
                    glUniform1i(material_type_location, 0);
                    glUniform4fv(material_color_location, 1, reinterpret_cast<const float *>(&(*mesh.material.color)));
                } else
                    continue;

                glBindVertexArray(mesh.vao);
                glDrawElements(GL_TRIANGLES, mesh.indices.count, mesh.indices.type,
                               reinterpret_cast<void *>(mesh.indices.view.offset));
            }
        };

        glActiveTexture(GL_TEXTURE0);
        draw_meshes(false);
//        glDepthMask(GL_FALSE);
//        draw_meshes(true);
//        glDepthMask(GL_TRUE);
    }

    void add_gltf_model(Object& object, std::string model_path) {
        object.add_component(yny::Mesh, new yny::GLTFMeshComponent(model_path));
        object.add_component(yny::Render, new yny::GLTFRenderComponent);
    }
} // yny