#include "tide_process.h"
#include "tide_display.h"
#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"
#include "tide_sdf.h"

#include <stdio.h>

static tide::TSDF_FONT* font;

void tide::ProcessInit()
{
    tide::InitTSDF();
    
    font = (TSDF_FONT*) malloc(sizeof(TSDF_FONT));
    tide::AddBitmapFont("../font/Hack-Regular.ttf", font);
    
    for(int i = ' '; i < '~'; i++)
    {
        tide::AddBitmapChar(*font, i);
    }
    
    tide::InitTextRendering();
}

void tide::ProcessUpdate(double t, double dt)
{
    
}

void tide::ProcessDraw()
{
    tide::RenderText(*font, "test", 0, 0, 1);
}

void tide::ProcessClose()
{
    tide::FreeFontMetadata(font);
    tide::FreeTextRendering();
    tide::FreeTSDF();
    free((void*) font);
}