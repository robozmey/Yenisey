//
// Created by vladimir on 28.12.22.
//

#ifndef YENISEY_OBJECT_H
#define YENISEY_OBJECT_H

#include "Feature.h"
#include "Player.h"

#include <vector>

namespace yny {

    class Object {
    public:
        std::vector<Feature> features;

        virtual void render(Player& scene_player);
    };

} // yny

#endif //YENISEY_OBJECT_H
