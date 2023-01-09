//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_MESHCOMPONENT_H
#define YENISEY_MESHCOMPONENT_H

#include <vector>
#include "Component.h"
#include "Camera.h"

namespace yny {
    class MeshComponent : public Component{
    public:

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        bool has_vbo = false;
        GLuint vbo;

        virtual void update_vertices(Camera* scene_player);

        void apply_transform();

        MeshComponent();
    };
}


#endif //YENISEY_MESHCOMPONENT_H
