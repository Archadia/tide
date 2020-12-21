#include "tide_log.h"
#include "tide_sdf.h"
#include <stb_ds.h>
#include <glad/glad.h>

static FT_Library ftLibrary;

TSDF_ERROR InitTSDF()
{
    return FT_Init_FreeType(&ftLibrary);
}

TSDF_ERROR FreeTSDF()
{
    return FT_Done_FreeType(ftLibrary);
}

TSDF_ERROR FreeFontMetadata(TSDF_FONT* font)
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
    // Deleting the textures here on the GPU since
    // I can't think of a time where this would be done
    // otherwise...
    glDeleteTextures(1, &font->texture->id);
    // Deleting the compilation and texture structures too.
    free((void*) font->compilation);
    free((void*) font->texture);
    return 1;
}

TSDF_ERROR AddBitmapFont(const char* fontPath, TSDF_FONT* output)
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
    output->compilation = (COMPILATION_TEXTURE*) malloc(sizeof(COMPILATION_TEXTURE));
    output->texture = (ARRAY_TEXTURE*) malloc(sizeof(ARRAY_TEXTURE));
    CreateArrayTexture(ftSizeRequest.height / 64, ftSizeRequest.height / 64, output->compilation);
    output->glyphs = NULL;
    return 0;
}

TSDF_ERROR AddBitmapChar(TSDF_FONT& font, uint32_t unicodeCodepoint)
{
    FT_Error ftError = FT_Load_Char(font.face, unicodeCodepoint, FT_LOAD_RENDER);
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
    
    //TIDE_LOG("width: %d", character->height);
    int len = sizeof(character->buffer)/sizeof(character->buffer[0]);
    //TIDE_LOG("len: %d", len);
    
    hmput(font.glyphs, unicodeCodepoint, character);
    
    AddToArrayTexture(character->buffer, font.compilation);
    return 0;
}

void CompileBitmapTexture(TSDF_FONT& font)
{
    // Knowing this deletes the internal buffer of the COMPILATION_TEXTURE
    CompileArrayTexture(*font.compilation, GL_RED, font.texture);
    free((void*) font.compilation);
}