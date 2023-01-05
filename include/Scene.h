//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_SCENE_H
#define YENISEY_SCENE_H

#include <vector>

#include "Object.h"
#include "Player.h"
#include "Skybox.h"

namespace yny {

    class Scene : public Object {
    public:
        Player player;
        Skybox skybox;

        void update_vertices();

        void render();

        void update_time();

        Scene();

        explicit Scene(std::string);
    };

} // yny

#endif //YENISEY_SCENE_H
