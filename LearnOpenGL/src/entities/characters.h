#include <map>
#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#ifndef CHARACTERS_H
#define CHARACTERS_H

class Characters {
public:
	std::map<char32_t, Character>	AllCharacters;
	Characters();
	~Characters();
};

#endif
