//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_INPUT_H
#define YENISEY_INPUT_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include <map>

namespace yny {

    class Input {
    public:
        std::map<SDL_Keycode, bool> button_down;
    };

} // yny

#endif //YENISEY_INPUT_H
