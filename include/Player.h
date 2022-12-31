//
// Created by vladimir on 28.12.22.
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

    float wgs_distance(float lat1, float lon1, float lat2, float lon2);

    class Player {
    public:
        float time = 0;

        float rotation_speed = 1;
        float movement_speed = 10;

        glm::vec3 inner_camera_position = glm::vec3(0, -200, -100);
        glm::vec3 camera_rotation = glm::vec3(0.5, 0, 0);
        glm::vec3 camera_direction, side_direction;

        glm::vec3 camera_position;

        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        glm::mat4 projection = glm::mat4(1.f);

        int latitude = 55, longitude = 92;
        int latitude_minute = 29, longitude_minute = 30;

        float near = 0.1f;
        float far = 100000.f;

        int width, height;

        void move(std::map<SDL_Keycode, bool> button_down, float dt);

        void update_screen(int width, int height);

        void recalc();
    };


} // yny

#endif //YENISEY_PLAYER_H
