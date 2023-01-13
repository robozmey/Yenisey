//
// Created by vladimir on 09.01.23.
//

#include "scripts/PlayerScriptComponent.h"

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

#include "base/Object.h"
#include "base/Scene.h"
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

//    std::u32string to_u32string(float val) {
//        return to_u32string(std::to_string(val));
//    }

    void PlayerScriptComponent::update() {

        std::map<SDL_Keycode, bool> button_down = Component::componentsObject->scene->input.button_down;
        float dt =  Component::componentsObject->scene->dt;

        TransformComponent* tc = reinterpret_cast<yny::TransformComponent *>(Component::componentsObject->components[Transform]);
        TransformComponent* elevation_tc = reinterpret_cast<yny::TransformComponent *>(elevationObject->components[Transform]);

        int latitude = elevationDataObject->latitude;
        int longitude = elevationDataObject->longitude;
        int latitude_minute = elevationDataObject->latitude_minute;
        int longitude_minute = elevationDataObject->longitude_minute;
        std::u32string latitude_prefix = std::u32string(latitude > 0 ? U"N" : U"S") + (latitude < 10 ? U"0" : U"");
        std::u32string longitude_prefix = std::u32string(longitude > 0 ? U"E" : U"W") +
                (longitude < 10 ? U"00" : (longitude < 100 ? U"0" : U""));

        std::u32string text;
//        text += latitude_prefix + to_u32string(latitude) + U"\260 " + to_u32string(latitude_minute) + U"'" +
//                U" " +
//                longitude_prefix + to_u32string(longitude) + U"\260 " + to_u32string(longitude_minute) + U"'" +
//                U"    ";
        text += U"  X: " + to_u32string(tc->get_position().x);
        text += U", Y: " +  to_u32string(tc->get_position().y);
        text += U", Z: " +  to_u32string(tc->get_position().z);
        text += U"  rX: " + to_u32string(tc->get_rotation().x / glm::pi<float>() * 180);
        text += U", rY: " +  to_u32string(tc->get_rotation().y / glm::pi<float>() * 180);
        text += U", rZ: " +  to_u32string(tc->get_rotation().z / glm::pi<float>() * 180);
        interfaceData->text = text;
        interfaceData->text_changed = true;

//        printf("%f %f %f\n", tc->get_rotation().x, tc->get_rotation().y, tc->get_rotation().z);

        glm::vec3 movement(0);
        glm::vec3 rotationX(0);
        glm::vec3 rotationY(0);

        if (button_down[SDLK_UP])
            rotationX[0] += rotation_speed * dt;
        if (button_down[SDLK_DOWN])
            rotationX[0] -= rotation_speed * dt;

        if (button_down[SDLK_LEFT])
            rotationY[1] += rotation_speed * dt;
        if (button_down[SDLK_RIGHT])
            rotationY[1] -= rotation_speed * dt;

//        tc->rotateX(rotation[0]);
//        tc->rotateY(rotation[1]);

        player_rotation += rotationY;
        tc->set_rotation(player_rotation);
        tc->rotate(rotationX);
        player_rotation = tc->get_rotation();

        glm::vec3 forward_rotation = tc->get_rotation(); //{0, tc->get_rotation().y, 0};
        glm::vec3 forward_direction = (rotation_matrix(forward_rotation)) * glm::vec4(0, 0, movement_speed, 1);
//        printf("%s: %f %f %f\n", "fd", forward_direction.x, forward_direction.y, forward_direction.z);

        if (button_down[SDLK_w])
            movement -= forward_direction;
        if (button_down[SDLK_s])
            movement += forward_direction;

        glm::vec3 side_direction = (rotation_matrix(forward_rotation)) * glm::vec4(movement_speed, 0, 0, 1);

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

        if (abs(tc->get_position().x - elevation_tc->get_position().x) > max_dist_x * 0.9) {
            if (tc->get_position().x - elevation_tc->get_position().x > 0) {
                elevation_tc->move({max_dist_x, 0, 0});
                elevationDataObject->move(0, 0, 1, 0);
            } else {
                elevation_tc->move({-max_dist_x, 0, 0});
                elevationDataObject->move(0, 0, -1, 0);
            }
            interfaceData->text_changed = true;
        }
        if (abs(tc->get_position().z - elevation_tc->get_position().z) > max_dist_z * 0.9) {
            if (tc->get_position().z - elevation_tc->get_position().z > 0) {
                elevation_tc->move({0, 0, max_dist_z});
                elevationDataObject->move(0, 0, 0, -1);
            } else {
                elevation_tc->move({0, 0, -max_dist_z});
                elevationDataObject->move(0, 0, 0, 1);
            }
            interfaceData->text_changed = true;
        }
    }

    PlayerScriptComponent::PlayerScriptComponent(InterfaceData *interfaceData, ElevationDataObject* elevationDataObject, Object* elevationObject)
        : interfaceData(interfaceData), elevationDataObject(elevationDataObject), elevationObject(elevationObject) {

    }
} // yny