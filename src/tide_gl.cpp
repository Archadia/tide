#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"
#include <glad/glad.h>

#include <stb_ds.h>

void tide::CreateVAO(VAO& vao)
{
    uint32_t id;
    glGenVertexArrays(1, &id);
    vao.buffers = NULL;
    vao.id = id;
}

void tide::UploadToVAO(VAO& vao, uint32_t index, uint32_t size, float* data, uint32_t length)
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

void tide::FreeVAO(VAO& vao)
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

void tide::BindVAO(uint32_t id)
{
    glBindVertexArray(id);
}

void tide::DrawVAO()
{
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void tide::CreateProgram(SHADER_PROGRAM& program)
{
    program.id = glCreateProgram();
}

uint32_t tide::AddShaderToProgram(SHADER_PROGRAM& program, const char* source, uint32_t glType)
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

void tide::LinkAndValidateProgram(SHADER_PROGRAM& program)
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

void tide::FreeProgram(SHADER_PROGRAM& program)
{
    glDeleteProgram(program.id);
}

void tide::FreeShader(uint32_t shader)
{
    glDeleteShader(shader);
}

uint32_t tide::GetUniformLocation(SHADER_PROGRAM& program, const char* name)
{
    return glGetUniformLocation(program.id, name);
}

template<> void tide::LoadToProgram(uint32_t location, int value)
{
    glUniform1i(location, value);
}

template<> void tide::LoadToProgram(uint32_t location, float value)
{
    glUniform1f(location, value);
}

template<> void tide::LoadToProgram(uint32_t location, tide::MAT4F value)
{
    glUniformMatrix4fv(location, 1, false, value.d);
}

// TODO(kroma): do loading for vec2, vec3, vec4, mat4

void tide::BindProgram(uint32_t id)
{
    glUseProgram(id);
}

// Texture

void tide::BindTexture(uint32_t id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}