//
// Created by vladimir on 11.01.23.
//

#ifndef YENISEY_INTERFACERENDERCOMPONENT_H
#define YENISEY_INTERFACERENDERCOMPONENT_H

#include "component/RenderComponent.h"
#include "InterfaceData.h"

namespace yny {

    const char msdf_vertex_shader_source[] =
            R"(#version 330 core

    uniform mat4 transform;

    layout (location = 0) in vec2 in_position;
    layout (location = 1) in vec2 in_texcoord;

    out vec2 texcoord;

    void main()
    {
        gl_Position = transform * vec4(in_position, 0.0, 1.0);
        texcoord = in_texcoord;
    }
    )";

    const char msdf_fragment_shader_source[] =
                R"(#version 330 core

    layout (location = 0) out vec4 out_color;

    uniform float sdf_scale;

    uniform sampler2D sdf_texture;

    in vec2 texcoord;

    float median(vec3 v) {
        return max(min(v.r, v.g), min(max(v.r, v.g), v.b));
    }

    void main()
    {
        vec3 font_color = vec3(1, 1, 1);
        float sdf_texture_value = median(texture(sdf_texture, texcoord).rgb);
        float sdf_value = sdf_scale * (sdf_texture_value - 0.5);
        float smopth_const = length(vec2(dFdx(sdf_value), dFdy(sdf_value)))/sqrt(2.0);
        float alpha = smoothstep(-smopth_const, smopth_const, sdf_value);

        float ob_sdf_value = sdf_scale * (sdf_texture_value - 0.3);
        float ob_smopth_const = length(vec2(dFdx(ob_sdf_value), dFdy(ob_sdf_value)))/sqrt(2.0);
        float ob_alpha = smoothstep(-ob_smopth_const, ob_smopth_const, ob_sdf_value);

        font_color = mix(vec3(0, 0, 0), font_color, alpha);

        out_color = vec4(font_color, ob_alpha);
//        out_color = vec4(vec3(texcoord, 0), 1);
    }
    )";

    struct msdf_vertex {
        glm::vec2 position;
        glm::vec2 texcoord;
    };

    class InterfaceRenderComponent : public RenderComponent {
    public:
        InterfaceData* interfaceData;

        std::vector<glm::vec2> bbox;

        std::vector<msdf_vertex> vertexes;
        GLuint text_vao, text_vbo;

        GLuint texture;
        int texture_width1, texture_height1;
        float texture_width, texture_height;

        GLuint msdf_program;
        GLuint transform_location;
        GLuint sdf_scale_location;
        GLuint sdf_texture_location;

        virtual void light_render(Camera* camera, LightSource* lightSource) override;
        virtual void light_render(Camera* camera) override;

        void render(Camera* camera, GLuint light_map) override;

        InterfaceRenderComponent(InterfaceData* interfaceData);
    };

} // yny

#endif //YENISEY_INTERFACERENDERCOMPONENT_H
