//
// Created by vladimir on 05.01.23.
//

#include <stdexcept>
#include "base/LightSource.h"
#include "base/Scene.h"

namespace yny {
    LightSource::LightSource() : lightSourceType(AmbientLight), light_intensity(0.2) {
        create_light_source();
    }

    LightSource::LightSource(LightSourceType lightSourceType) : lightSourceType(lightSourceType), light_intensity(0.2) {
        create_light_source();
    }

    void LightSource::create_light_source() {
        if (lightSourceType == AmbientLight)
            return;
        if (lightSourceType == DirectionalLight) {

            glGenTextures(1, &shadow_map);
            glBindTexture(GL_TEXTURE_2D, shadow_map);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, shadow_map_resolution, shadow_map_resolution, 0, GL_RGBA, GL_FLOAT,
                         nullptr);
            glGenerateMipmap(GL_TEXTURE_2D);

            glGenFramebuffers(1, &shadow_fbo);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_fbo);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadow_map, 0);
            if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw std::runtime_error("Incomplete framebuffer!");

            glGenRenderbuffers(1, &shadow_rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, shadow_rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, shadow_map_resolution, shadow_map_resolution);
            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_rbo);
        }
    }

    void LightSource::shadow_render(Scene *scene) {
        if (lightSourceType == DirectionalLight) {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_fbo);
//            glClearColor(0.0f, 0.6f, 0.6f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, shadow_map_resolution, shadow_map_resolution);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glm::vec3 light_z = glm::normalize(-direction);
            glm::vec3 light_x = glm::normalize(glm::cross(light_z, {0.f, 1.f, 0.f}));
            glm::vec3 light_y = glm::normalize(glm::cross(light_x, light_z));
            float shadow_scale = 2.f;
            float shadow_scale_x = -1e8;
            float shadow_scale_y = -1e8;
            float shadow_scale_z = -1e8;

            for (int i = 0; i < 8; i++) {
                glm::vec3 V = glm::vec3(scene->bounding_box[0][i / 4], scene->bounding_box[1][i / 2 % 2], scene->bounding_box[2][i % 2]);

                float tmp;
                tmp = glm::dot(V - scene->C, light_x);
                shadow_scale_x = std::max(shadow_scale_x, std::abs(tmp));

                tmp = glm::dot(V - scene->C, light_y);
                shadow_scale_y = std::max(shadow_scale_y, std::abs(tmp));

                tmp = glm::dot(V - scene->C, light_z);
                shadow_scale_z = std::max(shadow_scale_z, std::abs(tmp));
            }

            shadow_transform = glm::mat4(1.f);
            for (size_t i = 0; i < 3; ++i)
            {
                shadow_transform[i][0] = shadow_scale_x * light_x[i];
                shadow_transform[i][1] = shadow_scale_y * light_y[i];
                shadow_transform[i][2] = shadow_scale_z * light_z[i];
                shadow_transform[i][3] = scene->C[i];
            }

            shadow_transform = glm::transpose(shadow_transform);
            shadow_transform = glm::inverse(shadow_transform);


            scene->Object::shadow_render(this);

            glBindTexture(GL_TEXTURE_2D, shadow_map);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
} // yny