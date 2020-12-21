/* date = December 18th 2020 1:28 pm */

#ifndef TIDE_SDF_H
#define TIDE_SDF_H
#include "tide_gl.h"

#include <stdint.h.>
#include <ft2build.h>
#include FT_FREETYPE_H

struct TSDF_GLYPH
{
    float width;
    float height;
    float bearingX;
    float bearingY;
    float advanceX;
    float advanceY;
    unsigned char *buffer;
};

struct TSDF_KEY_MAP
{
    char key;
    TSDF_GLYPH *value;
};

struct TSDF_FONT
{
    FT_Face face;
    COMPILATION_TEXTURE *compilation;
    ARRAY_TEXTURE *texture;
    TSDF_KEY_MAP *glyphs;
};

/*
 * Success on 0,
* Fail on 1+.
*/
typedef uint8_t TSDF_ERROR;

/*
Initialises FreeType & other factors.
*/
TSDF_ERROR InitTSDF();

/*
Uninitialises FreeType & frees allocated memory.
*/
TSDF_ERROR FreeTSDF();

/*
Frees additional meta such as hashmaps between character and glyph.
*/
TSDF_ERROR FreeFontMetadata(TSDF_FONT *font);

/*
Uses FreeType to load a bitmap font and store it in the supplied TSDF_FONT output.
*/
TSDF_ERROR AddBitmapFont(const char *fontPath, TSDF_FONT *output);

/*
Uses FreeType to load in a FT_Char, and adds it to the TSDF_FONT font.
*/
TSDF_ERROR AddBitmapChar(TSDF_FONT &font, uint32_t unicodeCodepoint);

void CompileBitmapTexture(TSDF_FONT &font);

#endif //TIDE_SDF_H
