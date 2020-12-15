#include "tide_text_rendering.h"
#include "tide_log.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <glad/glad.h>

static FT_Library library;
static std::vector<tide::FONT_FACE*> fontFaces;

bool tide::LoadFreeType()
{
    int error = FT_Init_FreeType(&library);
    if(error != 0)
    {
        printlnf("%s FreeType Error: %u", TIDE_ERROR_PREFIX, error);
        return false;
    }

    printlnf("%s FreeType successfully loaded", TIDE_LOG_PREFIX);
    return true;
}

void tide::FreeFace()
{
    for(tide::FONT_FACE* face : fontFaces)
    {
        for(auto it = face->characters->begin(); it != face->characters->end(); it++)
        {
            free(it->second); 
        }
        delete face->characters;
        free(face);
    }
    printlnf("%s FreeType successfully destroyed", TIDE_LOG_PREFIX);
}

tide::FONT_FACE* tide::LoadFace(const char* font, int fontSize, int index)
{
    FT_Open_Args args;
    args.flags = FT_OPEN_STREAM;
    args.pathname = (char*) font;

    FT_Face face;
    int error = FT_New_Face(library, font, index, &face);
    if(error != 0)
    {
        printlnf("%s FreeType Error: %u. Unable to load FreeType face", TIDE_ERROR_PREFIX, error);
        return NULL;
    }

    FT_Size_RequestRec sizeRequest;
    sizeRequest.type = FT_SIZE_REQUEST_TYPE_REAL_DIM;
    sizeRequest.width = 0;
    sizeRequest.height = fontSize * 64;
    sizeRequest.horiResolution = 0;
    sizeRequest.vertResolution = 0;

    error = FT_Request_Size(face, &sizeRequest);
    if(error != 0)
    {
        printlnf("%s FreeType Error: %u. Unable to request size", TIDE_ERROR_PREFIX, error);
        return NULL;
    }

    tide::FONT_FACE* fontFace = (tide::FONT_FACE*) malloc(sizeof(tide::FONT_FACE));
    fontFace->characters = new std::map<uint8_t, tide::FONT_CHARACTER*>();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(uint8_t c = 0; c < 1; c++)
    {
        error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error != 0)
        {
            printlnf("%s FreeType Error: %u. Unable to load FreeType glyph\n", TIDE_ERROR_PREFIX, error);
            continue;
        }
        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        tide::FONT_CHARACTER* character = (tide::FONT_CHARACTER*) malloc(sizeof(tide::FONT_CHARACTER));
        character->id = texture;
        character->width = face->glyph->bitmap.width;
        character->height = face->glyph->bitmap.rows;
        character->bearingX = face->glyph->bitmap_left;
        character->bearingY = face->glyph->bitmap_top;
        character->advance = face->glyph->advance.x;
        
        fontFace->characters->insert(std::map<uint8_t, tide::FONT_CHARACTER*>::value_type(c, character));
    }
    fontFaces.push_back(fontFace);
    return fontFace;
}

void tide::RenderText(const char* text, float x, float y, float scale)
{

}