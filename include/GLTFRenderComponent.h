//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_GLTFRENDERCOMPONENT_H
#define YENISEY_GLTFRENDERCOMPONENT_H

#include "component/RenderComponent.h"
#include "GLTFMeshComponent.h"

namespace yny {

    class GLTFRenderComponent : public RenderComponent {
        void render(Camera* camera, LightSource* lightSource) override;
    };

    void add_gltf_model(Object& object, std::string model_path);

} // yny

#endif //YENISEY_GLTFRENDERCOMPONENT_H
