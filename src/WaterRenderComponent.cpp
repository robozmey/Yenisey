//
// Created by vladimir on 02.01.23.
//

#include "WaterRenderComponent.h"
#include "tools.h"
#include "component/MeshComponent.h"
#include "WaterMeshComponent.h"

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_texcoord;
layout (location = 4) in vec3 in_instance_position;

out vec3 position;
out vec3 normal;
out vec3 tangent;
out vec2 texcoord;

void main()
{
    position = in_position + in_instance_position;
    gl_Position = projection * view * model * vec4(position, 1.0);
    tangent = mat3(model) * in_tangent;
    normal = mat3(model) * in_normal;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform vec3 light_direction;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;


void main()
{
    vec3 albedo = vec3(0.0, 0.0, 0.6);

    vec3 ambient = vec3(0.2);
    float diffuse = max(0.0, dot(normalize(normal), -light_direction));

    vec3 color = albedo * (ambient + diffuse);

    out_color = vec4(color, 1);
}
)";

namespace yny {

    void WaterRenderComponent::render(Player& scene_player) {

        WaterMeshComponent* mc = static_cast<WaterMeshComponent *>(parentObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;
        std::vector<glm::vec3>& offsets = mc->offsets;

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, offsets_vbo);
        glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(offsets[0]), offsets.data(), GL_DYNAMIC_DRAW);

        glUseProgram(program);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.projection));

        auto l = glm::vec3(0, 1, 0);
        glUniform3fv(light_direction_location, 1, reinterpret_cast<float *>(&l));

        glBindVertexArray(vao);
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr, offsets.size());
    }

    WaterRenderComponent::WaterRenderComponent() {

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");

        light_direction_location = glGetUniformLocation(program, "light_direction");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &offsets_vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, tangent));

        glBindBuffer(GL_ARRAY_BUFFER, offsets_vbo);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glVertexAttribDivisor(4, 1);
    }
} // yny