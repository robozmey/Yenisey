//
// Created by vladimir on 07.01.23.
//

#include "FreeCameraScriptComponent.h"
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

    void FreeCameraScriptComponent::update() {

        std::map<SDL_Keycode, bool> button_down = Component::componentsObject->scene->input.button_down;
        float dt =  Component::componentsObject->scene->dt;

        TransformComponent* tc = reinterpret_cast<yny::TransformComponent *>(Component::componentsObject->components[Transform]);
//        printf("%f %f %f\n", tc->get_rotation().x, tc->get_rotation().y, tc->get_rotation().z);

        glm::vec3 movement(0);
        glm::vec3 rotation(0);

        if (button_down[SDLK_UP])
            rotation[0] += rotation_speed * dt;
        if (button_down[SDLK_DOWN])
            rotation[0] -= rotation_speed * dt;

        if (button_down[SDLK_LEFT])
            rotation[1] += rotation_speed * dt;
        if (button_down[SDLK_RIGHT])
            rotation[1] -= rotation_speed * dt;

//        tc->rotateX(rotation[0]);
//        tc->rotateY(rotation[1]);

        auto rot = rotation;
        tc->rotate(rot);

        glm::vec3 forward_rotation = tc->get_rotation();
        glm::vec3 forward_direction = (rotation_matrix(forward_rotation * 0.25f)) * glm::vec4(0, 0, movement_speed, 1);

        if (button_down[SDLK_w])
            movement -= forward_direction;
        if (button_down[SDLK_s])
            movement += forward_direction;

        glm::vec3 side_rotation = tc->get_rotation() + glm::vec3(0, glm::pi<float>() / 2, 0);
        glm::vec3 side_direction = (rotation_matrix(side_rotation)) * glm::vec4(0, 0, movement_speed, 1);

        if (button_down[SDLK_a])
            movement -= side_direction;
        if (button_down[SDLK_d])
            movement += side_direction;

        tc->move(movement);


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
    }
} // yny