#include "config.h"
#include "guihelper.h"
#include "render/stb_image.h"

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

// Taken from nanovg.c since it didn't wanna compile in there.
int nvgCreateImage(NVGcontext* ctx, const char* filename, int imageFlags)
{
	int w, h, n, image;
	unsigned char* img;
	stbi_set_unpremultiply_on_load(1);
	stbi_convert_iphone_png_to_rgb(1);
	img = stbi_load(filename, &w, &h, &n, 4);
	if (img == NULL) {
		printf("Failed to load %s - %s\n", filename, stbi_failure_reason());
		return 0;
	}
	image = nvgCreateImageRGBA(ctx, w, h, imageFlags, img);
	stbi_image_free(img);
	return image;
}

void GUI::DrawExplosion(NVGcontext* vg, float x, float y, float w, float h, float dt) {
	const int NUM_FRAMES = 50;
	static int frames[50] = {123};
	if (frames[0] == 123) {
		printf("once\n");
		for (int i = 0; i < NUM_FRAMES; i++) {
			char file[128];
			snprintf(file, 128, "assets/explosion/tile%d.png", i);
			frames[i] = nvgCreateImage(vg, file, 0);
		}

	}

	const float FRAME_DELAY = 0.1f;
	const float FRAME_SIZE = 100.0f;

	static float frameTime = 0.0f;
	static int currentFrame = 0;
	if (frameTime > FRAME_DELAY) {
		if (currentFrame < NUM_FRAMES) {
			currentFrame++;
			frameTime = 0.0f;
		}
		else {
			currentFrame = 0;
		}
	}

	DrawFilledBox(vg, x, y, w, h, nvgRGBA(0, 0, 0, 255));

	nvgBeginPath(vg);

	//float tx = 

	//NVGpaint imgPaint = nvgImagePattern(vg, 0, 300.0, 100.0f, 100.0f, 0.0f, frames[currentFrame], 1.0f);
	//NVGpaint imgPaint = nvgImagePattern(vg, 0.0f, 0.0f, 1000.0f, 500.0f, 0.0f, frames[currentFrame], 1.0f);
	//NVGpaint imgPaint = nvgImagePattern(vg, x, y, x + 1000.0f, y + 500.0f, 0.0f, frames[currentFrame], 1.0f);
	// Works.
	NVGpaint imgPaint = nvgImagePattern(vg, x, y, FRAME_SIZE, FRAME_SIZE, 0.0f, frames[currentFrame], 1.0f);
	
	//nvgRect(vg, tx, ty, w, h);
	//nvgRect(vg, x + tx, y + ty, TILE_XSIZE, TILE_YSIZE);
	// Works.
	nvgRect(vg, x, y, w, h);
	nvgFillPaint(vg, imgPaint);
	nvgFill(vg);

	frameTime += dt;
}