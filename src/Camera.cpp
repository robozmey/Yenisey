//
// Created by vladimir on 28.12.22.
//

#include "Camera.h"
#include "component/TransformComponent.h"

namespace yny {

    void Camera::update_screen(int width1, int height1) {
        width = width1;
        height = height1;
    }

    glm::vec3 Camera::get_camera_position() {
        TransformComponent* tc = reinterpret_cast<TransformComponent *>(Object::components[Transform]);
        return tc->get_position();
    }

    glm::vec3 Camera::get_camera_rotation() {
        TransformComponent* tc = reinterpret_cast<TransformComponent *>(Object::components[Transform]);
        return tc->get_rotation();
    }

    glm::mat4 Camera::get_camera_model() {
        return model;
    }

    glm::mat4 Camera::get_camera_view() {
        TransformComponent* tc = reinterpret_cast<TransformComponent *>(Object::components[Transform]);
        glm::mat4 res = tc->get_transform();
//        res = glm::translate(res, tc->get_position() * (-2.f));
        return glm::inverse(res);
    }

    glm::mat4 Camera::get_camera_projection() {
        float top = near;
        float right = (top * width) / height;

        projection = glm::mat4(1.f);
        projection = glm::perspective(glm::pi<float>() / 2.f, (1.f * width) / height, near, far);

        return projection;
    }

    Camera::Camera(std::string name) : Object(name) {}

    Camera::Camera() : Object() {}


} // yny