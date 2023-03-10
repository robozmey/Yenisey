//
// Created by vladimir on 28.12.22.
//

#include <iostream>
#include "base/Scene.h"
#include "tools/tools.h"

const char light_sum_vertex_shader_source[] =
        R"(#version 330 core

vec2 vertices[6] = vec2[6](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0, -1.0),
    vec2( 1.0,  1.0),
    vec2(-1.0,  1.0)
);

uniform float screen_height;
uniform float screen_width;

out vec2 texcoord;

void main()
{
    vec2 position = vertices[gl_VertexID];
    gl_Position = vec4(position, 0.0, 1.0);
    texcoord = (position + 1);
    texcoord.x *= screen_width / 2048 / 2;
    texcoord.y *= screen_height / 2048 / 2;
}
)";

const char light_sum_fragment_shader_source[] =
        R"(#version 330 core

uniform sampler2D light_render_map;
uniform float screen_height;

in vec2 texcoord;

layout (location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(texture(light_render_map, texcoord).rgb, 1.0);
}
)";

namespace yny {
    void Scene::update_vertices() {

        this->Object::update_vertices(sceneCamera);

    }

    void Scene::render() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        lightSources[0]->direction = {sin(time / 2), 1, cos(time / 2)};
        lightSources[1]->position = {sin(time) * 1000, 700, cos(time) * 1000};

        /// Render SHADOWS
//        for (auto vertex : scene.vertices) {
//            for (int i = 0; i < 3; i++) {
//                bounding_box[i][0] = std::min(bounding_box[i][0], vertex.position[i]);
//                bounding_box[i][1] = std::max(bounding_box[i][1], vertex.position[i]);
//            }
//        }

        C = {0, 0, 0};
        C = sceneCamera->get_camera_position();
//        for (auto vertex : scene.vertices) {
//            for (int i = 0; i < 3; i++) {
//                C[i] += vertex.position[i];
//            }
//        }
//        for (int i = 0; i < 3; i++) {
//            C[i] /= scene.vertices.size();
//        }

        for (auto lightSource : lightSources) {
            lightSource->shadow_render(scene);
        }


        /// Render LIGHTS
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, sceneCamera->width, sceneCamera->height);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, light_fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        glViewport(0, 0, sceneCamera->width, sceneCamera->height);

        for (auto lightSource : lightSources) {
            /// Render Light
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, light_render_fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);

            this->Object::light_render(sceneCamera, lightSource);

            /// Sum Light
            glBindTexture(GL_TEXTURE_2D, light_render_map);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, light_fbo);
            glClear(GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, light_render_map);

            glBindVertexArray(light_sum_vao);
            glUseProgram(light_sum_program);

            glUniform1f(screen_height_location, sceneCamera->height);
            glUniform1f(screen_width_location, sceneCamera->width);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glDisable(GL_BLEND);

        }

        glBindTexture(GL_TEXTURE_2D, light_map);
        glGenerateMipmap(GL_TEXTURE_2D);

        /// MAIN

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);


        skybox.render(sceneCamera);

        glClear(GL_DEPTH_BUFFER_BIT);

        this->Object::render(sceneCamera, light_map);

        glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
//        glClearColor(0.8f, 0.8f, 1.f, 0.f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->Object::render_interface(sceneCamera, light_map);

//        glClearColor(0.8f, 0.8f, 1.f, 0.f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
////        lightSources[0]->shadow_render(this);
//        this->Object::shadow_render(lightSources[0]);
//        this->Object::light_render(sceneCamera, lightSources[0]);

    }

    void Scene::update() {
        this->Object::update();
    }

    void Scene::create_scene() {
        scene = this;

        light_map_resolution = 2048;

        glGenTextures(1, &light_render_map);
        glBindTexture(GL_TEXTURE_2D, light_render_map);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, light_map_resolution, light_map_resolution, 0, GL_RGBA, GL_FLOAT, nullptr);

        glGenFramebuffers(1, &light_render_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, light_render_fbo);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, light_render_map, 0);
        if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Incomplete framebuffer!");

        glGenRenderbuffers(1, &light_render_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, light_render_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, light_map_resolution, light_map_resolution);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, light_render_rbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glGenTextures(1, &light_map);
        glBindTexture(GL_TEXTURE_2D, light_map);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, light_map_resolution, light_map_resolution, 0, GL_RGBA, GL_FLOAT, nullptr);

        glGenFramebuffers(1, &light_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, light_fbo);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, light_map, 0);
        if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Incomplete framebuffer!");

        glGenRenderbuffers(1, &light_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, light_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, light_map_resolution, light_map_resolution);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, light_rbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        auto light_sum_vertex_shader = create_shader(GL_VERTEX_SHADER, light_sum_vertex_shader_source);
        auto light_sum_fragment_shader = create_shader(GL_FRAGMENT_SHADER, light_sum_fragment_shader_source);
        light_sum_program = create_program(light_sum_vertex_shader, light_sum_fragment_shader);

        screen_height_location = glGetUniformLocation(light_sum_program, "screen_height");
        screen_width_location = glGetUniformLocation(light_sum_program, "screen_width");
        light_render_map_location = glGetUniformLocation(light_sum_program, "light_render_map");

        glGenVertexArrays(1, &light_sum_vao);



        /// LIGHT

        LightSource* directionalLightSource = new LightSource(DirectionalLight);
        directionalLightSource->direction = {sin(time / 2), 1, cos(time / 2)};
        lightSources.push_back(directionalLightSource);
//
//        LightSource* directionalLightSource2 = new LightSource(DirectionalLight);
//        directionalLightSource2->direction = {sin(time / 2), 1, cos(time / 2)};
//        lightSources.push_back(directionalLightSource2);

        LightSource* spotLightSource = new LightSource(SpotLight);
        spotLightSource->color = {1, 1, 0};
        spotLightSource->position = {0, 500, 0};
        lightSources.push_back(spotLightSource);

        LightSource* ambientLightSource = new LightSource();
        lightSources.push_back(ambientLightSource);

    }

    Scene::Scene() : Object("Scene") {
        create_scene();
    }

    Scene::Scene(std::string name) : Object(name) {
        create_scene();
    }
} // yny