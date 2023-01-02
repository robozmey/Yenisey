//
// Created by vladimir on 28.12.22.
//

#include "Object.h"
#include "component/TransformComponent.h"
#include "component/RenderComponent.h"
#include "component/RigibodyComponent.h"
#include <iostream>

namespace yny {

    void Object::apply_transform() {
        TransformComponent* transform_component = reinterpret_cast<TransformComponent *>(components[Transform]);

        for (vertex& v : vertices) {
            glm::mat4 transform = glm::mat4(1);
            transform = glm::rotate(transform, transform_component->rotation.x, {1, 0, 0});
            transform = glm::rotate(transform, transform_component->rotation.y, {0, 1, 0});
            transform = glm::rotate(transform, transform_component->rotation.z, {0, 0, 1});
            transform = glm::translate(transform, transform_component->position);

            v.position = glm::vec3(transform * glm::vec4(v.position, 1));
        }
    }

    void Object::update_vertices(Player& scene_player) {}

    void Object::render(Player& scene_player) {
        if (components.contains(Render)) {
            RenderComponent* renderComponent = static_cast<RenderComponent *>(components[Render]);
            renderComponent->render(scene_player);
        }
    }

    void Object::update_time(Player& scene_player) {
        if (components.contains(Rigibody)) {
            yny::RigibodyComponent* rc = reinterpret_cast<yny::RigibodyComponent *>(components[yny::Rigibody]);
            rc->move(scene_player.dt);

        }
    }

    void Object::add_component(ComponentType type) {
        switch (type) {
            case Transform:
                components[type] = new TransformComponent();
                break;
            case Render:
                components[type] = new RenderComponent();
                break;
            case Rigibody:
                components[type] = new RigibodyComponent();
                break;
        };
        components[type]->parentObject = this;
    }

    void Object::add_component(ComponentType type, Component* ptr) {
        components[type] = ptr;
        components[type]->parentObject = this;
    }

    Object::Object() {
        components[Transform] = new TransformComponent();
    };
} // yny