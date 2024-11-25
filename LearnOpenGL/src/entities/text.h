#ifndef TEXT_H
#define TEXT_H

#include <glm/glm.hpp>

class Character {
public:
    Character();
    Character(unsigned int texture, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);
    ~Character();
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

#endif