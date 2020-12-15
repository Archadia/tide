#include "tide_process.h"
#include "tide_display.h"
#include "tide_text_rendering.h"

#include "tide_log.h"

#include <stdio.h>

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
    
}

void tide::ProcessClose()
{
    tide::UnloadFreeType();
}