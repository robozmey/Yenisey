//
// Created by vladimir on 06.01.23.
//

#include "Player.h"

namespace yny {

    void Player::move(std::map<SDL_Keycode, bool> button_down, float new_time) {

//        dt = new_time - time;
//        time = new_time;
//
//        if (button_down[SDLK_UP])
//            camera_rotation[0] -= rotation_speed * dt;
//        if (button_down[SDLK_DOWN])
//            camera_rotation[0] += rotation_speed * dt;
//
//        if (button_down[SDLK_LEFT])
//            camera_rotation[1] -= rotation_speed * dt;
//        if (button_down[SDLK_RIGHT])
//            camera_rotation[1] += rotation_speed * dt;
//
//        camera_direction = glm::vec4(0, 0, movement_speed, 1) * rotation_matrix(camera_rotation);
//
//        if (button_down[SDLK_w])
//            inner_camera_position += camera_direction;
//        if (button_down[SDLK_s])
//            inner_camera_position -= camera_direction;
//
//        side_direction = glm::vec4(movement_speed, 0, 0, 1) * rotation_matrix(camera_rotation);
//
//        if (button_down[SDLK_a])
//            inner_camera_position += side_direction;
//        if (button_down[SDLK_d])
//            inner_camera_position -= side_direction;

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

//        this->recalc();
    }
} // yny