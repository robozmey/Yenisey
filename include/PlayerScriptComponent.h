//
// Created by vladimir on 09.01.23.
//

#ifndef YENISEY_PLAYERSCRIPTCOMPONENT_H
#define YENISEY_PLAYERSCRIPTCOMPONENT_H

#include "component/ScriptComponent.h"
#include "interface/InterfaceData.h"
#include "elevation/ElevationDataObject.h"

namespace yny {

    class PlayerScriptComponent : public ScriptComponent {
    public:
        float rotation_speed = 1;
        float movement_speed = 10;
        InterfaceData* interfaceData;
        ElevationDataObject* elevationDataObject;

        void update() override;

        explicit PlayerScriptComponent(InterfaceData* interfaceData, ElevationDataObject* elevationDataObject);
    };

} // yny

#endif //YENISEY_PLAYERSCRIPTCOMPONENT_H
