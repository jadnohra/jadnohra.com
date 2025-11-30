#include "GaussianBlur.hpp"
#include "../WE3/WETL/WETLSizeAlloc.h"

#ifdef RayTracerConfig_SSE
	#include "xmmintrin.h"
	#include "dvec.h"
#endif

namespace rayTracer {

static const int kernelSize = 13;
static const int kernelStartOffset = -((kernelSize - 1) / 2);
static const int kernelEndOffset = (-kernelStartOffset);
static const float sigma = 3.5f;
static const float g = 1.0f / (sigma * sqrt( 2.0f * 3.1415927f));

//f(x) = (1/(sigma*sqrt(2*PI))) * exp(-0.5*((x-mu)/sigma)^2)

static const float BlurWeights[kernelSize] = 
{
	g * exp( -36.0/(2.0*sigma*sigma) ),
	g * exp( -25.0/(2.0*sigma*sigma) ),
	g * exp( -16.0/(2.0*sigma*sigma) ),
	g * exp( -9.0/(2.0*sigma*sigma) ),
	g * exp( -3.0/(2.0*sigma*sigma) ),
	g * exp( -1.0/(2.0*sigma*sigma) ),
	g,
	g * exp( -1.0/(2.0*sigma*sigma) ),
	g * exp( -3.0/(2.0*sigma*sigma) ),
	g * exp( -9.0/(2.0*sigma*sigma) ),
	g * exp( -16.0/(2.0*sigma*sigma) ),
	g * exp( -25.0/(2.0*sigma*sigma) ),
	g * exp( -36.0/(2.0*sigma*sigma) )
};

void GaussianBlur::transform(Clock& clock, RenderTarget& src, RenderTarget& dest) {

	int w = src.getWidth();
	int h = src.getHeight();

	#ifdef RayTracerConfig_SSE
		const int alignment = 16;
	#else
		const int alignment = 0;
	#endif

	#ifdef RayTracerConfig_SSE
	__declspec(align(16))
	#endif
	struct Pixel {

		union {
			
			struct { float el[4]; };
			
			#ifdef RayTracerConfig_SSE
			struct { __m128 rgba; };
			#endif
		};
	};


	typedef WETL::SizeAllocT<Pixel, unsigned int> Buffer;
	Buffer pixels;

	pixels.resize(w * h, alignment);

	if ((((unsigned int) pixels.el) % 16) != 0) {

		int x = 0;
	}

	float processTime = clock.getRawTime();

	{
		//horiz
		for (int yd = 0; yd < h; ++yd) {

			for (int xd = 0; xd < w; ++xd) {

				Pixel& dstPixel = pixels.el[yd * w + xd];

				#ifdef RayTracerConfig_SSE

					dstPixel.rgba = _mm_setzero_ps();

					if ((((unsigned int) &dstPixel.rgba) % 16) != 0) {

						int x = 0;
					}
				
				#else

					float& rd = dstPixel.el[0];
					float& gd = dstPixel.el[1];
					float& bd = dstPixel.el[2];
					float& ad = dstPixel.el[3];
					
					rd = 0.0f;
					gd = 0.0f;
					bd = 0.0f;
					ad = 0.0f;

				#endif
				
				for (int ky = 0; ky <= 0; ++ky) {

					int ys = yd + ky;

					if (ys >= 0 && ys < h) {

						const float* kw = BlurWeights;
						for (int kx = kernelStartOffset; kx <= kernelEndOffset; ++kx) {

							int xs = xd + kx;

							if (xs >= 0 && xs < w) {

								Pixel srcPixel;
								src.getPixel(xs, ys, srcPixel.el[0], srcPixel.el[1], srcPixel.el[2], srcPixel.el[3]);
								
								#ifdef RayTracerConfig_SSE

									__m128 weightedSrc = _mm_mul_ps(_mm_set1_ps(kw[0]), _mm_load_ps(srcPixel.el));
									dstPixel.rgba = _mm_add_ps(dstPixel.rgba, weightedSrc);

								#else
									rd += srcPixel.el[0] * kw[0];
									gd += srcPixel.el[1] * kw[0];
									bd += srcPixel.el[2] * kw[0];
									ad += srcPixel.el[3] * kw[0];
								#endif
							}

							++kw;
						}
					}
				}
			}
		}
	}

	{
		//vert
		for (int yd = 0; yd < h; ++yd) {

			for (int xd = 0; xd < w; ++xd) {

				Pixel dstPixel;

				#ifdef RayTracerConfig_SSE

					dstPixel.rgba = _mm_setzero_ps();

					if ((((unsigned int) &dstPixel.rgba) % 16) != 0) {

						int x = 0;
					}

				#else

					dstPixel.el[0] = 0.0f;
					dstPixel.el[1] = 0.0f;
					dstPixel.el[2] = 0.0f;
					dstPixel.el[3] = 0.0f;

				#endif

				const float* kw = BlurWeights;
				for (int ky = kernelStartOffset; ky <= kernelEndOffset; ++ky) {
				
					int ys = yd + ky;

					if (ys >= 0 && ys < h) {

						for (int kx = 0; kx <= 0; ++kx) {

							int xs = xd + kx;

							if (xs >= 0 && xs < w) {

								Pixel& srcPixel = pixels.el[ys * w + xs];

								#ifdef RayTracerConfig_SSE

									__m128 weightedSrc = _mm_mul_ps(_mm_set1_ps(kw[0]), _mm_load_ps(srcPixel.el));
									dstPixel.rgba = _mm_add_ps(dstPixel.rgba, weightedSrc);

								#else

									dstPixel.el[0] += srcPixel.el[0] * kw[0];
									dstPixel.el[1] += srcPixel.el[1] * kw[0];
									dstPixel.el[2] += srcPixel.el[2] * kw[0];
									dstPixel.el[3] += srcPixel.el[3] * kw[0];

								#endif
							}
						}
					}

					++kw;
				}

				#ifdef RayTracerConfig_SSE

					Pixel finalPixel;

					_mm_store_ps(finalPixel.el, dstPixel.rgba);
					
					dest.setPixel(xd, yd, finalPixel.el[0], finalPixel.el[1], finalPixel.el[2], finalPixel.el[3]);

				#else

					dest.setPixel(xd, yd, dstPixel.el[0], dstPixel.el[1], dstPixel.el[2], dstPixel.el[3]);

				#endif
			}
		}
	}

	processTime = clock.getRawTime() - processTime;

	printf("filter process time: %f secs\n", processTime);
}


}