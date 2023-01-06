//
// Created by vladimir on 02.01.23.
//

#include "Object.h"
#include "component/RenderComponent.h"
#include "tools.h"
#include "component/TransformComponent.h"
#include "component/MeshComponent.h"

const char vertex_shader_source[] =
        R"(#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec2 in_texcoord;

out vec3 position;
out vec3 normal;
out vec3 tangent;
out vec2 texcoord;

void main()
{
    position = in_position;
    gl_Position = projection * view * model * transform * vec4(in_position, 1.0);
    tangent = mat3(model) * in_tangent;
    normal = mat3(model) * in_normal;
}
)";

const char fragment_shader_source[] =
        R"(#version 330 core

uniform vec3 camera_position;

// Material

int one_color_material_type = 0;
int texture_material_type = 1;

uniform int material_type;
uniform vec4 material_color;
uniform sampler2D material_texture;

uniform bool has_normal_map;
uniform sampler2D normal_texture;


// LightSource

int ambient_light_type = 0;
int directional_light_type = 1;
int spot_light_type = 2;

uniform int light_type;
uniform vec3 light_color;
uniform float light_intensity;
uniform vec3 directional_light_direction;
uniform vec3 spot_light_position;


// Mist

uniform vec3 mist_color;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

layout (location = 0) out vec4 out_color;


void main()
{
    vec2 real_texcoord = position.xz / 100;

    vec3 albedo = vec3(0.1, 0, 0.2);
    if (material_type == one_color_material_type) {
        albedo = material_color.rgb;
    } else if (material_type == texture_material_type) {
        albedo = texture(material_texture, real_texcoord).rgb;
    }

    vec3 real_normal = normal;

    if (has_normal_map) {
        vec3 bitangent = cross(tangent, normal);
        mat3 tbn = mat3(tangent, bitangent, normal);
        real_normal = tbn * (texture(normal_texture, real_texcoord).xyz * 2.0 - vec3(1.0));
    }

    if (light_type == ambient_light_type) {
        out_color = vec4(albedo * light_intensity, 1);
    } else {
        vec3 light_direction;
        if (light_type == directional_light_type) {
            light_direction = directional_light_direction;
        } else if (light_type == spot_light_type) {
            light_direction = normalize(position - spot_light_position);
        }
        float diffuse = max(0.0, dot(normalize(normal), light_direction));

        vec3 color = albedo * (diffuse);

        out_color = vec4(color, 1);
    }

    // Mist
    float optical_depth = distance(position, camera_position) / 1000;

    optical_depth = min(1, max(0, optical_depth));

//    out_color = vec4(mist_color, 1) * optical_depth + out_color * (1 - optical_depth);

}
)";

namespace yny {

    Material default_material = Material();

    LightSource default_light_source = LightSource();

    void RenderComponent::render(Camera* camera, LightSource* lightSource) {

        MeshComponent* mc = static_cast<MeshComponent *>(componentsObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glUseProgram(program);
        {
            glm::vec3 camera_position = camera->get_camera_position();
            glUniform3fv(camera_position_location, 1, reinterpret_cast<float *>(&camera_position));
            glm::mat4 model = camera->get_camera_model();
            glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&model));
            glm::mat4 view = camera->get_camera_view();
            glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&view));
            glm::mat4 projection = camera->get_camera_projection();
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&projection));
        }

        glm::mat4 transform;
        {
            TransformComponent* tc = reinterpret_cast<TransformComponent *>(componentsObject->components[Transform]);
            transform = tc->get_transform();
        }
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, reinterpret_cast<float *>(&transform));

        {   // Material
            glUniform1i(material_type_location, material->materialType);
            if (material->materialType == OneColorMaterial) {
                glUniform4fv(material_color_location, 1, reinterpret_cast<float *>(&material->color));
            } else if (material->materialType == TextureMaterial) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material->texture);
                glUniform1i(material_texture_location, 0);
            }
        }

        { // LightSource
            glUniform1i(light_type_location, lightSource->lightSourceType);
            glUniform1f(light_intensity_location, lightSource->light_intensity);
            if (lightSource->lightSourceType == DirectionalLight) {
                glUniform3fv(directional_light_direction_location, 1, reinterpret_cast<float *>(&lightSource->direction));
            } else if (lightSource->lightSourceType == SpotLight) {
                glUniform3fv(directional_light_direction_location, 1, reinterpret_cast<float *>(&lightSource->position));
            }
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    void RenderComponent::render(Camera* scene_player) {
        render(scene_player, &default_light_source);
    }

    void RenderComponent::create_render_component() {

        type = Render;

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);

        camera_position_location = glGetUniformLocation(program, "camera_position");

        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");
        transform_location = glGetUniformLocation(program, "transform");

        material_type_location = glGetUniformLocation(program, "material_type");
        material_color_location = glGetUniformLocation(program, "material_color");
        material_texture_location = glGetUniformLocation(program, "material_texture");

        light_type_location = glGetUniformLocation(program, "light_type");
        light_color_location = glGetUniformLocation(program, "light_color");
        light_intensity_location = glGetUniformLocation(program, "light_intensity");
        directional_light_direction_location = glGetUniformLocation(program, "directional_light_direction");
        spot_light_position_location = glGetUniformLocation(program, "spot_light_position");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, tangent));
    }

    RenderComponent::RenderComponent() : material(&default_material) {
        create_render_component();
    }

    RenderComponent::RenderComponent(Material *material) : material(material) {
        create_render_component();
    }

} // yny