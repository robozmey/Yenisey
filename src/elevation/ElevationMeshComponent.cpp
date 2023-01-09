//
// Created by vladimir on 02.01.23.
//

#include "elevation/ElevationMeshComponent.h"
#include "elevation/elevation.h"
#include "tools.h"

namespace yny {
    void ElevationMeshComponent::recalc_terrain(Camera* scene_player) {
        int latitude_seconds = elevationDataObject->latitude_minute * 60;
        int longitude_seconds = elevationDataObject->longitude_minute * 60;
        float center_latitude_grad = elevationDataObject->latitude + latitude_seconds / 3600.f;
        float center_longitude_grad = elevationDataObject->longitude + longitude_seconds / 3600.f;

        int diam_seconds = 90;

        int diam_size = diam_seconds * 2 + 1;

        latitude_minute = elevationDataObject->latitude_minute;
        longitude_minute = elevationDataObject->longitude_minute;

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
                    int longitude_second = -(j - diam_seconds) + longitude_seconds / diam_step;

                    float x = (i - diam_seconds) * diam_step * latitude_factor;
                    float ed = elevationDataObject->get_data(elevationDataObject->latitude,
                                                             elevationDataObject->longitude,
                                                             lod,
                                                             latitude_second,
                                                             longitude_second);
                    float y = ed - 600; //(lod_count-1 - lod) * 100
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

                        indices.push_back(lod_verteces_offset + (j + 1 - (1 - t2)) * diam_size + i);
                        indices.push_back(lod_verteces_offset + (j - (1 - t2)) * diam_size + i);
                        indices.push_back(pr_lod_verteces_offset + ((j - diam_seconds / 2) * 2) * diam_size + pr_i);
                        if (t == 0) {
                            std::swap(indices[indices.size() - 2], indices[indices.size() - 1]);
                        }
                    }

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
                    glm::vec3 tangent_sum = {0, 0, 0};
                    float normal_count = 0, tangent_count = 0;
                    for (int i1 = -1; i1 <= 1; i1 += 2) {
                        for (int j1 = -1; j1 <= 1; j1 += 2) {
                            if (0 <= i + i1 && i + i1 < diam_size && 0 <= j + j1 && j + j1 < diam_size) {
                                glm::vec3 tmp_normal = get_triangle_normal(
                                        vertices[lod_verteces_offset + i * diam_size + j].position,
                                        vertices[lod_verteces_offset + (i + i1) * diam_size + j].position,
                                        vertices[lod_verteces_offset + i * diam_size + j + j1].position);
                                if (tmp_normal.y < 0)
                                    tmp_normal = get_triangle_normal(
                                            vertices[lod_verteces_offset + i * diam_size + j].position,
                                            vertices[lod_verteces_offset + i * diam_size + j + j1].position,
                                            vertices[lod_verteces_offset + (i + i1) * diam_size + j].position);
                                normal_sum += tmp_normal;
                                normal_count++;
                            }
                        }
                        if (0 <= i + i1 && i + i1 < diam_size) {
                            glm::vec3 tmp_tangent = {0, 0, 0};
                            tmp_tangent.x = vertices[lod_verteces_offset + i * diam_size + j].position.x
                                            - vertices[lod_verteces_offset + (i + i1) * diam_size + j].position.x;
                            if (tmp_tangent.x < 0)
                                tmp_tangent.x *= -1;
                            tangent_sum += tmp_tangent;
                            tangent_count++;
                        }
                    }
                    vertices[lod_verteces_offset + i * diam_size + j].normal = glm::normalize(normal_sum / normal_count);
                    vertices[lod_verteces_offset + i * diam_size + j].tangent = glm::normalize(tangent_sum / tangent_count);
                }
            }
        }
    }

    void ElevationMeshComponent::update_vertices(Camera* scene_player) {
        if (latitude_minute != elevationDataObject->latitude_minute || longitude_minute != elevationDataObject->longitude_minute) {
            recalc_terrain(scene_player);
        }
    }

    ElevationMeshComponent::ElevationMeshComponent() {
        elevationDataObject = new ElevationDataObject();
    }
} // yny