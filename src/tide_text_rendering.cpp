#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_log.h"
#include "tide_gl.h"
#include "tide_display.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stb_ds.h>

#include <glad/glad.h>

static tide::SHADER_PROGRAM* program;
static tide::VAO* quadVAO;

static uint32_t locTransformation;
static uint32_t locOrthographic;

// TODO(kroma): Move somewhere on its own.
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

void tide::InitTextRendering()
{
    // Creating Shader Program
    program = (tide::SHADER_PROGRAM*) malloc(sizeof(tide::SHADER_PROGRAM));
    // TODO: Probably should be passed in as a pointer.
    tide::CreateProgram(*program);
    
    char* vertexSource = ReadTextFile("fontv.glsl");
    char* fragmentSource = ReadTextFile("fontf.glsl");
    
    if(vertexSource != NULL)
    {
        uint32_t vshader = tide::AddShaderToProgram(*program, vertexSource, GL_VERTEX_SHADER);
        tide::FreeShader(vshader);
        FreeFileText(vertexSource);
    }
    
    if(vertexSource != NULL)
    {
        uint32_t fshader = tide::AddShaderToProgram(*program, fragmentSource, GL_FRAGMENT_SHADER);
        tide::FreeShader(fshader);
        FreeFileText(fragmentSource);
    }
    tide::LinkAndValidateProgram(*program);
    
    // Create quad
    
    quadVAO = (tide::VAO*) malloc(sizeof(tide::VAO));
    tide::CreateVAO(*quadVAO);
    
    float vertices[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    tide::UploadToVAO(*quadVAO, 0, 2, vertices, 12);
    
    float texturecoords[] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
    tide::UploadToVAO(*quadVAO, 1, 2, texturecoords, 12);
    
    // Create uniform locations
    
    locTransformation = tide::GetUniformLocation(*program, "transformation");
    locOrthographic = tide::GetUniformLocation(*program, "orthographic");
}

void tide::RenderText(tide::TSDF_FONT& font, const char* text, float x, float y, float scale)
{
    tide::BindProgram(program->id);
    tide::BindVAO(quadVAO->id);
    
    // main rendering
    tide::MAT4F orm = tide::OrthoM4F(0, tide::GetDisplayWidth(), tide::GetDisplayHeight(), 0, 0, 100);
    tide::LoadToProgram(locOrthographic, orm);
    
    int xcaret = 0;
    
    int len = strlen(text);
    
    for(int i = 0; i < len; i++)
    {
        char c = text[i];
        tide:TSDF_GLYPH* glyph = hmget(font.glyphs, c);
        
        tide::MAT4F tfm = tide::IdentityM4F();
        tide::TranslateM4F(tfm, x + xcaret * scale, y + (16 - glyph->bearingY) * scale, 0);
        tide::ScaleM4F(tfm, glyph->width, glyph->height, 1);//glyph->width, glyph->height, 1);
        tide::LoadToProgram(locTransformation, tfm);
        
        tide::DrawVAO();
        xcaret += glyph->advanceX/64;
    }
    
    tide::BindProgram(0);
    tide::BindVAO(0);
}

void tide::FreeTextRendering()
{
    FreeVAO(*quadVAO);
    FreeProgram(*program);
    free((void*) quadVAO);
    free((void*) program);
}