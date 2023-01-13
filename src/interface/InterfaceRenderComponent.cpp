//
// Created by vladimir on 11.01.23.
//

#include "interface/InterfaceRenderComponent.h"
#include "tools/stb_image.h"
#include "tools/tools.h"


namespace yny {
    void InterfaceRenderComponent::render(Camera* camera, GLuint light_map) {

        glm::mat4 transform(0);
        transform[0][0] = 2.f / camera->width;
        transform[1][1] = -2.f / camera->height;
        transform[2][2] = 1;
        transform[3][3] = 1;
        transform = glm::translate(transform, {-camera->width / 2.f, -camera->height / 2.f, 0});

        if (interfaceData->text_changed) {
            glm::vec3 pen(0.0);
            vertexes.clear();
            for (auto letter : interfaceData->text) {
                auto glyph = interfaceData->font.glyphs.at(letter);
                vertexes.insert(vertexes.end(),{
                        {
                                {glyph.xoffset + pen.x, glyph.yoffset + pen.y},
                                {glyph.x / texture_width, glyph.y / texture_width}
                        }, {
                                {glyph.xoffset + glyph.width + pen.x, glyph.yoffset + pen.y},
                                {(glyph.x + glyph.width) / texture_width, glyph.y / texture_height}
                        }, {
                                {glyph.xoffset + pen.x, glyph.yoffset + glyph.height + pen.y},
                                {glyph.x / texture_width, (glyph.y + glyph.height) / texture_height}
                        }, {
                                {glyph.xoffset + glyph.width + pen.x, glyph.yoffset + pen.y},
                                {(glyph.x + glyph.width) / texture_width, glyph.y / texture_height}
                        }, {
                                {glyph.xoffset + pen.x, glyph.yoffset + glyph.height + pen.y},
                                {glyph.x / texture_width, (glyph.y + glyph.height) / texture_height}
                        }, {
                                {glyph.xoffset + glyph.width + pen.x, glyph.yoffset + glyph.height + pen.y},
                                {(glyph.x + glyph.width) / texture_width, (glyph.y + glyph.height) / texture_height}
                        }
                });
                pen.x += glyph.advance;
            }
            if (!vertexes.empty()) {
                bbox[0].x = vertexes[0].position.x;
                bbox[0].y = vertexes[0].position.y;
                bbox[1].x = vertexes[0].position.x;
                bbox[1].y = vertexes[0].position.y;
            }
            for (auto v : vertexes) {
                bbox[0].x = fmin(bbox[0].x, v.position.x);
                bbox[0].y = fmin(bbox[0].y, v.position.y);
                bbox[1].x = fmax(bbox[1].x, v.position.x);
                bbox[1].y = fmax(bbox[1].y, v.position.y);
            }

            glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
            glBufferData(GL_ARRAY_BUFFER,  sizeof(vertex)*vertexes.size(), vertexes.data(), GL_DYNAMIC_DRAW);
            interfaceData->text_changed = false;

        }

        glm::vec2 center = glm::vec2((bbox[0].x + bbox[1].x) / 2, (bbox[0].y + bbox[1].y) / 2);
        float scale = 1;
        transform = glm::translate(transform, {camera->width * 0.01f, camera->height * 0.01f, 0});
        transform = glm::scale(transform, {scale, scale, 1});

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(msdf_program);
        glUniform1f(sdf_texture_location, 0);
        glUniform1f(sdf_scale_location, interfaceData->font.sdf_scale);
        glUniformMatrix4fv(transform_location, 1, GL_FALSE,
                           reinterpret_cast<float *>(&transform));

        glBindVertexArray(text_vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

        glDisable(GL_BLEND);
    }

    InterfaceRenderComponent::InterfaceRenderComponent(InterfaceData* interfaceData) : interfaceData(interfaceData) {
        type = Render;

        is_interface = true;

        bbox.resize(2);

        int channels;
        auto data = stbi_load(interfaceData->font.texture_path.c_str(), &texture_width1, &texture_height1, &channels, 4);
        assert(data);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width1, texture_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        texture_width = texture_width1;
        texture_height = texture_height1;

        glGenVertexArrays(1, &text_vao);
        glGenBuffers(1, &text_vbo);

        glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(msdf_vertex), (void *)offsetof(msdf_vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(msdf_vertex), (void *)offsetof(msdf_vertex, texcoord));

        auto msdf_vertex_shader = create_shader(GL_VERTEX_SHADER, msdf_vertex_shader_source);
        auto msdf_fragment_shader = create_shader(GL_FRAGMENT_SHADER, msdf_fragment_shader_source);
        msdf_program = create_program(msdf_vertex_shader, msdf_fragment_shader);

        transform_location = glGetUniformLocation(msdf_program, "transform");
        sdf_scale_location = glGetUniformLocation(msdf_program, "sdf_scale");
        sdf_texture_location = glGetUniformLocation(msdf_program, "sdf_texture");
    }

    void InterfaceRenderComponent::light_render(Camera *camera, LightSource *lightSource) {}

    void InterfaceRenderComponent::light_render(Camera *camera) {}
} // yny