//
// Created by vladimir on 05.01.23.
//

#include "base/Material.h"
#include "tools/tools.h"

namespace yny {

    void Material::create_one_color(glm::vec4 material_color) {
        color = material_color;
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

    Material::Material(std::string texture_path) : texture_path(texture_path) {
        materialType = TextureMaterial;
        texture = load_texture(texture_path);
    }

    void Material::add_normal_map(std::string texture_path) {
        has_texture_normal = true;
        texture_normal_path = texture_path;
        texture_normal = load_texture(texture_path);
    }

    void Material::add_roughness_map(std::string texture_path) {
        has_texture_roughness = true;
        texture_roughness_path = texture_path;
        texture_roughness = load_texture(texture_path);
    }
} // yny