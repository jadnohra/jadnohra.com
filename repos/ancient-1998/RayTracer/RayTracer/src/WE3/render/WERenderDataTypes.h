#ifndef _WERenderDataTypes_h
#define _WERenderDataTypes_h

#include "../WEDataTypes.h"
#include "../math/WEVector.h"

namespace WE {
	
	typedef UINT32 RenderUINT;

	enum RenderColorElement {

		RC_Red = 0, RC_Green = 1, RC_Blue = 2
	};

	struct RenderColor : WE::Vector3 {

		float alpha;

		RenderColor();
		RenderColor(float r, float g, float b, float alpha = 1.0f);
		RenderColor(int r, int g, int b, int alpha = 255);
		RenderColor(int scale, int alpha = 255);

		void set(float r, float g, float b, float alpha = 1.0f);
		void set(int r, int g, int b, int alpha = 255);
		void set(float scale, float alpha = 1.0f);
		void set(int scale, int alpha = 255);

		inline void set2(float r, float g, float b, float alpha) { set(r, g, b, alpha); }

		inline float getR() { return el[RC_Red]; }
		inline float getG() { return el[RC_Green]; }
		inline float getB() { return el[RC_Blue]; }
		inline float getAlpha() { return alpha; }

		void toColorComplement();

		static const RenderColor kBlack;
		static const RenderColor kWhite;
		static const RenderColor kRed;
		static const RenderColor kGreen;
		static const RenderColor kBlue;
		static const RenderColor kYellow;
		static const RenderColor kMagenta;
		static const RenderColor kBluish1;
		static const RenderColor kHotPink;
		static const RenderColor kDeepPink;
		static const RenderColor kLightPink;
	};
}

#endif