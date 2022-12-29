//
// Created by vladimir on 29.12.22.
//

#ifndef YENISEY_SKYBOX_H
#define YENISEY_SKYBOX_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include "Player.h"

namespace yny {

    class Skybox {
    public:
        GLuint vao;

        GLuint program;

        GLuint environment_texture;

        GLuint view_location;
        GLuint projection_location;
        GLuint camera_position_location;
        GLuint environment_texture_location;

        void render(Player& scene_player);

        Skybox();
    };

} // yny

#endif //YENISEY_SKYBOX_H
