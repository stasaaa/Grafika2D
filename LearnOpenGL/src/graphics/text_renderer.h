#include "shader.h"
#include "../entities/characters.h"
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

class TextRenderer {
public:
    TextRenderer();
    // Constructor (inits shaders/shapes)
    TextRenderer(Shader& shader, Characters& characters);
    // Destructor
    ~TextRenderer();
    // Renders a defined quad textured with given sprite
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
    // Render state
    Shader                          shader;
    unsigned int                    VAO, VBO;
    Characters                      CharactersDetails;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif