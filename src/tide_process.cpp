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
}

void tide::ProcessUpdate(double t, double dt)
{
}

void tide::ProcessDraw()
{
    tide::RenderText("test 123", 10, 10, 1);
}

void tide::ProcessClose()
{
    tide::FreeTextRenderer();
    tide::UnloadFreeType();
}