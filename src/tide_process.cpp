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
    
    MAT4F mat1 = { 1, 15, 6, -54,
        2, -34, 23, 2.534,
        434.6, -34.56, 3, 4343,
        -4354, 3423, -2342, 3
    };
    
    VEC4F vec1 = { 1, 2, 3, 4 };
    
    VEC4F newVec = mat1 * vec1;
    
    printf("%f, %f, %f, %f\n", newVec.x, newVec.y, newVec.z, newVec.w);
}

void tide::ProcessUpdate(double t, double dt)
{
    
}

void tide::ProcessDraw()
{
    
}

void tide::ProcessClose()
{
    tide::UnloadFreeType();
}