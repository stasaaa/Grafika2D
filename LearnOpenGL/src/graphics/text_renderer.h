#include "shader.h"
#include "../entities/characters.h"
#include <string>
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

class TextRenderer {
public:
    TextRenderer();
    TextRenderer(Shader& shader, Characters& characters);
    ~TextRenderer();
    void RenderText(std::wstring text, float x, float y, float scale, glm::vec3 color);
    Shader                          shader;
    unsigned int                    VAO, VBO;
    Characters                      CharactersDetails;
    void initRenderData();
};

#endif