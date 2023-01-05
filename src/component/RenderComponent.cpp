//
// Created by vladimir on 02.01.23.
//

#include "Object.h"
#include "component/RenderComponent.h"
#include "tools.h"
#include "component/TransformComponent.h"
#include "component/MeshComponent.h"

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_texcoord;

out vec3 position;
out vec3 normal;
out vec3 tangent;
out vec2 texcoord;

void main()
{
    position = in_position;
    gl_Position = projection * view * model * transform * vec4(in_position, 1.0);
    tangent = mat3(model) * in_tangent;
    normal = mat3(model) * in_normal;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform int material_type;
uniform vec4 material_color;
uniform sampler2D material_texture;

uniform bool has_normal_map;
uniform sampler2D normal_texture;

uniform vec3 light_direction;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;


void main()
{
    vec2 real_texcoord = position.xz / 100;

    vec3 albedo = texture(material_texture, real_texcoord).rgb;

    vec3 real_normal = normal;

    if (has_normal_map) {
        vec3 bitangent = cross(tangent, normal);
        mat3 tbn = mat3(tangent, bitangent, normal);
        real_normal = tbn * (texture(normal_texture, real_texcoord).xyz * 2.0 - vec3(1.0));
    }

    vec3 ambient = vec3(0.2);
    float diffuse = max(0.0, dot(normalize(normal), light_direction));

    vec3 color = albedo * (ambient + diffuse);

    out_color = vec4(color, 1);
}
)";

namespace yny {

    Material default_material = Material();

    void RenderComponent::render(Player& scene_player) {

        MeshComponent* mc = static_cast<MeshComponent *>(componentsObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glUseProgram(program);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.projection));
        glm::mat4 transform;
        {
            TransformComponent* tc = reinterpret_cast<TransformComponent *>(componentsObject->components[Transform]);
            transform = tc->get_transform();
        }
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, reinterpret_cast<float *>(&transform));

        glUniform1i(material_type_location, material->materialType);
        if (material->materialType == OneColorMaterial) {
            glUniform4fv(material_type_location, 1, reinterpret_cast<float *>(&material->color));
        } else if (material->materialType == TextureMaterial) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material->texture);
            glUniform1i(material_texture_location, 0);
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    void RenderComponent::create_render_component() {

        type = Render;

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");
        transform_location = glGetUniformLocation(program, "transform");

        material_type_location = glGetUniformLocation(program, "material_type");
        material_color_location = glGetUniformLocation(program, "material_color");
        material_texture_location = glGetUniformLocation(program, "material_texture");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, tangent));
    }

    RenderComponent::RenderComponent() : material(&default_material) {
        create_render_component();
    }

    RenderComponent::RenderComponent(Material *material) : material(material) {
        create_render_component();
    }

} // yny