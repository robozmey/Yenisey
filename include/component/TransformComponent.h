//
// Created by vladimir on 31.12.22.
//

#ifndef YENISEY_TRANSFORMCOMPONENT_H
#define YENISEY_TRANSFORMCOMPONENT_H

#include "component/Component.h"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace yny {

    glm::mat4 rotation_matrix(glm::vec3 rotation);

    glm::vec3 get_rotation(glm::mat4 transform);

    class TransformComponent : public Component {
    public:

        ComponentType type = Transform;

        glm::mat4 transform = glm::mat4(1);

        glm::vec3 position = {0, 0, 0};
        glm::vec3 rotation = {0, 0, 0};
        glm::vec3 scale = {1, 1, 1};

        explicit TransformComponent(glm::vec3 position);

        TransformComponent();

        void move(glm::vec3 movement);
        void rotate(glm::vec3 rotation1);
        void rotateX(float x);
        void rotateY(float y);
        void rotateZ(float z);

        glm::mat4 get_transform();

        glm::vec3 get_position();

        glm::vec3 get_rotation();
    };

} // yny

#endif //YENISEY_TRANSFORMCOMPONENT_H
