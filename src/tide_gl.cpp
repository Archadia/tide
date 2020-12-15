#include "tide_gl.h"
#include <glad/glad.h>

#include <stb_ds.h>

void tide::CreateVAO(VAO* vao)
{
    uint32_t id;
    glGenVertexArrays(1, &id);
    vao->buffers = NULL;
    vao->id = id;
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