#ifndef YENISEY_TOOLS_H
#define YENISEY_TOOLS_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include <string>
#include <iostream>

std::string to_string(std::string_view str);

void sdl2_fail(std::string_view message);

void glew_fail(std::string_view message, GLenum error);

GLuint create_shader(GLenum type, const char * source);

GLuint load_texture(std::string const & path);

template <typename ... Shaders>
GLuint create_program(Shaders ... shaders);

#include "create_program.h"

#endif //YENISEY_TOOLS_H