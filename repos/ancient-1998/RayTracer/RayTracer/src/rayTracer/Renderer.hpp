#ifndef _RayTracer_Renderer_hpp
#define _RayTracer_Renderer_hpp

#include "RayTracerTypes.hpp"
#include "Ray.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include "ParallelProcessor.hpp"

namespace rayTracer {

	class Scene;

	class Renderer {
	public:

		struct RenderStats {

			int raysShot;
			Time startTime;
			Time renderEndTime;
			Time endTime;

			RenderStats() {

				reset();	
			}

			void add(const RenderStats& stats) {

				raysShot += stats.raysShot;
			}

			void reset() {

				raysShot = 0;
				startTime = 0.0f;
				renderEndTime = 0.0f;
				endTime = 0.0f;
			}

			inline float getRenderTime() { return renderEndTime - startTime; }
			inline float getTotalTime() { return endTime - startTime; }
		};

	public:

		Renderer();

		void render(Clock& clock, Scene& scene, Camera& camera, RenderTarget& target, RenderTarget* pGlowTarget, const RenderParams& renderParams, int jobCount = 1, ParallelProcessor* pParallelProcessor = NULL);

		inline RenderParams& renderParams() { return mRenderParams; }
		inline RenderStats& renderStats() { return mRenderContext.stats; }
		inline Scene* getRenderContextScene() { return mRenderContext.scene; }
		inline Camera* getRenderContextCamera() { return mRenderContext.camera; }
		
	protected:

		struct RenderContext {

			unsigned int windowSizeX;
			unsigned int windowSizeY;

			Matrix4x3 viewMatrix;
			Matrix4x4 invProjMatrix;

			Vector3 backColor;

			float dy;
			float dx;
			int sampleCountY;
			int sampleCountX;
			float sdy;
			float sdx;
			float sampleWeight;
			float sampleSpacingY;
			float sampleSpacingX;
			float startSampleOffsetY;
			float startSampleOffsetX;

			SoftPtr<Scene> scene;
			SoftPtr<Camera> camera;
			SoftPtr<RenderTarget> target;
			SoftPtr<RenderTarget> glowTarget;
			SoftPtr<Clock> clock;

			Vector3 ambient;

			RenderStats stats;
		};

		class RenderBlock : public ParallelProcessor::Task {
		public:

			int startPixelX;
			int endPixelX;

			int startPixelY;
			int endPixelY;

			int pixelSpacing;

			SoftPtr<Renderer> renderer;
			RenderStats stats;

			virtual void execute();
			virtual ~RenderBlock();

			inline RenderContext& renderContext() { return renderer->renderContext(); }
			inline RenderParams& renderParams() { return renderer->renderParams(); }

		protected:

			bool gatherRayColor(Scene& scene, const Ray& ray, Vector3& inOutColor, Primitive* pOriginPrimitive = NULL, int selfIntersectCount = 0, int maxDepth = -1, float restColorContribution = 1.0f);
			void bumpRayShotCounter();
			void gatherLambertDiffuse(Primitive& prim, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color);
			void gatherReflections(Primitive& prim, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color, int maxDepth = -1, float restColorContribution = 1.0f);
			void gatherRefractions(Primitive* pSourcePrim, Primitive& prim, int selfIntersectCount, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color, int maxDepth = -1, float restColorContribution = 1.0f);
			void applyExposure(Vector3& color, const int pixelX, const int pixelY);
		};

		class RenderBlockManager {
		public:

			void genBlocks(Renderer& renderer, const RenderBlock& source, int blockCount);

			inline const int& getBlockCount() { return mBlocks.count; }
			inline RenderBlock& getBlock(int index) { return mBlocks[index]; }

		protected:

			typedef WETL::CountedArray<RenderBlock, false, int> RenderBlocks;
			RenderBlocks mBlocks;
		};

	protected:

		inline RenderContext& renderContext() { return mRenderContext; }
		
	protected:

		RenderParams mRenderParams;
		RenderContext mRenderContext;
		RenderBlockManager mBlockManager;
	};
}

#endif