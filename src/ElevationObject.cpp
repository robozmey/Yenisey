//
// Created by vladimir on 28.12.22.
//

#include <array>
#include "ElevationObject.h"
#include "tools.h"

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
    position = in_position;
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = in_normal;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform sampler2D elevation_texture;

uniform vec3 light_direction;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;


void main()
{
    vec3 albedo = vec3(0.5);
    vec3 water_albedo = vec3(0.0, 0.0, 0.5);
    if (position.y < -370)
        albedo = water_albedo;

    vec3 ambient = vec3(0.2);
    float diffuse = max(0.0, dot(normalize(normal), light_direction));

    vec3 color = albedo * (ambient + diffuse);

    out_color = vec4(color, 1);
}
)";

glm::vec3 get_triangle_normal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::cross(b-a, c-a);
}


namespace yny {
    void ElevationObject::recalc_terrain(Player& scene_player) {
        int latitude_seconds = scene_player.latitude_minute * 60;
        int longitude_seconds = scene_player.longitude_minute * 60;
        float center_latitude_grad = scene_player.latitude + latitude_seconds / 3600.f;
        float center_longitude_grad = scene_player.longitude + longitude_seconds / 3600.f;

        int diam_seconds = 60;

        int diam_size = diam_seconds * 2 + 1;

        latitude_minute = scene_player.latitude_minute;
        longitude_minute = scene_player.longitude_minute;

        float latitude_factor = wgs_distance(center_latitude_grad,
                                             center_longitude_grad,
                                             center_latitude_grad + 1 / 3600.f,
                                             center_longitude_grad) * 1000;
        float longitude_factor = wgs_distance(center_latitude_grad,
                                              center_longitude_grad,
                                              center_latitude_grad,
                                              center_longitude_grad + 1 / 3600.f) * 1000;

        vertices.resize(diam_size * diam_size * lod_count);
        indices.resize(0);
        for (int lod = 0; lod < lod_count; lod++) {
            int lod_verteces_offset = diam_size * diam_size * lod;
            int diam_step = (1 << lod);
            for (int i = 0; i < diam_size; i++) {
                for (int j = 0; j < diam_size; j++) {
                    int latitude_second = (i - diam_seconds) + latitude_seconds / diam_step;
                    int longitude_second = (j - diam_seconds) + longitude_seconds / diam_step;

                    float x = (i - diam_seconds) * diam_step * latitude_factor;
                    float y = float(elevation_map_lod[lod][latitude_second][longitude_second]) - 600; //(lod_count-1 - lod) * 100
                    float z = (j - diam_seconds) * diam_step * longitude_factor;
                    vertex v;
                    v.position = {x, y, z}; //
                    vertices[lod_verteces_offset + i * diam_size + j] = v;
                }
            }
            for (int i = 1; i < diam_size; i++) {
                for (int j = 1; j < diam_size; j++) {
                    if (lod == 0 || abs(i-0.5 - diam_seconds) * 2 > diam_seconds+1
                                 || abs(j-0.5 - diam_seconds) * 2 > diam_seconds+1) {
                        indices.push_back(lod_verteces_offset + i * diam_size + j);
                        indices.push_back(lod_verteces_offset + (i) * diam_size + j - 1);
                        indices.push_back(lod_verteces_offset + (i - 1) * diam_size + j);

                        indices.push_back(lod_verteces_offset + (i - 1) * diam_size + j - 1);
                        indices.push_back(lod_verteces_offset + (i - 1) * diam_size + j);
                        indices.push_back(lod_verteces_offset + (i) * diam_size + j - 1);
                    }
                    if (lod > 0 && abs(i-0.5 - diam_seconds) * 2 == diam_seconds+1
                                && abs(j-0.5 - diam_seconds) * 2 <= diam_seconds+1) {

                    }
                }
            }
            int pr_lod_verteces_offset = diam_size * diam_size * (lod - 1);
            if (lod > 0) {
                for (int t = 0; t < 2; t++) {
                    int i = 3 * diam_seconds / 2 + 1;
                    int pr_i = diam_size - 1;
                    if (t == 1) {
                        i = diam_seconds / 2 - 1;
                        pr_i = 0;
                    }
                    for(int t2 = 0; t2 < 2; t2++) {
                        int j = diam_seconds / 2;
                        if (t2 == 1) {
                            j = 3 * diam_seconds / 2;
                        }
                        indices.push_back(lod_verteces_offset + i * diam_size + j + 1 - (1 - t2));
                        indices.push_back(lod_verteces_offset + i * diam_size + j - (1 - t2));
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2);
                        if (t == 1) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }

//                        indices.push_back(lod_verteces_offset + (i - (1 - t2)) * diam_size + j);
//                        indices.push_back(lod_verteces_offset + (i+1 - (1 - t2)) * diam_size + j);
//                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2);
//                        if ((t2 == 0)) {
//                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
//                        }
                    }
//                    continue;

                    for (int j = diam_seconds / 2 + 1; j < diam_size - diam_seconds / 2; j++) {
                        indices.push_back(lod_verteces_offset + i * diam_size + j);
                        indices.push_back(lod_verteces_offset + i * diam_size + j - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2 - 1);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }

                        indices.push_back(lod_verteces_offset + i * diam_size + j - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2 - 2);
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2 - 1);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }

                        indices.push_back(lod_verteces_offset + i * diam_size + j);
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2 - 1);
                        indices.push_back(pr_lod_verteces_offset + pr_i * diam_size + (j - diam_seconds / 2) * 2);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }


                        indices.push_back(lod_verteces_offset + (j - 1) * diam_size + i);
                        indices.push_back(lod_verteces_offset + j * diam_size + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2 - 1) * diam_size + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }

                        indices.push_back(lod_verteces_offset + (j - 1) * diam_size + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2 - 1) * diam_size + pr_i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2 - 2) * diam_size + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }

                        indices.push_back(lod_verteces_offset + (j) * diam_size + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2) * diam_size + pr_i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2 - 1) * diam_size + pr_i);
                        if (t == 1) {
                            std::swap(indices[indices.size()-2], indices[indices.size()-1]);
                        }
                    }
                }
            }
            for (int i = 0; i < diam_size; i++) {
                for (int j = 0; j < diam_size; j++) {
                    glm::vec3 normal_sum = {0, 0, 0};
                    float count = 0;
                    for (int i1 = -1; i1 <= 1; i1 += 2) {
                        for (int j1 = -1; j1 <= 1; j1 += 2) {
                            if (0 <= i + i1 && i + i1 < diam_size && 0 <= j + j1 && j + j1 < diam_size) {
                                glm::vec3 tmp_normal = get_triangle_normal(
                                        vertices[lod_verteces_offset + i * diam_size + j].position,
                                        vertices[lod_verteces_offset + (i + i1) * diam_size + j].position,
                                        vertices[lod_verteces_offset + i * diam_size + j + j1].position);
                                if (tmp_normal.y < 0)
                                    tmp_normal *= -1.f;
                                normal_sum += tmp_normal;
                                count++;
                            }
                        }
                    }
                    vertices[lod_verteces_offset + i * diam_size + j].normal = glm::normalize(normal_sum / count);
                }
            }
        }
    }

    void ElevationObject::render(Player& scene_player)  {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, elevation_texture);

        if (latitude_minute != scene_player.latitude_minute || longitude_minute != scene_player.longitude_minute) {
            recalc_terrain(scene_player);
        }
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, normal));


        ///

        glUseProgram(program);
        glUniform1i(texture_location, 0);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&scene_player.projection));

        auto l = glm::vec3(0, 1, 0);
        glUniform3fv(light_direction_location, 1, reinterpret_cast<float *>(&l));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    ElevationObject::ElevationObject() {
        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        texture_location = glGetUniformLocation(program, "elevation_texture");
        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");

        light_direction_location = glGetUniformLocation(program, "light_direction");

        elevation_map_lod.resize(lod_count);

        upload_elevation_map(55, 92, elevation_map_lod[0]);

        for (int lod = 1; lod < lod_count; lod++) {
            int pr_height = elevation_map_lod[lod-1].size();
            int pr_width = elevation_map_lod[lod-1][0].size();
            int height = (pr_height + 1) / 2;
            int width = (pr_width + 1) / 2;

            elevation_map_lod[lod].assign(height, std::vector<uint16_t>(width));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    int val = 0;
                    int count = 0;
                    for (int i1 = 0; i1 < 2; i1++) {
                        for (int j1 = 0; j1 < 2; j1++) {
                            if (i*2 + i1 < pr_height && j*2 + j1 < pr_width) {
                                val += elevation_map_lod[lod-1][i*2 + i1][j*2 + j1];
                                count++;
                            }
                        }
                    }
                    elevation_map_lod[lod][i][j] = val / count;
                }
            }
        }

//        elevation_texture = load_elevation_texture(elevation_map_lod[4]);

        vao = 1;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
    }
} // yny