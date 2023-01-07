//
// Created by vladimir on 31.12.22.
//

#include "component/TransformComponent.h"
#include "Object.h"

glm::mat4 rotation_matrix(glm::vec3 rotation) {
    glm::mat4 view(1.f);
    view = glm::rotate(view, rotation.x, {1, 0, 0});
    view = glm::rotate(view, rotation.y, {0, 1, 0});
    view = glm::rotate(view, rotation.z, {0, 0, 1});
    return view;
}

namespace yny {
    TransformComponent::TransformComponent() {}

    TransformComponent::TransformComponent(glm::vec3 position) : position(position) {}

    void TransformComponent::move(glm::vec3 movement) {
        position += movement;
    }

    void TransformComponent::rotate(glm::vec3 additional_rotation) {
        rotation += additional_rotation;
    }

    glm::mat4 TransformComponent::get_transform() {
        glm::mat4 res(1);

        if (componentsObject != nullptr && componentsObject->parentObject != nullptr) {
            res = (reinterpret_cast<TransformComponent*>(componentsObject->parentObject->components[Transform]))->get_transform();
        }

        res = res * rotation_matrix(rotation);
        res = glm::translate(res, position);

        return res;
    }

    glm::vec3 get_position(glm::mat4 transform) {
        return glm::inverse(transform) * glm::vec4(0, 0, 0, 1);
    }

    glm::vec3 TransformComponent::get_position() {
        auto transform = get_transform();
        return ::yny::get_position(transform);
    }

    glm::vec3 get_rotation(glm::mat4 transform) {

        double h, p, b; // angles in degrees

// extract pitch
        double sinP = -transform[1][2];
        if (sinP >= 1) {
            p = glm::pi<float>() / 2; }       // pole
        else if (sinP <= -1) {
            p = -glm::pi<float>() / 2; } // pole
        else {
            p = asin(sinP); }

// extract heading and bank
        if (sinP < -0.9999 || sinP > 0.9999) { // account for small angle errors
            h = atan2(-transform[2][0], transform[0][0]);
            b = 0; }
        else {
            h = atan2(transform[0][2], transform[2][2]);
            b = atan2(transform[1][0], transform[1][1]); }
        return {-p, -h, b};
    }

    glm::vec3 TransformComponent::get_rotation() {
        auto transform = get_transform();
        return ::yny::get_rotation(transform);
    }
} // yny