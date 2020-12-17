#include "tide_process.h"
#include "tide_display.h"
#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"

#include <stdio.h>
#include <string.h>

void tide::ProcessInit()
{
    tide::LoadFreeType();
    tide::LoadFace("../font/Hack-Regular.ttf", 16, 0);
    
    tide::SHADER_PROGRAM* program = (tide::SHADER_PROGRAM*) malloc(sizeof(tide::SHADER_PROGRAM));
    
    const char* text = "#version 330\nuniform float test;\nvoid main() {\n}";
    
    tide::CreateProgram(*program);
    uint32_t vertexShader = tide::AddShaderToProgram(*program, text, TIDE_VERTEX_SHADER);
    tide::LinkAndValidateProgram(*program);
    tide::FreeShader(vertexShader);
    
    uint32_t location = tide::GetUniformLocation(*program, "test");
    tide::LoadToProgram<float>(location, 1);
    
    tide::FreeProgram(*program);
    free((void*) program);
}

void tide::ProcessUpdate(double t, double dt)
{
}

void tide::ProcessDraw()
{
    tide::RenderText("void main();", 0, 0, 1);
}

void tide::ProcessClose()
{
    tide::FreeTextRenderer();
    tide::UnloadFreeType();
}