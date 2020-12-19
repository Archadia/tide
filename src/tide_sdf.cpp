#include "tide_log.h"
#include "tide_sdf.h"
#include <stb_ds.h>

static FT_Library ftLibrary;

tide::TSDF_ERROR tide::InitTSDF()
{
    return FT_Init_FreeType(&ftLibrary);
}

tide::TSDF_ERROR tide::FreeTSDF()
{
    return FT_Done_FreeType(ftLibrary);
}

tide::TSDF_ERROR tide::FreeFontMetadata(TSDF_FONT* font)
{
    for(int i = 0; i < hmlen(font->glyphs); i++)
    {
        free((void*) font->glyphs[i].value);
    }
    if(font && font->glyphs)
    {
        hmfree(font->glyphs);
        return 0;
    }
    return 1;
}

tide::TSDF_ERROR tide::AddBitmapFont(const char* fontPath, TSDF_FONT* output)
{
    // Create FT_Face
    FT_Face ftFace;
    FT_Error ftError = FT_New_Face(ftLibrary, fontPath, 0, &ftFace);
    if(ftError > 0)
    {
        TIDE_ERROR("Tide SDF Freetype Error: \"%u\" while loading a font", ftError);
        return ftError;
    }
    
    // Form size request.
    FT_Size_RequestRec ftSizeRequest;
    ftSizeRequest.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
    ftSizeRequest.width = 0;
    ftSizeRequest.height = 1024; // fontSize;
    ftSizeRequest.horiResolution = 0;
    ftSizeRequest.vertResolution = 0;
    
    ftError = FT_Request_Size(ftFace, &ftSizeRequest);
    if(ftError > 0)
    {
        TIDE_ERROR("Tide SDF FreeType Error: \"%u\" while requesting font size", ftError);
        return ftError;
    }
    
    output->face =  ftFace;
    output->glyphs = NULL;
    return 0;
}

tide::TSDF_ERROR tide::AddBitmapChar(TSDF_FONT& font, uint32_t unicodeCodepoint)
{
    FT_Error ftError = FT_Load_Char(font.face, unicodeCodepoint, FT_LOAD_DEFAULT);
    if(ftError > 0)
    {
        TIDE_ERROR("Tide SDF FreeType Error: \"%u\" while loading character", ftError);
        return ftError;
    }
    
    FT_Face& face = font.face;
    
    TSDF_GLYPH* character = (TSDF_GLYPH*) malloc(sizeof(TSDF_GLYPH));
    
    character->width = face->glyph->bitmap.width;
    character->height = face->glyph->bitmap.rows;
    character->bearingX = face->glyph->bitmap_left;;
    character->bearingY = face->glyph->bitmap_top;
    character->advanceX = face->glyph->advance.x;
    character->advanceY = face->glyph->advance.y;
    character->buffer = face->glyph->bitmap.buffer;
    
    hmput(font.glyphs, unicodeCodepoint, character);
    return 0;
}