/* date = December 15th 2020 0:27 pm */

#ifndef TIDE_GL_H
#define TIDE_GL_H

#include <stdint.h>
#include <stdio.h>

#define TIDE_VERTEX_SHADER 0x8B31
#define TIDE_FRAGMENT_SHADER 0x8B30

namespace tide
{
    
    struct VAO
    {
        uint32_t id;
        uint32_t* buffers = NULL;
        bool initialised = false;
    };
    
    void CreateVAO(VAO& vao);
    void UploadToVAO(VAO& vao, uint32_t index, uint32_t size, float* data, uint32_t length);
    void FreeVAO(VAO& vao);
    
    // 
    
    struct SHADER_PROGRAM
    {
        uint32_t id;
    };
    
    void CreateProgram(SHADER_PROGRAM& program);
    uint32_t AddShaderToProgram(SHADER_PROGRAM& program, const char* source, uint32_t glType);
    void LinkAndValidateProgram(SHADER_PROGRAM& program);
    void FreeProgram(SHADER_PROGRAM& program);
    void FreeShader(uint32_t shader);
    uint32_t GetUniformLocation(SHADER_PROGRAM& program, const char* name);
    template<typename T> void LoadToProgram(uint32_t location, T value);
    void BindProgram(uint32_t id);
}
#endif //TIDE_GL_H
