/* date = December 18th 2020 1:28 pm */

#ifndef TIDE_SDF_H
#define TIDE_SDF_H
#include <stdint.h.>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tide
{
    struct TSDF_FONT
    {
        FT_Face face;
    };
    
    struct TSDF_GLYPH
    {
        float width;
        float height;
        float bearingX;
        float bearingY;
        float advanceX;
        float advanceY;
        unsigned char* buffer;
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
    TSDF_ERROR DeInitTSDF();
    
    /*
Uses FreeType to load a bitmap font and store it in the supplied TSDF_FONT output.
*/
    TSDF_ERROR AddBitmapFont(const char* fontPath, TSDF_FONT* output);
    
    /*
Uses FreeType to load in a FT_Char, and adds it to the TSDF_FONT font.
*/
    TSDF_ERROR AddBitmapChar(TSDF_FONT& font, uint32_t unicodeCodepoint, TSDF_GLYPH* character);
}

#endif //TIDE_SDF_H
