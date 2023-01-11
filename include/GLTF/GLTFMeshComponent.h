//
// Created by vladimir on 07.01.23.
//

#ifndef YENISEY_GLTFMESHCOMPONENT_H
#define YENISEY_GLTFMESHCOMPONENT_H

#include "component/MeshComponent.h"

#include "gltf_loader.hpp"

namespace yny {

    class GLTFMeshComponent : public MeshComponent {
    public:

        struct mesh
        {
            GLuint vao;
            gltf_model::accessor indices;
            gltf_model::material material;
        };

        gltf_model input_model;
        std::vector<mesh> meshes;
        std::map<std::string, GLuint> textures;

        void update_vertices(Camera* scene_player) override;

        GLTFMeshComponent(std::string model_path);
    };

} // yny

#endif //YENISEY_GLTFMESHCOMPONENT_H
