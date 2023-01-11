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

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"

std::string to_string(std::string_view str);

void sdl2_fail(std::string_view message);

void glew_fail(std::string_view message, GLenum error);

GLuint create_shader(GLenum type, const char * source);

GLuint load_texture(std::string const & path);

glm::vec3 get_triangle_normal(glm::vec3 a, glm::vec3 b, glm::vec3 c);

template <typename ... Shaders>
GLuint create_program(Shaders ... shaders);

#include "create_program.h"

#endif //YENISEY_TOOLS_H