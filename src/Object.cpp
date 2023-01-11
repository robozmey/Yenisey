//
// Created by vladimir on 28.12.22.
//

#include "Object.h"
#include "Scene.h"
#include "component/TransformComponent.h"
#include "component/RenderComponent.h"
#include "component/RigibodyComponent.h"
#include "component/ColliderComponent.h"
#include "component/MeshComponent.h"
#include "component/ScriptComponent.h"
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

        if (components.contains(Mesh)) {
            MeshComponent* meshComponent = static_cast<MeshComponent *>(components[Mesh]);
            meshComponent->apply_transform();
        }

        for (Object* obj : objects) {
            obj->apply_transform();
        }
    }

    void Object::update_vertices(Camera* scene_player) {
        if (components.contains(Mesh)) {
            MeshComponent* meshComponent = static_cast<MeshComponent *>(components[Mesh]);
            meshComponent->update_vertices(scene_player);
        }

        for (Object* obj : objects) {
            obj->update_vertices(scene_player);
        }
    }

    void Object::light_render(Camera* scene_player, LightSource* lightSource) {
        if (components.contains(Render)) {
            RenderComponent* renderComponent = static_cast<RenderComponent *>(components[Render]);
            renderComponent->light_render(scene_player, lightSource);
        }

        for (Object* obj : objects) {
            obj->light_render(scene_player, lightSource);
        }
    }

    void Object::light_render(Camera* scene_player) {
        if (components.contains(Render)) {
            RenderComponent* renderComponent = static_cast<RenderComponent *>(components[Render]);
            renderComponent->light_render(scene_player);
        }

        for (Object* obj : objects) {
            obj->light_render(scene_player);
        }
    }

    void Object::render(Camera* scene_player, GLuint light_map) {
        if (components.contains(Render)) {
            RenderComponent* renderComponent = static_cast<RenderComponent *>(components[Render]);
            if (!renderComponent->is_interface)
                renderComponent->render(scene_player, light_map);
        }

        for (Object* obj : objects) {
            obj->render(scene_player, light_map);
        }
    }

    void Object::render_interface(Camera* scene_player, GLuint light_map) {
        if (components.contains(Render)) {
            RenderComponent* renderComponent = static_cast<RenderComponent *>(components[Render]);
            if (renderComponent->is_interface)
                renderComponent->render(scene_player, light_map);
        }

        for (Object* obj : objects) {
            obj->render_interface(scene_player, light_map);
        }
    }


    void Object::update() {

        if (components.contains(Rigibody)) {
            yny::RigibodyComponent* rc = reinterpret_cast<yny::RigibodyComponent *>(components[Rigibody]);
            rc->move(scene->dt);
        }

        if (components.contains(Script)) {
            yny::ScriptComponent* sc = static_cast<yny::ScriptComponent *>(components[Script]);
            sc->update();
        }

        for (Object* obj : objects) {
            obj->update();
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
        components[type]->componentsObject = this;
    }

    void Object::add_component(ComponentType type, Component* ptr) {
        components[type] = ptr;
        components[type]->componentsObject = this;
    }

    void Object::add_object(Object* obj) {
        obj->parentObject = this;
        obj->scene = scene;
        objects.push_back(obj);
    }

    bool is_collide_components(Object* object, Object* other_object) {
        if (object->components.contains(Collider) && other_object->components.contains(Collider)) {
            yny::ColliderComponent* cc1 = static_cast<yny::ColliderComponent *>(object->components[yny::Collider]);
            yny::ColliderComponent* cc2 = static_cast<yny::ColliderComponent *>(other_object->components[yny::Collider]);
            if (cc1 == cc2)
                return false;
            return cc1->is_collide(cc2);
        }
        return false;
    }

    bool Object::is_collide(Object* other_object) {

        if (this == other_object)
            return false;

        if (is_collide_components(this, other_object) || is_collide_components(other_object, this))
            return true;

        for (Object* sub_other_object : other_object->objects) {
            if (this->is_collide(sub_other_object))
                return true;
        }

//        for (Object* sub_object : objects) {
//            if (sub_object->is_collide(other_object))
//                return true;
//        }
        return false;
    }

    Object::Object() {
        parentObject = nullptr;
        add_component(Transform);
    };

    Object::Object(std::string name) : name(name) {
        parentObject = nullptr;
        add_component(Transform);
    };
} // yny