//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_SCENE_H
#define YENISEY_SCENE_H

#include <vector>

#include "Object.h"
#include "Player.h"

namespace yny {

    class Scene {
    public:
        std::vector<Object*> objects;
        Player player;

        void render();
    };

} // yny

#endif //YENISEY_SCENE_H
