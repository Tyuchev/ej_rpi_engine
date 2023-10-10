#include "config.h"
#include "guihelper.h"

// Most code taken from nvg demo.c

int isBlack(NVGcolor col)
{
	if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
	{
		return 1;
	}
	return 0;
}

void GUI::DrawFilledBox(NVGcontext* vg, float x, float y, float w, float h, NVGcolor col) {
    nvgBeginPath(vg);
    nvgRect(vg, x, y, w, h);
    nvgFillColor(vg, col);
    nvgFill(vg);
    nvgClosePath(vg);
}

void GUI::DrawLabel(NVGcontext* vg, const char* text, float fontSize, float x, float y, float w, float h, NVGcolor col) {
	NVG_NOTUSED(w);

	nvgFontSize(vg, fontSize);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, col);

	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x,y+h*0.5f,text, NULL);
}

// No callback functionality, this is probably not needed as we can just use labels and time them.
void GUI::DrawButton(NVGcontext* vg, const char* text, float x, float y, float w, float h, NVGcolor col) {
	NVGpaint bg;
	float cornerRadius = 4.0f;
	float tw = 0, iw = 0;

	bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
	if (!isBlack(col)) {
		nvgFillColor(vg, col);
		nvgFill(vg);
	}
	nvgFillPaint(vg, bg);
	nvgFill(vg);

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans-bold");
	tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);

	nvgFontSize(vg, 17.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGBA(0,0,0,160));
	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);
}