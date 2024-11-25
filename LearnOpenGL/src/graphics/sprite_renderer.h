#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include <map>


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(Shader& shader, bool isCircle = false);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float alpha = 1.0f,
        glm::vec2 textOffSet = glm::vec2(0.0f), glm::vec2 textRepeat = glm::vec2(0.0f),float rotate = 0.0f, 
        glm::vec3 color = glm::vec3(1.0f), bool useTexture = true);
private:
    // Render state
    Shader                          shader;
    unsigned int                    quadVAO;
    bool                            isCircle;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif