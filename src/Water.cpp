//
// Created by vladimir on 30.12.22.
//

#include "Water.h"
#include "tools.h"

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

float cross2(glm::vec2 a, glm::vec2 b) {
    return a.x * b.x + a.y * b.y;
}

namespace yny {
    float Water::h(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.amplitude * sin(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    float Water::dhx(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.wavelength * wf.direction.x * wf.amplitude * cos(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    float Water::dhz(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.wavelength * wf.direction.y * wf.amplitude * cos(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    void Water::update_vertices(Player& scene_player) {
        int k = 50;
        int r = 20;
        int n = r * 2 + 1;
        vertices.resize(lod_count * n * n);
        indices.clear();
        for (int lod = 0; lod < lod_count; lod++) {
            int lod_verteces_offset = n * n * lod;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    float time = scene_player.time;

                    float x = (i - r) * k * (1<<lod);
                    float z = (j - r) * k * (1<<lod);
                    float y = h(x, z, time) - 370;
                    vertex &v = vertices[lod_verteces_offset + i * n + j];
                    v.position = {x, y, z}; //

                    glm::vec3 b = {1, dhx(x, z, time), 0};
                    glm::vec3 t = {0, dhz(x, z, time), 1};

                    v.normal = glm::cross(b, t);
                    v.tangent = t;
                }
            }

            for (int i = 1; i < n; i++) {
                for (int j = 1; j < n; j++) {
                    if (lod == 0 || abs(i - r - 0.5) * 2 > r + 1 || abs(j - r - 0.5) * 2 > r + 1) {
                        indices.push_back(lod_verteces_offset + i * n + j);
                        indices.push_back(lod_verteces_offset + (i) * n + j - 1);
                        indices.push_back(lod_verteces_offset + (i - 1) * n + j);

                        indices.push_back(lod_verteces_offset + (i - 1) * n + j - 1);
                        indices.push_back(lod_verteces_offset + (i - 1) * n + j);
                        indices.push_back(lod_verteces_offset + (i) * n + j - 1);
                    }
                }
            }
            int pr_lod_verteces_offset = n * n * (lod - 1);
            if (lod > 0) {
                for (int t = 0; t < 2; t++) {
                    int i = 3 * r / 2 + 1;
                    int pr_i = n - 1;
                    if (t == 1) {
                        i = r / 2 - 1;
                        pr_i = 0;
                    }
                    for (int t2 = 0; t2 < 2; t2++) {
                        int j = r / 2;
                        if (t2 == 1) {
                            j = 3 * r / 2;
                        }
                        indices.push_back(lod_verteces_offset + i * n + j + 1 - (1 - t2));
                        indices.push_back(lod_verteces_offset + i * n + j - (1 - t2));
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

                        indices.push_back(lod_verteces_offset + (j + 1 - (1 - t2)) * n + i);
                        indices.push_back(lod_verteces_offset + (j - (1 - t2)) * n + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2) * n + pr_i);
                        if (t == 0) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }
                    }

                    for (int j = r / 2 + 1; j < n - r / 2; j++) {
                        indices.push_back(lod_verteces_offset + i * n + j);
                        indices.push_back(lod_verteces_offset + i * n + j - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2 - 1);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

                        indices.push_back(lod_verteces_offset + i * n + j - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2 - 2);
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2 - 1);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

                        indices.push_back(lod_verteces_offset + i * n + j);
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2 - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * n + (j - r / 2) * 2);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }


                        indices.push_back(lod_verteces_offset + (j - 1) * n + i);
                        indices.push_back(lod_verteces_offset + j * n + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2 - 1) * n + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

                        indices.push_back(lod_verteces_offset + (j - 1) * n + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2 - 1) * n + pr_i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2 - 2) * n + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

                        indices.push_back(lod_verteces_offset + (j) * n + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2) * n + pr_i);
                        indices.push_back(pr_lod_verteces_offset + ((j - r / 2) * 2 - 1) * n + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }
                    }
                }
            }
        }

        int offset_n = 0;
        offsets.clear();
        for (int i = -offset_n; i <= offset_n; i++) {
            for (int j = -offset_n; j <= offset_n; j++) {
                offsets.emplace_back(i * (n - 1) * k, 0, j * (n - 1) * k);
            }
        }
    }

    void Water::render(Player& scene_player) {

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

    Water::Water() {
        wave_funcs.push_back({1, {1, 0}, 1, 1});
        wave_funcs.push_back({1, {0.5, 1}, 1, 1});

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