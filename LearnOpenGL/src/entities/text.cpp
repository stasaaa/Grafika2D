#include "text.h"

Character::Character()
{
}

Character::Character(unsigned int texture, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance):
	TextureID(texture), Size(size), Bearing(bearing), Advance(advance)
{
}

Character::~Character()
{
}
