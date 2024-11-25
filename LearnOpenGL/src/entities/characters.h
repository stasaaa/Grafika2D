#include <map>
#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#ifndef CHARACTERS_H
#define CHARACTERS_H

class Characters {
public:
	std::map<char, Character>	AllCharacters;
	Characters();
	~Characters();
};

#endif
