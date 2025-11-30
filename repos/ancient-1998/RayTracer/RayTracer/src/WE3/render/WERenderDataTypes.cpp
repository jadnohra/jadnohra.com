#include "WERenderDataTypes.h"

namespace WE {

const RenderColor RenderColor::kBlack(0.0f, 0.0f, 0.0f);
const RenderColor RenderColor::kWhite(1.0f, 1.0f, 1.0f);
const RenderColor RenderColor::kRed(1.0f, 0.0f, 0.0f);
const RenderColor RenderColor::kGreen(0.0f, 1.0f, 0.0f);
const RenderColor RenderColor::kBlue(0.0f, 0.0f, 1.0f);
const RenderColor RenderColor::kYellow(1.0f, 1.0f, 0.0f);
const RenderColor RenderColor::kMagenta(1.0f, 0.0f, 1.0f);
const RenderColor RenderColor::kBluish1(0.176f, 0.196f, 0.666f);
const RenderColor RenderColor::kHotPink(255, 105, 180);
const RenderColor RenderColor::kDeepPink(255, 20, 147);
const RenderColor RenderColor::kLightPink(255, 182, 193);


RenderColor::RenderColor() {
}

RenderColor::RenderColor(float r, float g, float b, float _alpha) {

	set(r, g, b, _alpha);
}

RenderColor::RenderColor(int r, int g, int b, int _alpha) {

	set(r, g, b, _alpha);
}

RenderColor::RenderColor(int scale, int alpha) {

	set(scale, alpha);
}

void RenderColor::set(float r, float g, float b, float _alpha) {

	el[RC_Red] = r;
	el[RC_Green] = g;
	el[RC_Blue] = b;
	alpha = _alpha;
}


void RenderColor::set(int r, int g, int b, int _alpha) {

	el[RC_Red] = (float) r / 255.0f;
	el[RC_Green] = (float) g / 255.0f;
	el[RC_Blue] = (float) b / 255.0f;
	alpha = (float) _alpha / 255.0f;
}

void RenderColor::set(float scale, float _alpha) {

	set(scale, scale, scale, _alpha);
}

void RenderColor::set(int scale, int _alpha) {

	set(scale, scale, scale, _alpha);
}

void RenderColor::toColorComplement() {

	el[RC_Red] = 1.0f - el[RC_Red];
	el[RC_Green] = 1.0f - el[RC_Green];
	el[RC_Blue] = 1.0f - el[RC_Blue];
}

}