#include "text_renderer.h"
#include "../entities/text.h"
#include <iostream>

TextRenderer::TextRenderer()
{
}

TextRenderer::TextRenderer(Shader& shader, Characters& characters)
{
	this->shader = shader;
    this->CharactersDetails = characters;
	initRenderData();
}

TextRenderer::~TextRenderer()
{
	glDeleteVertexArrays(1, &this->VAO);
}

void TextRenderer::RenderText(std::wstring text, float x, float y, float scale, glm::vec3 color)
{
    this->shader.Use().SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    for (auto c = text.begin(); c != text.end(); c++)
    {
        unsigned int unicodeChar = static_cast<unsigned int>(*c);
        Character ch = this->CharactersDetails.AllCharacters[unicodeChar];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - ch.Bearing.y * scale;

        float w = ch.Size.x* scale;
        float h = ch.Size.y * scale;
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::initRenderData()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
