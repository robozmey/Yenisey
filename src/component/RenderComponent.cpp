//
// Created by vladimir on 02.01.23.
//

#include "base/Object.h"
#include "component/RenderComponent.h"
#include "tools/tools.h"
#include "component/TransformComponent.h"
#include "component/MeshComponent.h"

namespace yny {

    Material default_material = Material();

    LightSource default_light_source = LightSource();

    void RenderComponent::write_program_camera_uniforms(Camera* camera) {
        {
            glm::vec3 camera_position = camera->get_camera_position();
            glUniform3fv(camera_position_location, 1, reinterpret_cast<float *>(&camera_position));
            glm::mat4 model = camera->get_camera_model();
            glUniformMatrix4fv(model_location, 1, GL_FALSE, reinterpret_cast<float *>(&model));
            glm::mat4 view = camera->get_camera_view();
            glUniformMatrix4fv(view_location, 1, GL_FALSE, reinterpret_cast<float *>(&view));
            glm::mat4 projection = camera->get_camera_projection();
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, reinterpret_cast<float *>(&projection));

            float screen_height = camera->height;
            glUniform1f(screen_height_location, screen_height);
        }
    }

    void RenderComponent::write_program_object_uniforms() {
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

                glUniform1i(material_has_texture_normal_location, material->has_texture_normal);
                if (material->has_texture_normal) {
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, material->texture_normal);
                    glUniform1i(material_texture_normal_location, 1);
                }
                if (material->has_texture_roughness) {
                    glActiveTexture(GL_TEXTURE2);
                    glBindTexture(GL_TEXTURE_2D, material->texture_roughness);
                    glUniform1i(material_texture_roughness_location, 2);
                } else {
                    glUniform1f(material_roughness_location, material->roughness);
                }

            }
        }
    }

    void RenderComponent::write_program_light_source_uniforms(LightSource* lightSource) {
        { // LightSource
            glUniform1i(light_type_location, lightSource->lightSourceType);
            glUniform1f(light_intensity_location, lightSource->light_intensity);
            glUniform3fv(light_color_location, 1, reinterpret_cast<float *>(&lightSource->color));
            if (lightSource->lightSourceType == DirectionalLight) {
                glUniform3fv(directional_light_direction_location, 1, reinterpret_cast<float *>(&lightSource->direction));
            } else if (lightSource->lightSourceType == SpotLight) {
                glUniform3fv(spot_light_position_location, 1, reinterpret_cast<float *>(&lightSource->position));
                glUniform3fv(spot_light_attenuation_location, 1, reinterpret_cast<float *>(&lightSource->attenuation));
            }
        }
    }

    void RenderComponent::write_light_program_uniforms(Camera *camera, LightSource *LightSource) {
        write_program_camera_uniforms(camera);
        write_program_object_uniforms();
        write_program_light_source_uniforms(LightSource);
    }

    void RenderComponent::write_program_uniforms(Camera *camera, GLuint light_map) {
        write_program_camera_uniforms(camera);
        write_program_object_uniforms();

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, light_map);
        glUniform1i(light_map_location, 2);

    }

    void RenderComponent::light_render(Camera* camera, LightSource* lightSource) {

        MeshComponent* mc = static_cast<MeshComponent *>(componentsObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        getUniformLocations(light_program);
        glUseProgram(light_program);
        write_light_program_uniforms(camera, lightSource);

        glUniform1i(has_texcoord_location, mc->has_texcoord);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    void RenderComponent::light_render(Camera* scene_player) {
        light_render(scene_player, &default_light_source);
    }

    void RenderComponent::render(Camera* camera, GLuint light_map) {

        MeshComponent* mc = static_cast<MeshComponent *>(componentsObject->components[Mesh]);
        std::vector<vertex>& vertices = mc->vertices;
        std::vector<uint32_t>& indices = mc->indices;

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        getUniformLocations(program);
        glUseProgram(program);
        write_program_uniforms(camera, light_map);

        glUniform1i(has_texcoord_location, mc->has_texcoord);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(),  GL_UNSIGNED_INT, nullptr);
    }

    void RenderComponent::getUniformLocations(GLuint program) {
        camera_position_location = glGetUniformLocation(program, "camera_position");

        model_location = glGetUniformLocation(program, "model");
        view_location = glGetUniformLocation(program, "view");
        projection_location = glGetUniformLocation(program, "projection");
        transform_location = glGetUniformLocation(program, "transform");

        has_texcoord_location = glGetUniformLocation(program, "has_texcoord");

        material_type_location = glGetUniformLocation(program, "material_type");
        material_color_location = glGetUniformLocation(program, "material_color");
        material_texture_location = glGetUniformLocation(program, "material_texture");
        material_has_texture_normal_location = glGetUniformLocation(program, "material_has_texture_normal");
        material_texture_normal_location = glGetUniformLocation(program, "material_texture_normal");
        material_has_texture_roughness_location = glGetUniformLocation(program, "material_has_texture_roughness");;
        material_roughness_location = glGetUniformLocation(program, "material_roughness");;
        material_texture_roughness_location = glGetUniformLocation(program, "material_texture_roughness");;

        screen_height_location = glGetUniformLocation(program, "screen_height");
        light_map_location = glGetUniformLocation(program, "light_map");

        light_type_location = glGetUniformLocation(program, "light_type");
        light_color_location = glGetUniformLocation(program, "light_color");
        light_intensity_location = glGetUniformLocation(program, "light_intensity");
        directional_light_direction_location = glGetUniformLocation(program, "directional_light_direction");
        spot_light_position_location = glGetUniformLocation(program, "spot_light_position");
        spot_light_attenuation_location = glGetUniformLocation(program, "spot_light_attenuation");
    }

    void RenderComponent::create_render_component() {

        type = Render;

        auto light_vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto light_fragment_shader = create_shader(GL_FRAGMENT_SHADER, light_fragment_shader_source);
        light_program = create_program(light_vertex_shader, light_fragment_shader);
        getUniformLocations(light_program);

        auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = create_program(vertex_shader, fragment_shader);
        getUniformLocations(program);

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
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, texcoord));
    }

    RenderComponent::RenderComponent() : material(&default_material) {
        create_render_component();
    }

    RenderComponent::RenderComponent(Material *material) : material(material) {
        create_render_component();
    }

} // yny