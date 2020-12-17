#ifndef TIDE_TEXT_RENDERING_H
#define TIDE_TEXT_RENDERING_H

#include <stdint.h>
#include <map>

namespace tide
{
    struct FONT_CHARACTER
    {
        uint32_t id;
        float width, height;
        float bearingX, bearingY;
        float advance;
    };
    
    struct FONT_FACE
    {
        const char* name;
        std::map<uint8_t, FONT_CHARACTER*>* characters;
    };
    
    bool LoadFreeType();
    void UnloadFreeType();
    FONT_FACE* LoadFace(const char* font, int fontSize, int index);
    void RenderText(const char* text, float x, float y, float scale);
    void FreeTextRenderer();
}

#endif