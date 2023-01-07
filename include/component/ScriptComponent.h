//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_SCRIPTCOMPONENT_H
#define YENISEY_SCRIPTCOMPONENT_H

#include "component/Component.h"

namespace yny {

    class ScriptComponent : public Component {
    public:
        virtual void start();
        virtual void update();
    };

} // yny

#endif //YENISEY_SCRIPTCOMPONENT_H
