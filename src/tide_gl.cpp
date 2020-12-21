#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"
#include <glad/glad.h>

#include <stb_ds.h>

void CreateVAO(VAO& vao)
{
    uint32_t id;
    glGenVertexArrays(1, &id);
    vao.buffers = NULL;
    vao.id = id;
}

void UploadToVAO(VAO& vao, uint32_t index, uint32_t size, float* data, uint32_t length)
{
    glBindVertexArray(vao.id);
    uint32_t vbo;
    if(!vao.initialised || index >= arrlen(vao.buffers))
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, 0);
        arrput(vao.buffers, vbo);
        vao.initialised = true;
    }
    else
    {
        vbo = vao.buffers[index];
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FreeVAO(VAO& vao)
{
    if(vao.buffers)
    {
        if(arrlen(vao.buffers) > 0)
        {
            glDeleteBuffers(arrlen(vao.buffers), vao.buffers);
            arrfree(vao.buffers);
        }
    }
    glDeleteVertexArrays(1, &vao.id);
}

void BindVAO(uint32_t id)
{
    glBindVertexArray(id);
}

void DrawVAO()
{
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CreateProgram(SHADER_PROGRAM& program)
{
    program.id = glCreateProgram();
}

uint32_t AddShaderToProgram(SHADER_PROGRAM& program, const char* source, uint32_t glType)
{
    uint32_t shader = glCreateShader(glType);
    
    glShaderSource(shader, 1, (const char**) &source, NULL);
    glCompileShader(shader);
    
    int32_t compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        TIDE_ERROR("Shader compilation failed [TYPE=%u]", glType);
        glDeleteShader(shader);
        return 0;
    }
    glAttachShader(program.id, shader);
    return shader;
}

void LinkAndValidateProgram(SHADER_PROGRAM& program)
{
    glLinkProgram(program.id);
    glValidateProgram(program.id);
    
    int32_t linked = 0;
    glGetProgramiv(program.id, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        TIDE_ERROR("Program linking failed");
        //FreeShadersAndProgram(program);
        return;
    }
}

void FreeProgram(SHADER_PROGRAM& program)
{
    glDeleteProgram(program.id);
}

void FreeShader(uint32_t shader)
{
    glDeleteShader(shader);
}

uint32_t GetUniformLocation(SHADER_PROGRAM& program, const char* name)
{
    return glGetUniformLocation(program.id, name);
}

template<> void LoadToProgram(uint32_t location, int value)
{
    glUniform1i(location, value);
}

template<> void LoadToProgram(uint32_t location, float value)
{
    glUniform1f(location, value);
}

template<> void LoadToProgram(uint32_t location, MAT4F value)
{
    glUniformMatrix4fv(location, 1, false, value.d);
}

// TODO(kroma): do loading for vec2, vec3, vec4, mat4

void BindProgram(uint32_t id)
{
    glUseProgram(id);
}

// Texture

void CreateArrayTexture(int width, int height, COMPILATION_TEXTURE* out)
{
    out->width = width;
    out->height = height;
    out->buffer = NULL;
}

// TODO(kroma): consider changing COMPILATION_TEXTURE to reference
void AddToArrayTexture(unsigned char* buffer, COMPILATION_TEXTURE* tex)
{
    for(int y = 0; y < tex->height; y++)
    {
        for(int x = 0; x < tex->width; x++)
        {
            //arrput(tex->buffer, buffer[x + y * tex->width]);
            tex->size++;
        }
    }
}

void CompileArrayTexture(COMPILATION_TEXTURE& in, uint32_t format, ARRAY_TEXTURE* out)
{
    glGenTextures(1, &out->id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, out->id);
    
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, in.width, in.height, in.size, 0, format, GL_UNSIGNED_BYTE, in.buffer);
    
    // Params, gonna keep with just nearest for now
    // TODO(kroma), allow the caller to choose tex params.
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    
    // Free the input buffer, the caller can deal with the allocation.
    FreeArrayTexture(&in);
    
    out->width = in.width;
    out->height = in.height;
}

void FreeArrayTexture(COMPILATION_TEXTURE* tex)
{
    arrfree(tex->buffer);
}

void BindTexture(int type, uint32_t id)
{
    glBindTexture(type, id);
}