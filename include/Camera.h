//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_CAMERA_H
#define YENISEY_CAMERA_H

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

#include "Object.h"

namespace yny {

    float wgs_distance(float lat1, float lon1, float lat2, float lon2);

    class Object;

    class Camera : public Object {
    public:
        glm::vec3 inner_camera_position = glm::vec3(0, -800, -100);
        glm::vec3 camera_rotation = glm::vec3(0.5, 0, 0);

        glm::vec3 camera_position;

        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projection = glm::mat4(1.f);

        float near = 0.1f;
        float far = 1000000.f;

        int width, height;

        void update_screen(int width, int height);

        glm::vec3 get_camera_position();
        glm::vec3 get_camera_rotation();

        glm::mat4 get_camera_model();

        glm::mat4 get_camera_view();

        glm::mat4 get_camera_projection();

        explicit Camera(std::string name);
        Camera();
    };


} // yny

#endif //YENISEY_CAMERA_H
