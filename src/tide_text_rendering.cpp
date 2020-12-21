#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_log.h"
#include "tide_gl.h"
#include "tide_display.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stb_ds.h>

#include <glad/glad.h>

static SHADER_PROGRAM* program;
static VAO* quadVAO;

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

void InitTextRendering()
{
    // Creating Shader Program
    program = (SHADER_PROGRAM*) malloc(sizeof(SHADER_PROGRAM));
    // TODO(kroma): Probably should be passed in as a pointer.
    CreateProgram(*program);
    
    char* vertexSource = ReadTextFile("fontv.glsl");
    char* fragmentSource = ReadTextFile("fontf.glsl");
    
    if(vertexSource != NULL)
    {
        uint32_t vshader = AddShaderToProgram(*program, vertexSource, GL_VERTEX_SHADER);
        FreeShader(vshader);
        FreeFileText(vertexSource);
    }
    
    if(vertexSource != NULL)
    {
        uint32_t fshader = AddShaderToProgram(*program, fragmentSource, GL_FRAGMENT_SHADER);
        FreeShader(fshader);
        FreeFileText(fragmentSource);
    }
    LinkAndValidateProgram(*program);
    
    // Create quad
    
    quadVAO = (VAO*) malloc(sizeof(VAO));
    CreateVAO(*quadVAO);
    
    float vertices[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    UploadToVAO(*quadVAO, 0, 2, vertices, 12);
    
    float texturecoords[] = {0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
    UploadToVAO(*quadVAO, 1, 2, texturecoords, 12);
    
    // Create uniform locations
    
    locTransformation = GetUniformLocation(*program, "transformation");
    locOrthographic = GetUniformLocation(*program, "orthographic");
}

void RenderText(TSDF_FONT& font, const char* text, float x, float y, float scale)
{
    BindProgram(program->id);
    BindVAO(quadVAO->id);
    
    // main rendering
    MAT4F orm = OrthoM4F(0, GetDisplayWidth(), GetDisplayHeight(), 0, 0, 100);
    LoadToProgram(locOrthographic, orm);
    
    int xcaret = 0;
    
    int len = strlen(text);
    for(int i = 0; i < len; i++)
    {
        char c = text[i];
        tide:TSDF_GLYPH* glyph = hmget(font.glyphs, c);
        
        BindTexture(GL_TEXTURE_2D_ARRAY, font.texture->id);
        
        MAT4F tfm = IdentityM4F();
        TranslateM4F(tfm, x + xcaret * scale, y + (16 - glyph->bearingY) * scale, 0);
        ScaleM4F(tfm, glyph->width * scale, glyph->height * scale, 1);//glyph->width, glyph->height, 1);
        LoadToProgram(locTransformation, tfm);
        
        DrawVAO();
        xcaret += glyph->advanceX/64;
    }
    
    BindTexture(GL_TEXTURE_2D_ARRAY, 0);
    BindProgram(0);
    BindVAO(0);
}

void FreeTextRendering()
{
    FreeVAO(*quadVAO);
    FreeProgram(*program);
    free((void*) quadVAO);
    free((void*) program);
}