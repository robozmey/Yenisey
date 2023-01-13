//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_OBJECT_H
#define YENISEY_OBJECT_H

#ifdef WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <GL/glew.h>

#include "component/Component.h"
#include "LightSource.h"

#include <vector>
#include <string>
#include <map>

namespace yny {

    class Scene;

    class Camera;

    class Object {
    public:
        std::map<ComponentType, Component*> components;

        std::string name = "Unnamed object";

        Scene* scene;
        Object* parentObject = nullptr;
        std::vector<Object*> objects;

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        void apply_transform();

        virtual void update_vertices(Camera* scene_player);

        virtual void shadow_render(LightSource* lightSource);

        virtual void light_render(Camera* scene_player, LightSource* lightSource);
        virtual void light_render(Camera* scene_player);

        virtual void render(Camera* scene_player, GLuint light_texture);
        virtual void render_interface(Camera* scene_player, GLuint light_texture);

        virtual void update();

        void add_object(Object*);

        void add_component(ComponentType);
        void add_component(ComponentType, Component*);

        bool is_collide(Object*);

        Object();

        explicit Object(std::string name);
    };

} // yny

#endif //YENISEY_OBJECT_H
