/* date = December 15th 2020 0:27 pm */

#ifndef TIDE_GL_H
#define TIDE_GL_H

#include <stdint.h>
#include <stdio.h>

struct VAO
{
    uint32_t id;
    uint32_t *buffers = NULL;
    bool initialised = false;
};

void CreateVAO(VAO &vao);
void UploadToVAO(VAO &vao, uint32_t index, uint32_t size, float *data, uint32_t length);
void FreeVAO(VAO &vao);
void BindVAO(uint32_t id);
void DrawVAO();

// Shader Program

struct SHADER_PROGRAM
{
    uint32_t id;
};

void CreateProgram(SHADER_PROGRAM &program);
uint32_t AddShaderToProgram(SHADER_PROGRAM &program, const char *source, uint32_t glType);
void LinkAndValidateProgram(SHADER_PROGRAM &program);
void FreeProgram(SHADER_PROGRAM &program);
void FreeShader(uint32_t shader);
uint32_t GetUniformLocation(SHADER_PROGRAM &program, const char *name);
template <typename T>
void LoadToProgram(uint32_t location, T value);
void BindProgram(uint32_t id);

// Texture

// A intermediate structure for storing the compilation
// of textures to put in the array.
struct COMPILATION_TEXTURE
{
    int size;
    int width;
    int height;
    unsigned char *buffer;
};

struct ARRAY_TEXTURE
{
    int width;
    int height;
    uint32_t id;
};

void CreateArrayTexture(int width, int height, COMPILATION_TEXTURE *out);

void AddToArrayTexture(unsigned char *buffer, COMPILATION_TEXTURE *tex);

void CompileArrayTexture(COMPILATION_TEXTURE &in, uint32_t format, ARRAY_TEXTURE *out);

void FreeArrayTexture(COMPILATION_TEXTURE *tex);

// function to reduce glad includes,
// though im not sure whether this is useful
// or is just poluting the namespace at this
// point, I'll have a think
// TODO(kroma): *think*
void BindTexture(int type, uint32_t id);

#endif //TIDE_GL_H
