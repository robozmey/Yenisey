//
// Created by vladimir on 06.01.23.
//

#ifndef YENISEY_PLAYER_H
#define YENISEY_PLAYER_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <map>

namespace yny {

    class Player {
        void move(std::map<SDL_Keycode, bool> button_down, float new_time);
    };

} // yny

#endif //YENISEY_PLAYER_H
