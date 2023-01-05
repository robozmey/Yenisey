//
// Created by vladimir on 05.01.23.
//

#ifndef YENISEY_MATERIAL_H
#define YENISEY_MATERIAL_H

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
#include <string>

namespace yny {

    enum MaterialType {
        OneColorMaterial,
        TextureMaterial,
    };

    class Material {
    public:

        MaterialType materialType;

        GLuint texture;
        std::string texture_path;
        GLuint texture_normal;
        std::string texture_normal_path;

        glm::vec4 color;

        Material();

        explicit Material(glm::vec3);

        explicit Material(glm::vec4);

        explicit Material(std::string texture_path);

    private:
        void create_one_color(glm::vec4);
    };

} // yny

#endif //YENISEY_MATERIAL_H
