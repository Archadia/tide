/* date = December 15th 2020 0:27 pm */

#ifndef TIDE_GL_H
#define TIDE_GL_H

#include <stdint.h>
#include <stdio.h>

namespace tide
{
    
    struct VAO
    {
        uint32_t id;
        uint32_t* buffers = NULL;
        bool initialised = false;
    };
    
    void CreateVAO(VAO* vao);
    void UploadToVAO(VAO& vao, uint32_t index, uint32_t size, float* data, uint32_t length);
    void FreeVAO(VAO& vao);
    
}
#endif //TIDE_GL_H
