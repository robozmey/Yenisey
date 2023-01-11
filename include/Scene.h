//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_SCENE_H
#define YENISEY_SCENE_H

#include <vector>

#include "Object.h"
#include "Camera.h"
#include "Skybox.h"
#include "Input.h"

namespace yny {

    class Scene : public Object {
    public:
        Camera* sceneCamera;
        Skybox skybox;

        Input input;

        float time;
        float dt;

        void update_vertices();

        GLsizei light_map_resolution;
        GLuint light_render_map;
        GLuint light_render_fbo;
        GLuint light_render_rbo;
        GLuint light_map;
        GLuint light_fbo;
        GLuint light_rbo;

        GLuint light_sum_vao;

        GLuint screen_height_location;
        GLuint screen_width_location;
        GLuint light_render_map_location;

        GLuint light_sum_program;

        void render();

        void update();

        void create_scene();

        Scene();

        explicit Scene(std::string);
    };

} // yny

#endif //YENISEY_SCENE_H
