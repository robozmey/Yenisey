//
// Created by vladimir on 11.01.23.
//

#include "obj/ObjMeshComponent.h"
#include "obj/obj_parser.hpp"

namespace yny {
    ObjMeshComponent::ObjMeshComponent(std::string model_path) {
        obj_data model = parse_obj(model_path);
        for (auto v : model.vertices) {
            vertex v2;
            v2.position = glm::vec3(v.position[0], v.position[1], v.position[2]) * 200.f;
            v2.normal = glm::vec3(v.normal[0], v.normal[1], v.normal[2]);
            v2.texcoord = glm::vec2 (v.texcoord[0], v.texcoord[1]);

            vertices.push_back(v2);
        }
        indices = model.indices;
    }
} // yny