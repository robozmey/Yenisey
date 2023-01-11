//
// Created by vladimir on 09.01.23.
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

    std::u32string to_u32string(std::string s) {
        std::u32string s32;
        s32.clear();  // if not already empty
        for (unsigned char c : s) {s32 += c;}
        return s32;
    }

    std::u32string to_u32string(int val) {
        return to_u32string(std::to_string(val));
    }

    void PlayerScriptComponent::update() {

        std::map<SDL_Keycode, bool> button_down = Component::componentsObject->scene->input.button_down;
        float dt =  Component::componentsObject->scene->dt;


        int latitude = elevationDataObject->latitude;
        int longitude = elevationDataObject->longitude;
        int latitude_minute = elevationDataObject->latitude_minute;
        int longitude_minute = elevationDataObject->longitude_minute;
        std::u32string latitude_prefix = std::u32string(latitude > 0 ? U"N" : U"S") + (latitude < 10 ? U"0" : U"");
        std::u32string longitude_prefix = std::u32string(longitude > 0 ? U"E" : U"W") +
                (longitude < 10 ? U"00" : (longitude < 100 ? U"0" : U""));

        std::u32string text = latitude_prefix + to_u32string(latitude) + U"^ " + to_u32string(latitude_minute) +
                U" " +
                longitude_prefix + to_u32string(longitude) + U"^ " + to_u32string(longitude_minute);
        interfaceData->text = text;


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

        glm::vec3 forward_rotation = {0, tc->get_rotation().y, 0};
        glm::vec3 forward_direction = (rotation_matrix(forward_rotation)) * glm::vec4(0, 0, movement_speed, 1);

        if (button_down[SDLK_w])
            movement -= forward_direction;
        if (button_down[SDLK_s])
            movement += forward_direction;

        glm::vec3 side_rotation = forward_rotation + glm::vec3(0, glm::pi<float>() / 2, 0);
        glm::vec3 side_direction = (rotation_matrix(side_rotation)) * glm::vec4(0, 0, movement_speed, 1);

        if (button_down[SDLK_a])
            movement -= side_direction;
        if (button_down[SDLK_d])
            movement += side_direction;

        tc->move(movement);


        float max_dist_x = wgs_distance(latitude + latitude_minute / 60.f,
                                        longitude + longitude_minute / 60.f,
                                        latitude + (latitude_minute + 1) / 60.f,
                                        longitude + longitude_minute / 60.f) * 1000;
        float max_dist_z = wgs_distance(latitude + latitude_minute / 60.f,
                                        longitude + longitude_minute / 60.f,
                                        latitude + (latitude_minute) / 60.f,
                                        longitude + (longitude_minute + 1) / 60.f) * 1000;

        if (abs(tc->get_position().x) > max_dist_x) {
            if (tc->get_position().x > max_dist_x) {
                tc->move({-max_dist_x, 0, 0});
                elevationDataObject->move(0, 0, -1, 0);
            } else {
                tc->move({max_dist_x, 0, 0});
                elevationDataObject->move(0, 0, 1, 0);
            }
            interfaceData->text_changed = true;
        }
        if (abs(tc->get_position().z) > max_dist_z) {
            if (tc->get_position().z > max_dist_z) {
                tc->move({0, 0, -max_dist_z});
                elevationDataObject->move(0, 0, 0, -1);
            } else {
                tc->move({0, 0, max_dist_z});
                elevationDataObject->move(0, 0, 0, 1);
            }
            interfaceData->text_changed = true;
        }
    }

    PlayerScriptComponent::PlayerScriptComponent(InterfaceData *interfaceData, ElevationDataObject* elevationDataObject)
        : interfaceData(interfaceData), elevationDataObject(elevationDataObject) {

    }
} // yny