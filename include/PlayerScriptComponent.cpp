//
// Created by vladimir on 07.01.23.
//

#include "PlayerScriptComponent.h"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>
#include <map>

#include "Object.h"
#include "Scene.h"
#include "component/TransformComponent.h"

namespace yny {

    float wgs_distance(float lat1, float lon1, float lat2, float lon2) {
        double p = 0.017453292519943295;    // Math.PI / 180
        double a = 0.5 - cos((lat2 - lat1) * p)/2 +
                   cos(lat1 * p) * cos(lat2 * p) *
                   (1 - cos((lon2 - lon1) * p))/2;

        return 12742 * asin(sqrt(a)); // 2 * R; R = 6371 km
    }

    glm::mat4 rotation_matrix(glm::vec3 camera_rotation) {
        glm::mat4 view(1.f);
        view = glm::rotate(view, camera_rotation.x, {1, 0, 0});
        view = glm::rotate(view, camera_rotation.y, {0, 1, 0});
        view = glm::rotate(view, camera_rotation.z, {0, 0, 1});
        return view;
    }

    void PlayerScriptComponent::update() {

        std::map<SDL_Keycode, bool> button_down = Component::componentsObject->scene->input.button_down;
        float dt =  Component::componentsObject->scene->dt;

        TransformComponent* tc = reinterpret_cast<yny::TransformComponent *>(Component::componentsObject->components[Transform]);

        glm::vec3 movement(0);
        glm::vec3 rotation(0);

        if (button_down[SDLK_UP])
            rotation[0] -= rotation_speed * dt;
        if (button_down[SDLK_DOWN])
            rotation[0] += rotation_speed * dt;

        if (button_down[SDLK_LEFT])
            rotation[1] -= rotation_speed * dt;
        if (button_down[SDLK_RIGHT])
            rotation[1] += rotation_speed * dt;

        glm::vec3 camera_direction, side_direction;
        camera_direction = glm::vec4(0, 0, movement_speed, 1) * rotation_matrix(tc->get_rotation());

        if (button_down[SDLK_w])
            movement += camera_direction;
        if (button_down[SDLK_s])
            movement -= camera_direction;

        side_direction = glm::vec4(movement_speed, 0, 0, 1) * rotation_matrix(tc->get_rotation());

        if (button_down[SDLK_a])
            movement += side_direction;
        if (button_down[SDLK_d])
            movement -= side_direction;


//        float max_dist_x = wgs_distance(latitude + latitude_minute / 60.f,
//                                        longitude + longitude_minute / 60.f,
//                                        latitude + (latitude_minute + 1) / 60.f,
//                                        longitude + longitude_minute / 60.f) * 1000;
//        float max_dist_z = wgs_distance(latitude + latitude_minute / 60.f,
//                                        longitude + longitude_minute / 60.f,
//                                        latitude + (latitude_minute) / 60.f,
//                                        longitude + (longitude_minute + 1) / 60.f) * 1000;
//
//        if (abs(inner_camera_position.x) > max_dist_x) {
//            if (inner_camera_position.x > max_dist_x) {
//                inner_camera_position.x -= max_dist_x;
//                latitude_minute -= 1;
//            } else {
//                inner_camera_position.x += max_dist_x;
//                latitude_minute += 1;
//            }
//        }
//        if (abs(inner_camera_position.z) > max_dist_z) {
//            if (inner_camera_position.z > max_dist_z) {
//                inner_camera_position.z -= max_dist_z;
//                longitude_minute += 1;
//            } else {
//                inner_camera_position.z += max_dist_z;
//                longitude_minute -= 1;
//            }
//        }


        tc->move(movement);
        tc->rotate(rotation);
    }
} // yny