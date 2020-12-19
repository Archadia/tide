#include "tide_process.h"
#include "tide_display.h"
#include "tide_text_rendering.h"
#include "tide_math.h"
#include "tide_gl.h"
#include "tide_log.h"
#include "tide_sdf.h"

#include <stdio.h>
#include <string.h>

void tide::ProcessInit()
{
    //tide::LoadFreeType();
    //tide::LoadFace("../font/Hack-Regular.ttf", 16, 0);
    
    tide::InitTSDF();
    
    TSDF_FONT font;
    
    tide::AddBitmapFont("../font/Hack-Regular.ttf", &font);
    
    TSDF_GLYPH character;
    tide::AddBitmapChar(font, 'a', &character);
}

void tide::ProcessUpdate(double t, double dt)
{
    
}

void tide::ProcessDraw()
{
    //tide::RenderText("test 123", 10, 10, 1);
}

void tide::ProcessClose()
{
    tide::DeInitTSDF();
    //tide::UnloadFreeType();
    //tide::FreeTextRenderer();
}