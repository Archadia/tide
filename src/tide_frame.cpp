#include "tide_frame.h"

void tide::FrameSplit(tide::FRAME& frame)
{
    frame.split = true;
    frame.lSplit = {};
    frame.lSplit->parent = &frame;
    
    frame.rSplit = {};
    frame.rSplit->parent = &frame;
}