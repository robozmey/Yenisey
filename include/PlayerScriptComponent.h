//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_PLAYERSCRIPTCOMPONENT_H
#define YENISEY_PLAYERSCRIPTCOMPONENT_H

#include "component/ScriptComponent.h"

namespace yny {

    class PlayerScriptComponent : public ScriptComponent {
    public:

        float rotation_speed = 1;
        float movement_speed = 100;

        void update() override;

    };

} // yny

#endif //YENISEY_PLAYERSCRIPTCOMPONENT_H
