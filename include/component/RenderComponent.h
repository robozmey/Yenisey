//
// Created by vladimir on 02.01.23.
//

#ifndef YENISEY_RENDERCOMPONENT_H
#define YENISEY_RENDERCOMPONENT_H

#include <vector>
#include "Component.h"
#include "base/Camera.h"
#include "base/Material.h"
#include "base/LightSource.h"

namespace yny {

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

    out vec3 glposition;

    void main()
    {
        position = (transform * vec4(in_position, 1.0)).rgb;
        gl_Position = projection * view * model * transform * vec4(in_position, 1.0);
        glposition = gl_Position.xyz;
        tangent = mat3(model) * in_tangent;
        normal = mat3(model) * in_normal;
        texcoord = in_texcoord;
    }
    )";

    const char shadow_vertex_shader_source[] =
            R"(#version 330 core

    uniform mat4 model;
    uniform mat4 transform;
    uniform mat4 shadow_transform;

    layout (location = 0) in vec3 in_position;

    void main()
    {
        gl_Position = shadow_transform * transform * vec4(in_position, 1.0);
    }
    )";

    const char shadow_fragment_shader_source[] =
            R"(#version 330 core

    layout (location = 0) out vec4 out_coords;

    void main()
    {
        float z = gl_FragCoord.z;
        float bias = 1 / 4 * (dFdx (z) * dFdx (z) + dFdy (z) * dFdy (z));

        out_coords = vec4(z, z * z + bias, 0, 0);
    }
    )";

    const char light_fragment_shader_source[] =
            R"(#version 330 core

    uniform vec3 camera_position;
    uniform mat4 transform;

    uniform mat4 shadow_transform;

    uniform int has_texcoord;

    // Material

    int one_color_material_type = 0;
    int texture_material_type = 1;

    uniform int material_type;
    uniform vec4 material_color;
    uniform sampler2D material_texture;

    uniform bool material_has_texture_normal;
    uniform sampler2D material_texture_normal;

    uniform bool material_has_texture_glossiness;
    uniform float material_glossiness;
    uniform sampler2D material_texture_glossiness;

    uniform bool material_has_texture_ambient_occlusion;
    uniform sampler2D material_texture_ambient_occlusion;

    uniform float power;


    // LightSource

    int ambient_light_type = 0;
    int directional_light_type = 1;
    int spot_light_type = 2;

    uniform int light_type;
    uniform vec3 light_color;
    uniform float light_intensity;
    uniform sampler2D shadow_map;
    uniform vec3 directional_light_direction;
    uniform vec3 spot_light_position;
    uniform vec3 spot_light_attenuation;


    // Mist

    uniform vec3 mist_color;

    in vec3 position;
    in vec3 normal;
    in vec3 tangent;
    in vec2 texcoord;

    in vec3 glposition;

    layout (location = 0) out vec4 out_color;


    // Global
    float real_glossiness = 0;

    vec3 diffuse(vec3 direction, vec3 real_normal, vec3 albedo) {
        return albedo * max(0.0, dot(real_normal, direction));
    }

    vec3 specular(vec3 direction, vec3 real_normal, vec3 albedo) {
        vec3 reflected_direction = 2.0 * normal * dot(real_normal, direction) - direction;
        vec3 view_direction = normalize(camera_position - position);
        return real_glossiness * albedo * pow(max(0.0, dot(reflected_direction, view_direction)), power);

    }

    vec3 phong(vec3 direction, vec3 real_normal, vec3 albedo) {
        return diffuse(direction, real_normal, albedo) + specular(direction, real_normal, albedo);
    }

    void main()
    {
        vec2 real_texcoord = position.xz / 1000;
        if (has_texcoord == 1) {
            real_texcoord = texcoord;
        }

        vec3 real_normal = (transform * vec4(normal, 0)).xyz;

        if (material_has_texture_normal) {
            vec3 bitangent = cross(tangent, real_normal);
            mat3 tbn = mat3(tangent, bitangent, real_normal);
            real_normal = tbn * (texture(material_texture_normal, real_texcoord).xyz * 2.0 - vec3(1.0));
        }

        real_glossiness = material_glossiness;
        if (material_has_texture_glossiness) {
            real_glossiness = texture(material_texture_glossiness, real_texcoord).r;
        }

        if (light_type == ambient_light_type) {
            float real_light_intensity = light_intensity;
            if (material_has_texture_ambient_occlusion) {
                real_light_intensity = texture(material_texture_ambient_occlusion, real_texcoord).r * light_intensity;
            }
            out_color = vec4(light_color * real_light_intensity, 1);
        } else {
            vec3 light_direction;
            float real_light_intensity = light_intensity;

            if (light_type == directional_light_type) {
                light_direction = directional_light_direction;

                vec4 shadow_pos = shadow_transform * vec4(position, 1.0);
                shadow_pos /= shadow_pos.w;
                shadow_pos = shadow_pos * 0.5 + vec4(0.5);

                vec2 sum = vec2(0.0);
                float sum_w = 0.0;
                const int N = 3;
                float radius = 3.0;

                for (int x = -N; x <= N; ++x) {
                    for (int y = -N; y <= N; ++y) {
                        float c = exp(-float(x*x + y*y) / (radius*radius));
                        sum += c * texture(shadow_map, shadow_pos.xy + vec2(x,y) / vec2(textureSize(shadow_map, 0))).rg;;
                        sum_w += c;
                    }
                }
                vec2 data = sum / sum_w;

                float mu = data.r;
                float sigma = data.g - mu * mu;
                float z = shadow_pos.z - 0.001;
                float factor = (z < mu) ? 1.0 : sigma / (sigma + (z - mu) * (z - mu));

                float delta = 0.125;
                if (factor < delta) {
                    factor = 0;
                }
                else {
                    factor = (factor - delta) / (1 - delta);
                }
                real_light_intensity = light_intensity * factor;

            } else if (light_type == spot_light_type) {
                light_direction = normalize(spot_light_position - position);
                float dist = distance(position, spot_light_position);
                real_light_intensity = 1 / (spot_light_attenuation.x + spot_light_attenuation.y * dist + spot_light_attenuation.z * dist * dist) * light_intensity;
            }
            vec3 color = phong(light_direction, real_normal, light_color) * real_light_intensity;

            out_color = vec4(color, 1);
        }

    }
    )";

    const char fragment_shader_source[] =
            R"(#version 330 core

    uniform float gamma;

    uniform vec3 camera_position;
    uniform mat4 transform;

    uniform int has_texcoord;

    // Material

    int one_color_material_type = 0;
    int texture_material_type = 1;

    uniform int material_type;
    uniform vec4 material_color;
    uniform sampler2D material_texture;

    uniform bool material_has_texture_normal;
    uniform sampler2D material_texture_normal;


    // LightSource

    int ambient_light_type = 0;
    int directional_light_type = 1;
    int spot_light_type = 2;

    uniform int light_type;
    uniform vec3 light_color;
    uniform float light_intensity;
    uniform vec3 directional_light_direction;
    uniform vec3 spot_light_position;


    uniform sampler2D light_map;
    uniform float screen_height;


    // Mist

    uniform vec3 mist_color;

    in vec3 position;
    in vec3 normal;
    in vec3 tangent;
    in vec2 texcoord;

    in vec3 glposition;

    layout (location = 0) out vec4 out_color;
//    layout(pixel_center_integer) in vec4 gl_FragCoord;


    void main()
    {
        vec2 real_texcoord = position.xz / 1000;
        if (has_texcoord == 1) {
            real_texcoord = texcoord;
        }

        vec3 albedo;
        if (material_type == one_color_material_type) {
            albedo = material_color.rgb;
        } else if (material_type == texture_material_type) {
            albedo = texture(material_texture, real_texcoord).rgb;
        }

//        albedo = vec3(1, 1, 1);

        vec2 light_texcoord = gl_FragCoord.xy / 2048;
        vec3 color = albedo * texture(light_map, light_texcoord).rgb;
        out_color = vec4(color, 1);

//        // Mist
//        float optical_depth = distance(position, camera_position) / 1000;
//
//        optical_depth = min(1, max(0, optical_depth));

    //    out_color = vec4(mist_color, 1) * optical_depth + out_color * (1 - optical_depth);

    //    out_color = vec4(texture(material_texture_normal, real_texcoord).xyz, 1);
    //    out_color = vec4(vec3(material_has_texture_normal), 1);

//        if (position.y < 260) {
//            out_color = vec4(out_color.rgb * vec3(0.7, 0.7, 1), out_color.a);
//        }


//        out_color = vec4(pow(abs(out_color.rgb), vec3(gamma)), out_color.a);

    }
    )";

    class RenderComponent : public Component {
    public:
        ComponentType type = Render;

        bool is_interface = false;

        GLuint model_location;
        GLuint view_location;
        GLuint projection_location;
        GLuint transform_location;

        GLuint vao, vbo, ebo;
        GLuint shadow_program;
        GLuint light_program;
        GLuint program;

        GLuint camera_position_location;

        GLuint has_texcoord_location;

        GLuint gamma_location;

        Material* material;
        GLuint material_type_location;
        GLuint material_color_location;
        GLuint material_texture_location;
        GLuint material_has_texture_normal_location;
        GLuint material_texture_normal_location;
        GLuint material_has_texture_roughness_location;
        GLuint material_roughness_location;
        GLuint material_texture_roughness_location;
        GLuint material_has_texture_ambient_occlusion_location;
        GLuint material_texture_ambient_occlusion_location;

        GLuint screen_height_location;
        GLuint light_map_location;
        GLuint shadow_map_location;
        GLuint shadow_transform_location;

        GLuint light_type_location;
        GLuint light_color_location;
        GLuint light_intensity_location;
        GLuint directional_light_direction_location;
        GLuint spot_light_position_location;
        GLuint spot_light_attenuation_location;

        virtual void shadow_render(LightSource* lightSource);

        virtual void light_render(Camera* camera, LightSource* lightSource);
        virtual void light_render(Camera* camera);

        virtual void render(Camera* camera, GLuint light_map);

        RenderComponent();

        explicit RenderComponent(Material* material);

    private:
        void create_render_component();

    protected:
        void write_program_camera_uniforms(Camera* camera);
        void write_program_object_uniforms();
        void write_program_light_source_uniforms(LightSource* lightSource);

        void write_shadow_program_uniforms(LightSource *LightSource);
        void write_light_program_uniforms(Camera* camera, LightSource* lightSource);
        void write_program_uniforms(Camera *camera, GLuint light_map);

        void getUniformLocations(GLuint program);

        int write_in_buffer();
    };

} // yny

#endif //YENISEY_RENDERCOMPONENT_H
