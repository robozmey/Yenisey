//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_OBJECT_H
#define YENISEY_OBJECT_H

#include "component/Component.h"
#include "Player.h"

#include <vector>

namespace yny {

    class Scene;

    class Object {
    public:
        std::map<ComponentType, Component*> components;

        Scene* scene;
        Object* parentObject;
        std::vector<Object*> objects;

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        void apply_transform();

        virtual void update_vertices(Player& scene_player);
        virtual void render(Player& scene_player);
        void update_time(Player& scene_player);

        void add_object(Object*);

        void add_component(ComponentType);
        void add_component(ComponentType, Component*);

        bool is_collide(Object*);

        Object();
    };

} // yny

#endif //YENISEY_OBJECT_H
