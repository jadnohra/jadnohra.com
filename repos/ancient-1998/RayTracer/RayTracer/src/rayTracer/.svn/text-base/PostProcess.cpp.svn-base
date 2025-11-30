#include "PostProcess.hpp"
#include "GaussianBlur.hpp"
#include "RenderImage.hpp"

namespace rayTracer {

void PostProcess::finalize(Clock& clock, const RenderParams& renderParams, RenderTarget& renderTarget, RenderTarget* pGlowBuffer) {

	if (renderParams.exposureMode == ExposureMode_TestGlow && pGlowBuffer) {

		FloatRenderBuffer blurredGlowBuffer;

		blurredGlowBuffer.create(pGlowBuffer->getWidth(), pGlowBuffer->getHeight());

		GaussianBlur::transform(clock, *pGlowBuffer, blurredGlowBuffer);

		renderTarget.addToPixels(blurredGlowBuffer);

		/*
		{
			RenderImage showGlow;
			showGlow.create(blurredGlowBuffer.getWidth(), blurredGlowBuffer.getHeight());

			showGlow.copyPixelsFrom(blurredGlowBuffer);
			showGlow.write(L"showGlow.tga");
		}
		*/
	}
}

}