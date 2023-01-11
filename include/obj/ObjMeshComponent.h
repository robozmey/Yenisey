//
// Created by vladimir on 11.01.23.
//

#ifndef YENISEY_OBJMESHCOMPONENT_H
#define YENISEY_OBJMESHCOMPONENT_H

#include "component/MeshComponent.h"

namespace yny {

    class ObjMeshComponent : public MeshComponent {
    public:
        explicit ObjMeshComponent(std::string model_path);
    };

} // yny

#endif //YENISEY_OBJMESHCOMPONENT_H
