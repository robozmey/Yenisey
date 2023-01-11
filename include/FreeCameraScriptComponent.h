//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_FREECAMERASCRIPTCOMPONENT_H
#define YENISEY_FREECAMERASCRIPTCOMPONENT_H

#include "component/ScriptComponent.h"

namespace yny {

    class FreeCameraScriptComponent : public ScriptComponent {
    public:

        float rotation_speed = 1;
        float movement_speed = 100;

        void update() override;

    };

} // yny

#endif //YENISEY_FREECAMERASCRIPTCOMPONENT_H
