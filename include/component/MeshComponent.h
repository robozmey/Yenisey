//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_MESHCOMPONENT_H
#define YENISEY_MESHCOMPONENT_H

#include <vector>
#include "Component.h"
#include "base/Camera.h"

namespace yny {
    class MeshComponent : public Component{
    public:

        std::vector<vertex> vertices;
        std::vector<uint32_t> indices;

        int has_texcoord = 1;

        bool has_vbo = false;
        GLuint vbo;

        virtual void update_vertices(Camera* scene_player);

        void apply_transform();

        virtual std::vector<glm::vec2> get_bounding_box();

        MeshComponent();
    };
}


#endif //YENISEY_MESHCOMPONENT_H
