#pragma once
#include "config.h"
#include "render/window.h"


class GUI
{
public:
    static void DrawLabel(NVGcontext* vg, const char* text, float fontSize, float x, float y, float w, float h, NVGcolor col);
    static void DrawButton(NVGcontext* vg, const char* text, float x, float y, float w, float h, NVGcolor col);
    static void DrawFilledBox(NVGcontext* vg, float x, float y, float w, float h, NVGcolor col);
    static void DrawExplosion(NVGcontext* vg, float x, float y, float w, float h, float dt);
};