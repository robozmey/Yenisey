//
// Created by vladimir on 05.01.23.
//

#include "Material.h"
#include "tools.h"

namespace yny {

    void Material::create_one_color(glm::vec4) {
        materialType = OneColorMaterial;
    }

    Material::Material() {
        create_one_color({0.5, 0.5, 0.5, 1});
    }

    Material::Material(glm::vec3 color3) {
        create_one_color({color3, 1});
    }

    Material::Material(glm::vec4 color4) {
        create_one_color(color4);
    }

    Material::Material(std::string texture_path) {
        materialType = TextureMaterial;
        texture = load_texture(texture_path);
    }
} // yny