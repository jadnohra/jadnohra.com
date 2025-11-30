#include "SuperHeader.hpp"
#include "rayTracer/RayTracer.hpp"

//http://www.devmaster.net/articles/raytracing_series/part1.php
//http://www.codermind.com/articles/Raytracer-in-C++-Part-I-First-rays.html
//http://www.mandelbrot-dazibao.com/Raytuto/Raytuto.htm

//using namespace rayTracer;
#include "WE3/WELog.h"
#include "WE3/render/WERenderDataTypes.h"
using namespace WE;
#include <iostream>

#include "rayTracer/GaussianBlur.hpp"
#include "rayTracer/CPUID.hpp"

namespace rayTracer {

void test1() {
}

void testOpenMP() {

	#if RayTracerConfig_ParallelProcessor_OpenMP


	#if 0
	{
		//UNUSUAL
		//parallel region: every thread executes this once

		#pragma omp parallel
		{ 
			String::debug(L"Hello World\n");
		}
	}
	#endif
	
	#if 0
	{
		//BAD
		//counter not controlled

		#pragma omp parallel
		{ 
			#pragma omp for
			for (int i = 0; i < 4; ++i)
				String::debug(L"Hello World#%i - t%i\n", i, omp_get_thread_num());
		}
	}
	#endif



	#if 0
	{
		//GOOD
		//counter controlled, correct behavior

		#pragma omp parallel
		{ 
			int i;
			#pragma omp for private(i)
			for (i = 0; i < 4; ++i)
				String::debug(L"Hello World#%i - t%i\n", i, omp_get_thread_num());
		}
	}
	#endif

	#if 0
	{
		//BAD
		//j value dependws on previous iterations

		#pragma omp parallel
		{ 
			int i;
			int j = 0;
			#pragma omp for private(i)
			for (i = 0; i < 50; ++i) {

				++j;

				String::debug(L"Hello World#%i - %i - t%i\n", i, j, omp_get_thread_num());
			}
		}
	}
	#endif

	#if 0
	{
		//GOOD
		//counter controlled, correct behavior

		#pragma omp parallel
		{ 
			int i;
			#pragma omp for private(i)
			for (i = 0; i < 100; ++i) {

				String::debug(L"Hello World#%i - t%i\n", i, omp_get_thread_num());
			}
		}
	}
	#endif

	#if 1
	{
		#pragma omp parallel
		{ 
			int j;

			#pragma omp for private(j)
			for (int i = 0; i < 10; ++i) {

				for (j = 0; j < 10; ++j) {

					#pragma omp critical 
					String::debug(L"Hello World#%i,%i - t%i\n", i, j, omp_get_thread_num());
				}
			}
		}
	}
	#endif

	#endif
}

void createScene1() {

	#ifdef RayTracerConfig_SSE
	{
		if (!getCPUID().SSE || !getCPUID().SSE2)
			return;
	}
	#endif

	bool isLowQuality = false;

#ifdef _DEBUG
	isLowQuality = true;
#endif

	//isLowQuality = true;
	
	Scene scene;
	Clock clock;
	Camera camera;

	scene.initSimple(&clock);

	if (isLowQuality)
		camera.initBasic(scene, 2.0f, degToRad(90.0f), 320, 240);
	else
		camera.initBasic(scene, 2.0f, degToRad(90.0f), 640, 480);


	//scene.appendScene1();
	//scene.appendScene2();
	//scene.appendScene3();
	//scene.appendScene_wavefrontObj("../../media/test.obj", &camera);
	//scene.appendScene_wavefrontObj("../../media/cornell_box.obj", &camera);
	//scene.appendScene_wavefrontObj("../../RayTracer_v2/media/test_cornell_box.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/ducky.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/dogobj.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/mechanic.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/old_boat.obj", &camera, true, true, true);
	scene.appendScene_wavefrontObj("../../media/bunny.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/dragon.obj", &camera, true, true, true);
	//scene.appendScene_wavefrontObj("../../media/buddha.obj", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/kitchenTransformed.ra2", "../../media/kitchenTransformed.txt", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/MengerSponge_scaled_cubes.ra2", "../../media/MengerSponge_scaled_cubes.txt", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/clown.ra2", "../../media/clown.txt", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/happy_buddha_vrip.ra2", "../../media/happy_buddha_vrip.txt", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/scene6.ra2", "../../media/scene6.txt", &camera, true, true, true);
	//scene.appendScene_ra2("../../media/FairyForestF160.ra2", "../../media/FairyForestF160.txt", &camera, true, true, true);


	scene.finalizeScene(true, 16);
	//scene.finalizeScene(true, 30);

	//camera.worldTransform().setPosition(Vector3(scene.coordSys(), 0.0f, 3.0f, 8.0f));
	
	RenderImage glowBuffer;
	RenderImage renderImg;
	RenderWindow_PixelToaster renderWindow;
	VirtualRenderTarget renderTarget;

	renderImg.setFlushFilePath(L"", L"render", L".tga", false);

	renderTarget.addWeak(&renderImg);
	renderTarget.addWeak(&renderWindow);

	RenderParams renderParams;
	RenderParams previewParams;

	renderParams.backColor = RenderColor::kBluish1;
	renderParams.superSampleFactor = isLowQuality ? 0 : 1;
	renderParams.diffuseMode = DiffuseMode_Lambert;
	renderParams.shadowMode = ShadowMode_Hard;
	renderParams.specularMode = SpecularMode_BlinnPhong;
	renderParams.ambient.set(0.2f);
	renderParams.useSelfTolerance = false; //true; 
	//renderParams.exposureMode = ExposureMode_TestGlow;
	//renderParams.exposureMode = ExposureMode_Exp;
	//renderParams.exposure = -2.0f;
	//renderParams.exposure = -3.0f;
		
	/*
	{
		renderParams.superSampleFactor = 0;
		renderParams.diffuseMode = DiffuseMode_Flat;
		renderParams.shadowMode = ShadowMode_None;
		renderParams.specularMode = SpecularMode_None;
		renderParams.ambient.set(0.2f);
		renderParams.useSelfTolerance = false; //true; 
	}
	*/
		
	previewParams = renderParams;

	previewParams.backColor.zero();
	previewParams.superSampleFactor = 0;
	previewParams.diffuseMode = DiffuseMode_Flat;
	previewParams.shadowMode = ShadowMode_None;
	previewParams.specularMode = SpecularMode_None;
	previewParams.pixelSpacing = 12;
	previewParams.exposureMode = ExposureMode_None;
	previewParams.refractionMode = RefractionMode_None;
	previewParams.maxRayDepth = 1;

	//renderParams = previewParams;
	//renderParams.pixelSpacing = 0;
	
	int renderJobCount = 1;

	#if !RayTracerConfig_EnableParallelProcessing

		ParallelProcessor parallelProcessor;

	#elif RayTracerConfig_ParallelProcessor_Simple

		ParallelProcessor parallelProcessor;
		parallelProcessor.create(ParallelProcessor::ThreadPriority_Normal, renderJobCount > 1 ? renderJobCount : 0, false);

	#elif RayTracerConfig_ParallelProcessor_OpenMP

		ParallelProcessor parallelProcessor;

	#endif

	//camera.moveSpeed() = 0.2f;
	renderWindow.setManipCamera(&camera);

	clock.start();
	bool lastRenderWasPreview = false;
	bool usePreviewParams = true;
	int renderCount = 0;
	WE::Time lastEventTime = 0.0f;

	Log log;
	log.init(LOG_MASK_ALL, L"", L"log.txt", LOG_FORCE, LOG_ENABLED_BIT | LOG_LOG_TOCONSOLE_BIT | LOG_LOG_TOFILE_BIT | LOG_LOG_AUTOFLUSH_BIT);

	//ray traversal continue here
	//http://www.mpi-inf.mpg.de/~wald/PhD/

	#if RayTracerConfig_EnableParallelProcessing

		int numThreads = 0;	

		#pragma omp parallel
		{
			numThreads = omp_get_max_threads();	
		}

		#if RayTracerConfig_ParallelProcessor_OpenMP
			log.log(L"OpenMp: %ixprocessors, %ixthreads\n", omp_get_num_procs(), numThreads);
		#endif

		renderJobCount = numThreads * 2;

	#endif

	bool doAnim = false;
	bool doTakeCameToAnimPos = false;
	int camAnimPosIndex = 3;

	if (doAnim || doTakeCameToAnimPos) {

		renderImg.setFlushFilePath(L"", L"render", L".tga", true);

		Vector3 sphericalCoord;
		Vector3 pos;
		Vector3 lookAt;
		Vector3 up;

		up.zero();
		up.el[Scene_Up] = 1.0f;

		lookAt.zero();
		lookAt.el[Scene_Up] = 0.1f;
		lookAt.el[Scene_Right] = 1.0f;
		lookAt.el[Scene_Up] = 1.0f;
		lookAt.el[Scene_Forward] = 1.0f;
		

		float radius = 15.0f;
		Vector3 rotationCenter = lookAt;
		rotationCenter.el[Scene_Up] += 1.0f;

		sphericalCoord.el[Ro] = radius;
		sphericalCoord.el[Phi] = kPiOver2;
		sphericalCoord.el[The] = 0.0f;

		Renderer renderer;

		float angleInc = 0.5f;
		int iterCount = (int) (360.0f / angleInc);

		float angle = 0.0f;
		for (int i = 0; i < iterCount; ++i, angle += angleInc) {

			if (doAnim) {
				
				log.log(L"render: %d/%d (%s)", i, iterCount, renderImg.getFullFlushPath().c_tstr());
			}

			sphericalCoord.el[The] = degToRad(angle);

			sphericalCoord.sphericalToCartesian(pos);

			pos.add(rotationCenter);

			camera.worldTransform().identity();
			camera.worldTransform().setPosition(pos);
			camera.worldTransform().setOrientation(pos, lookAt, up);
			camera.markDirty();

			if (!doAnim && (doTakeCameToAnimPos && i == camAnimPosIndex)) {

				break;
			}
			
			if (doAnim) {

				renderer.renderStats().reset();
				renderer.render(clock, scene, camera, renderImg, &glowBuffer, renderParams, renderJobCount, &parallelProcessor);

				log.log(L"%u rays, %f secs\n", renderer.renderStats().raysShot, renderer.renderStats().getRenderTime());
			}
		}

		renderImg.setFlushFilePath(L"", L"render", L".tga", false);

		if (!doTakeCameToAnimPos) {
			
			return;
		}
	}
	

	bool doTestBlur = false;
	if (doTestBlur)
	{
		RenderImage srcImg;
		RenderImage blurImg;

		if (srcImg.load(L"glow.tga")) {

			blurImg.create(srcImg.getWidth(), srcImg.getHeight());
			GaussianBlur::transform(clock, srcImg, blurImg);

			blurImg.write(L"blurGlow.tga");
		}

		return;
	}

	{
		Renderer renderer;
		renderWindow.setSourceRenderer(&renderer);

		while (true) {

			Sleep(1);
			clock.startFrame();

			if (camera.isDirty() || (lastRenderWasPreview && !usePreviewParams)) {

				log.log(L"render: ");

				renderer.renderStats().reset();
				RenderParams& params = usePreviewParams ? previewParams : renderParams;
				renderer.render(clock, scene, camera, renderTarget, &glowBuffer, params, renderJobCount, &parallelProcessor);
				
				if (params.exposureMode == ExposureMode_TestGlow)
					glowBuffer.write(L"glow.tga");

				log.log(L"%u rays, %f secs, %f rays/sec\n", renderer.renderStats().raysShot, renderer.renderStats().getRenderTime(), ((float) renderer.renderStats().raysShot / renderer.renderStats().getRenderTime()));
				
				lastRenderWasPreview = usePreviewParams;
				++renderCount;

				camera.resetDirty();
			} 

			if (!renderWindow.update())
				break;

			if (camera.isDirty()) {

				lastEventTime = clock.getTime();
				usePreviewParams = true;

			} else {

				usePreviewParams = (clock.getTime() - lastEventTime < 1.5f);
			}
					
			clock.endFrame();
		}
	}
}

}


int main( int argc, const char* argv[] )
{
	//rayTracer::testOpenMP();
	rayTracer::createScene1();
	

	return 0;
}