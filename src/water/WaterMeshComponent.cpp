//
// Created by vladimir on 02.01.23.
//

#include "water/WaterMeshComponent.h"
#include "base/Scene.h"

float cross2(glm::vec2 a, glm::vec2 b) {
    return a.x * b.x + a.y * b.y;
}

namespace yny {
    float WaterMeshComponent::h(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.amplitude * sin(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    float WaterMeshComponent::dhx(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.wavelength * wf.direction[0] * wf.amplitude * cos(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    float WaterMeshComponent::dhz(float x, float z, float t) {
        float res = 0;
        for (auto wf : wave_funcs) {
            res += wf.wavelength * wf.direction[1] * wf.amplitude * cos(cross2(wf.direction, glm::vec2(x, z)) * wf.wavelength + t * wf.phase_constant);
        }
        return res;
    }

    void WaterMeshComponent::update_vertices(Camera* scene_player) {
        int k = 20;
        int r = 20;
        int n = r * 2 + 1;
        vertices.resize(lod_count * n * n);
        indices.clear();
        for (int lod = 0; lod < lod_count; lod++) {
            int lod_verteces_offset = n * n * lod;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    float time = componentsObject->scene->time;

                    float x = (i - r) * k * (1<<lod);
                    float z = (j - r) * k * (1<<lod);
                    float y = h(x, z, time) - 370;
                    vertex &v = vertices[lod_verteces_offset + i * n + j];
                    v.position = {x, y, z}; //

                    glm::vec3 b = {1, dhx(x, z, time), 0};
                    glm::vec3 t = {0, dhz(x, z, time), 1};

                    v.normal = glm::cross(b, t);
                    if (v.normal.y < 0)
                        v.normal *= -1;
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

    WaterMeshComponent::WaterMeshComponent() {
        wave_funcs.push_back({1, {1, 0}, 0.3, 1});
//        wave_funcs.push_back({1, {0, 1}, 1, 1});
    }
} // yny