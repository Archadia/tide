#include "tide_process.h"
#include "tide_display.h"
#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"
#include "tide_sdf.h"

#include <stdio.h>

static TSDF_FONT* font;

void ProcessInit()
{
    InitTSDF();
    
    font = (TSDF_FONT*) malloc(sizeof(TSDF_FONT));
    AddBitmapFont("../font/Hack-Regular.ttf", font);
    
    for(int i = ' '; i < '~'; i++)
    {
        AddBitmapChar(*font, i);
    }
    CompileBitmapTexture(*font);
    
    InitTextRendering();
}

void ProcessUpdate(double t, double dt)
{
    
}

void ProcessDraw()
{
    RenderText(*font, "test", 0, 0, 5);
}

void ProcessClose()
{
    FreeFontMetadata(font);
    FreeTextRendering();
    FreeTSDF();
    free((void*) font);
}