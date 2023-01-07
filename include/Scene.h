//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_SCENE_H
#define YENISEY_SCENE_H

#include <vector>

#include "Object.h"
#include "Camera.h"
#include "Skybox.h"
#include "Input.h"

namespace yny {

    class Scene : public Object {
    public:
        Camera* sceneCamera;
        Skybox skybox;

        Input input;

        float time;
        float dt;

        void update_vertices();

        void render();

        void update();

        Scene();

        explicit Scene(std::string);
    };

} // yny

#endif //YENISEY_SCENE_H
