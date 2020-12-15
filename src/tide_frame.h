#ifndef TIDE_FRAME_H
#define TIDE_FRAME_H

namespace tide
{

struct FRAME
{
    bool split;
    FRAME* parent;
    FRAME* lSplit;
    FRAME* rSplit;

    const char* buffer;
};

void FrameSplit(FRAME& frame);

}

#endif // TIDE_FRAME_H