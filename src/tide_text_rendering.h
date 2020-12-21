#ifndef TIDE_TEXT_RENDERING_H
#define TIDE_TEXT_RENDERING_H

#include <stdint.h>
#include <map>

#include "tide_sdf.h"

void InitTextRendering();
void FreeTextRendering();
void RenderText(TSDF_FONT& font, const char* text, float x, float y, float scale);

#endif