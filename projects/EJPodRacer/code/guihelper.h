#pragma once
#include "config.h"
#include "render/window.h"


class GUI
{
public:
    static void DrawLabel(NVGcontext* vg, const char* text, float x, float y, float w, float h);
    static void DrawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor col);
};