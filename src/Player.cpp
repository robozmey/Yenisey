//
// Created by vladimir on 28.12.22.
//

#include "Player.h"

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

    void Player::move(std::map<SDL_Keycode, bool> button_down, float new_time) {

        dt = new_time - time;
        time = new_time;

        if (button_down[SDLK_UP])
            camera_rotation[0] -= rotation_speed * dt;
        if (button_down[SDLK_DOWN])
            camera_rotation[0] += rotation_speed * dt;

        if (button_down[SDLK_LEFT])
            camera_rotation[1] -= rotation_speed * dt;
        if (button_down[SDLK_RIGHT])
            camera_rotation[1] += rotation_speed * dt;

        camera_direction = glm::vec4(0, 0, movement_speed, 1) * rotation_matrix(camera_rotation);

        if (button_down[SDLK_w])
            inner_camera_position += camera_direction;
        if (button_down[SDLK_s])
            inner_camera_position -= camera_direction;

        side_direction = glm::vec4(movement_speed, 0, 0, 1) * rotation_matrix(camera_rotation);

        if (button_down[SDLK_a])
            inner_camera_position += side_direction;
        if (button_down[SDLK_d])
            inner_camera_position -= side_direction;
//        return;

        float max_dist_x = wgs_distance(latitude + latitude_minute / 60.f,
                                        longitude + longitude_minute / 60.f,
                                        latitude + (latitude_minute + 1) / 60.f,
                                        longitude + longitude_minute / 60.f) * 1000;
        float max_dist_z = wgs_distance(latitude + latitude_minute / 60.f,
                                        longitude + longitude_minute / 60.f,
                                        latitude + (latitude_minute) / 60.f,
                                        longitude + (longitude_minute + 1) / 60.f) * 1000;

        if (abs(inner_camera_position.x) > max_dist_x) {
            if (inner_camera_position.x > max_dist_x) {
                inner_camera_position.x -= max_dist_x;
                latitude_minute -= 1;
            } else {
                inner_camera_position.x += max_dist_x;
                latitude_minute += 1;
            }
        }
        if (abs(inner_camera_position.z) > max_dist_z) {
            if (inner_camera_position.z > max_dist_z) {
                inner_camera_position.z -= max_dist_z;
                longitude_minute += 1;
            } else {
                inner_camera_position.z += max_dist_z;
                longitude_minute -= 1;
            }
        }

        this->recalc();
    }

    void Player::update_screen(int width1, int height1) {
        width = width1;
        height = height1;
    }

    void Player::recalc() {
        view = glm::mat4(1);
        view = glm::rotate(view, camera_rotation.x, {1, 0, 0});
        view = glm::rotate(view, camera_rotation.y, {0, 1, 0});
        view = glm::rotate(view, camera_rotation.z, {0, 0, 1});
        view = glm::translate(view, inner_camera_position);

        float top = near;
        float right = (top * width) / height;

        projection = glm::mat4(1.f);
        projection = glm::perspective(glm::pi<float>() / 2.f, (1.f * width) / height, near, far);

        camera_position = (glm::inverse(view) * glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
} // yny