#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_log.h"
#include "tide_gl.h"
#include "tide_display.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stb_ds.h>

#include <vector>
#include <glad/glad.h>

static FT_Library library;
static tide::FONT_FACE** fontFaces;

static tide::VAO* quadVAO;
static tide::SHADER_PROGRAM* program;

static uint32_t locTransformation;
static uint32_t locOrthographic;

char* ReadTextFile(const char* path)
{
    char* buf = NULL;
    FILE* file = fopen(path, "r");
    if(file)
    {
        char c;
        while((c = fgetc(file)) != EOF)
        {
            arrput(buf, c);
        }
        arrput(buf, '\0');
    }
    else
    {
        TIDE_ERROR("File couldn't be read %s", path);
        return buf;
    }
    fclose(file);
    return buf;
}

void FreeFileText(const char* text)
{
    arrfree(text);
}

bool tide::LoadFreeType()
{
    // Initialise FreeType
    int error = FT_Init_FreeType(&library);
    if(error != 0)
    {
        TIDE_ERROR("FreeType Error: %u", error);
        return false;
    }
    
    // Create and load the quad VAO used to draw text.
    quadVAO = (VAO*) malloc(sizeof(VAO));
    tide::CreateVAO(*quadVAO);
    
    float vertices[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    tide::UploadToVAO(*quadVAO, 0, 2, vertices, 12);
    
    float texturecoords[] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
    tide::UploadToVAO(*quadVAO, 1, 2, texturecoords, 12);
    
    char* vertexSource = ReadTextFile("fontv.glsl");
    char* fragmentSource = ReadTextFile("fontf.glsl");
    
    program = (tide::SHADER_PROGRAM*) malloc(sizeof(tide::SHADER_PROGRAM));
    tide::CreateProgram(*program);
    
    if(vertexSource != NULL)
    {
        uint32_t vshader = tide::AddShaderToProgram(*program, vertexSource, TIDE_VERTEX_SHADER);
        FreeShader(vshader);
        FreeFileText(vertexSource);
    }
    if(fragmentSource != NULL)
    {
        uint32_t fshader = tide::AddShaderToProgram(*program, fragmentSource, TIDE_FRAGMENT_SHADER);
        FreeShader(fshader);
        FreeFileText(fragmentSource);
    }
    tide::LinkAndValidateProgram(*program);
    
    locTransformation = tide::GetUniformLocation(*program, "transformation");
    locOrthographic = tide::GetUniformLocation(*program, "orthographic");
    
    TIDE_LOG("FreeType successfully loaded");
    return true;
}

void tide::UnloadFreeType()
{
    for(int i = 0; i < arrlen(fontFaces); i++)
    {
        tide::FONT_FACE* face = fontFaces[i];
        for(auto it = face->characters->begin(); it != face->characters->end(); it++)
        {
            free(it->second); 
        }
        delete face->characters;
        free(face);
    }
    FreeVAO(*quadVAO);
    TIDE_LOG("FreeType successfully destroyed");
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
        TIDE_ERROR("FreeType Error: %u. Unable to load FreeType face", error);
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
        TIDE_ERROR("FreeType Error: %u. Unable to request size", error);
        return NULL;
    }
    
    tide::FONT_FACE* fontFace = (tide::FONT_FACE*) malloc(sizeof(tide::FONT_FACE));
    fontFace->characters = new std::map<uint8_t, tide::FONT_CHARACTER*>();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    const char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";
    int alphabetLength = strlen(alphabet);
    for(int i = 0; i < alphabetLength; i++)
    {
        char c = alphabet[i];
        auto glyphIndex = FT_Get_Char_Index(face, c);
        error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
        if (error != 0)
        {
            TIDE_ERROR("%s FreeType Error: %u. Unable to load FreeType glyph\n", error);
            continue;
        }
        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        tide::FONT_CHARACTER* character = (tide::FONT_CHARACTER*) malloc(sizeof(tide::FONT_CHARACTER));
        character->id = texture;
        character->width = face->glyph->bitmap.width;
        character->height = face->glyph->bitmap.rows;
        character->bearingX = face->glyph->bitmap_left;
        character->bearingY = face->glyph->bitmap_top;
        character->advance = face->glyph->advance.x;
        
        fontFace->characters->insert(std::map<uint8_t, tide::FONT_CHARACTER*>::value_type(c, character));
    }
    arrput(fontFaces, fontFace);
    return fontFace;
}

void tide::RenderText(const char* text, float x, float y, float scale)
{
    tide::BindProgram(program->id);
    tide::BindVAO(quadVAO->id);
    
    float xcaret = 0;
    
    int len = strlen(text);
    for(int i = 0; i < len; i++)
    {
        char c = text[i];
        
        auto pos = fontFaces[0]->characters->find(c);
        if(pos != fontFaces[0]->characters->end())
        {
            tide::FONT_CHARACTER* fchar = fontFaces[0]->characters->at(c);
            
            MAT4F tfm = IdentityM4F();
            TranslateM4F(tfm, x + xcaret * scale, y + (16 - fchar->bearingY) * scale, 0);
            ScaleM4F(tfm, fchar->width * scale, fchar->height * scale, 1);
            tide::LoadToProgram(locTransformation, tfm);
            
            // Loading Ortho
            MAT4F otm = tide::OrthoM4F(0, tide::GetDisplayWidth(), tide::GetDisplayHeight(), 0, 0, 100);
            tide::LoadToProgram(locOrthographic, otm);
            
            tide::BindTexture(GL_TEXTURE_2D, fchar->id);
            tide::DrawVAO();
            
            xcaret += (fchar->advance / 64.0);
        }
    }
    tide::BindTexture(GL_TEXTURE_2D, 0);
    tide::BindVAO(0);
    tide::BindProgram(0);
}

void tide::FreeTextRenderer()
{
    FreeProgram(*program);
    free((void*)program);
}