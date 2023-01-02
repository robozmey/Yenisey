//
// Created by vladimir on 02.01.23.
//

#include "elevation/ElevationRenderComponent.h"
#include "tools.h"
#include "Object.h"
#include "component/MeshComponent.h"

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    tangent = mat3(model) * in_tangent;
    normal = mat3(model) * in_normal;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform sampler2D grass_texture;
uniform sampler2D grass_normal_texture;

uniform vec3 light_direction;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;


void main()
{
    vec2 real_texcoord = position.xz / 100;
    vec3 albedo = texture(grass_texture, real_texcoord).rgb;

//    vec3 bitangent = cross(tangent, normal);
//    mat3 tbn = mat3(tangent, bitangent, normal);
//    vec3 real_normal = tbn * (texture(grass_normal_texture, real_texcoord).xyz * 2.0 - vec3(1.0));

    vec3 water_albedo = vec3(0.0, 0.0, 0.5);
    if (position.y < -370)
        discard;

    vec3 ambient = vec3(0.2);
    float diffuse = max(0.0, dot(normalize(normal), light_direction));

    vec3 color = albedo * (ambient + diffuse);

    out_color = vec4(color, 1);
}
)";

namespace yny {

    void ElevationRenderComponent::render(Player& scene_player)  {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grass_normal_texture);

        MeshComponent* mc = static_cast<MeshComponent *>(parentObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;

        if (latitude_minute != scene_player.latitude_minute || longitude_minute != scene_player.longitude_minute) {

            latitude_minute = scene_player.latitude_minute;
            longitude_minute = scene_player.longitude_minute;

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
        }

        ///

        glUseProgram(program);
        glUniform1i(grass_texture_location, 0);
        glUniform1i(grass_normal_texture_location, 1);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.projection));

        auto l = glm::vec3(0, 1, 0);
        glUniform3fv(light_direction_location, 1, reinterpret_cast<float *>(&l));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    ElevationRenderComponent::ElevationRenderComponent() {

        elevationDataObject = new ElevationDataObject();
        lod_count = elevationDataObject->lod_count;

        std::string project_root = PROJECT_ROOT;
        grass_texture = load_texture(project_root + "/texture/grass.png");
        grass_normal_texture = load_texture(project_root + "/texture/grass_normal.png");

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        grass_texture_location = glGetUniformLocation(program, "grass_texture");
        grass_normal_texture_location = glGetUniformLocation(program, "grass_normal_texture");
        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");

        light_direction_location = glGetUniformLocation(program, "light_direction");

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
} // yny