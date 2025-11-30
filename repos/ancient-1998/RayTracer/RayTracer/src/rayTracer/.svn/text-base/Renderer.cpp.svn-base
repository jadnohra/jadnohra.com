#include "Renderer.hpp"
#include "Scene.hpp"
#include "PostProcess.hpp"

namespace rayTracer {

Renderer::Renderer() {
}

void Renderer::render(Clock& clock, Scene& scene, Camera& camera, RenderTarget& target, RenderTarget* pGlowTarget, const RenderParams& renderParams, int jobCount, ParallelProcessor* pParallelProcessor) {

	mRenderParams = renderParams;
	renderStats().reset();
	renderStats().startTime = clock.getRawTime();

	mRenderContext.scene = &scene;
	mRenderContext.camera = &camera;
	mRenderContext.target = &target;
	mRenderContext.glowTarget = pGlowTarget;
	mRenderContext.clock = &clock;

	mRenderContext.viewMatrix = camera.getViewMatrix();
	mRenderContext.invProjMatrix = camera.getInvProjMatrix();

	//inverse(camera.getWorldTransform(), mRenderContext.viewMatrix);
	//inverse(camera.getProjMatrix(), mRenderContext.invProjMatrix);

	mRenderContext.windowSizeX = (unsigned int) camera.getWindowSize()[Render_Right];
	mRenderContext.windowSizeY = (unsigned int) camera.getWindowSize()[Render_Up];

	target.create(mRenderContext.windowSizeX, mRenderContext.windowSizeY);

	if (pGlowTarget && renderParams.exposureMode == ExposureMode_TestGlow) {

		pGlowTarget->create(mRenderContext.windowSizeX, mRenderContext.windowSizeY);
		pGlowTarget->fill(0.0f, 0.0f, 0.0f, 0.0f);
	}

	mRenderContext.backColor.zero();
	mRenderContext.backColor = renderParams.backColor;
	
	unsigned int superSampleCount = powf(2.0f, renderParams.superSampleFactor);
	superSampleCount *= superSampleCount;
	
	mRenderContext.dy = 1.0f / ((float) (mRenderContext.windowSizeY));
	mRenderContext.dx = 1.0f / ((float) (mRenderContext.windowSizeX));
	mRenderContext.sampleCountY = (int) sqrtf((float) superSampleCount);
	mRenderContext.sampleCountX = mRenderContext.sampleCountY;
	mRenderContext.sampleSpacingY = 1.0f / ((float) (mRenderContext.sampleCountY));
	mRenderContext.sampleSpacingX = 1.0f / ((float) (mRenderContext.sampleCountX));
	mRenderContext.startSampleOffsetY = mRenderContext.sampleCountY <= 1 ? 0.0f : -1.0f + mRenderContext.sampleSpacingY * 0.5f;
	mRenderContext.startSampleOffsetX = mRenderContext.sampleCountX <= 1 ? 0.0f : -1.0f + mRenderContext.sampleSpacingX * 0.5f;

	superSampleCount = mRenderContext.sampleCountX * mRenderContext.sampleCountY;
	mRenderContext.sdy = mRenderContext.dy / (float) std::max(1, mRenderContext.sampleCountY - 1);
	mRenderContext.sdx = mRenderContext.dx / (float) std::max(1, mRenderContext.sampleCountX - 1);
	mRenderContext.sampleWeight = 1.0f / (float) (superSampleCount);

	mRenderContext.ambient = scene.hasAmbient() ? scene.getAmbient() : renderParams.ambient;

	if (mRenderParams.pixelSpacing < 1)
		mRenderParams.pixelSpacing = 1;

	if (mRenderParams.pixelSpacing > 1) {

		target.fill(mRenderContext.backColor.el[0], mRenderContext.backColor.el[1], mRenderContext.backColor.el[2], 0.0f);
	}

	#if (!RayTracerConfig_EnableParallelProcessing)
	{
		pParallelProcessor = NULL;
	} 
	#endif

	RenderBlock sourceBlock;

	sourceBlock.startPixelX = 0;
	sourceBlock.endPixelX = mRenderContext.windowSizeX;

	sourceBlock.startPixelY = 0;
	sourceBlock.endPixelY = mRenderContext.windowSizeY;

	sourceBlock.pixelSpacing = mRenderParams.pixelSpacing;
	sourceBlock.renderer = this;

	if (jobCount == 1 || (pParallelProcessor == NULL)) {

		sourceBlock.execute();

	} else {

		mBlockManager.genBlocks(dref(this), sourceBlock, jobCount);

		#if RayTracerConfig_ParallelProcessor_OpenMP
		
			int blockCount = mBlockManager.getBlockCount();

			#pragma omp parallel
			//#pragma omp for schedule(static)
			#pragma omp for schedule(dynamic)
			for (int bi = 0; bi < blockCount; ++bi) {

				mBlockManager.getBlock(bi).execute();
			}

			for (int bi = 0; bi < blockCount; ++bi) {

				renderStats().add(mBlockManager.getBlock(bi).stats);
			}
			
		#endif
	}

	renderStats().renderEndTime = clock.getRawTime();

	target.flush();
	if (pGlowTarget && renderParams.exposureMode == ExposureMode_TestGlow) 
		pGlowTarget->flush();

	PostProcess::finalize(clock, renderParams, target, pGlowTarget);

	renderStats().endTime = clock.getRawTime();
}

void Renderer::RenderBlockManager::genBlocks(Renderer& renderer, const RenderBlock& source, int blockCount) {

	int blockCountX = (int) sqrtf((float) blockCount);
	int blockCountY = blockCount / blockCountX;

	blockCount = blockCountX * blockCountY;

	int blockSizeX = renderer.renderContext().windowSizeX / blockCountX;
	int blockRestSizeX = renderer.renderContext().windowSizeX - (blockSizeX * blockCountX);

	int blockSizeY = renderer.renderContext().windowSizeY / blockCountY;
	int blockRestSizeY = renderer.renderContext().windowSizeY - (blockSizeY * blockCountY);

	mBlocks.count = 0;
	mBlocks.reserveCount(blockCount);

	for (int blockY = 0; blockY < blockCountY; ++blockY) {

		int startPixelY = blockY * blockSizeY;
		int endPixelY = (blockY + 1) * blockSizeY;

		if (blockY + 1 == blockCountY)
			endPixelY += blockRestSizeY;

		for (int blockX = 0; blockX < blockCountX; ++blockX) {

			RenderBlock& renderBlock = mBlocks.addOneReserved();

			renderBlock.startPixelY = startPixelY;
			renderBlock.endPixelY = endPixelY;

			renderBlock.startPixelX = blockX * blockSizeX;
			renderBlock.endPixelX = (blockX + 1) * blockSizeX;

			if (blockX + 1 == blockCountX)
				renderBlock.endPixelX += blockRestSizeX;

			renderBlock.pixelSpacing = renderer.renderParams().pixelSpacing;
			renderBlock.renderer = &renderer;
		}
	}
}

Renderer::RenderBlock::~RenderBlock() {

	if (renderer.isValid()) {

		renderContext().stats.add(stats);
	}

	/*
	{
		String::debug(L"%u rays, %f secs.\n", stats.raysShot, stats.getRenderTime());
	}
	*/
}



void Renderer::RenderBlock::execute() {

	stats.startTime = renderContext().clock->getRawTime();

	Renderer& renderer = this->renderer;
	Scene& scene = renderer.renderContext().scene;
	Camera& camera = renderer.renderContext().camera;
	RenderTarget& target = renderer.renderContext().target;

	Matrix4x3& viewMatrix = renderer.renderContext().viewMatrix;
	Matrix4x4& invProjMatrix = renderer.renderContext().invProjMatrix;

	unsigned int pixelIndex = 0;
	bool doTestPackets = false;

	#ifdef RayTracerConfig_EnablePackets
		doTestPackets = pixelSpacing <= 1;
	#endif
	

	#ifdef RayTracerConfig_EnablePackets
	if (doTestPackets) {

		const Constants cts;

		const int packetSizeY = 2;
		const int packetSizeX = 2;

		for (int pixelY = startPixelY; pixelY < endPixelY; pixelY += packetSizeY) {
		
			for (int pixelX = startPixelX; pixelX < endPixelX; pixelX += packetSizeX) {

				if (pixelX == 210 && pixelY == 64) {

					int x = 0;

				} else {

					//continue;
				}

				PacketRayShot rayPacket(NULL, scene.getRayTolerance());

				const int pixelY4[4] = { pixelY, pixelY, pixelY + 1, pixelY + 1 };
				const int pixelX4[4] = { pixelX, pixelX + 1, pixelX, pixelX + 1 };

				float viewportRight;
				float viewportUp;
				Vector3 rayDir;
		
				rayPacket.setRayOrigin4(camera.getCommonRayOrigin());
				camera.getRaysForPacket(cts, pixelX4, pixelY4, scene, rayPacket.ray4_direction);

				/*
				for (int i = 0; i < 4; ++i) {

					camera.pixelToViewport(pixelX4[i], pixelY4[i], viewportRight, viewportUp);
					camera.getRayDirForPixel(viewportRight, viewportUp, renderer.renderContext().scene, rayDir);

					rayPacket.setRayDir(i, rayDir);
				}
				*/
								
				rayPacket.prepareRays(cts);
			
				if (false)
				{
					Ray refRay;

					{
						camera.pixelToViewport(pixelX, pixelY, viewportRight, viewportUp);
						camera.setupRayForPixel(viewportRight, viewportUp, renderer.renderContext().scene, refRay);
					}

					if (true)
					{
						RayShot rayShot(refRay, NULL, scene.getRayTolerance());
						RayShotHit rayHit;
						
						rayHit.reset();

						if (scene.shootRay(rayShot, rayHit)) {

							int x = 0;
						}
					}

					if (false)
					{
						Primitive_Triangle& testTri = dref((Primitive_Triangle*) scene.primitives()[0]);

						RayShot refRayShot(refRay, NULL, scene.getRayTolerance());
						RayIntersectionT refRayIntersT;
						refRayIntersT.reset();
						if (testTri.intersects(refRayShot, refRayIntersT)) {

							int x = 0;
						}
					}
				}
				
				PacketRayHit rayPacketHit;

				rayPacketHit.reset(cts);
				stats.raysShot += 4;
				scene.shootRayPacket(cts, rayPacket, rayPacketHit);
				
				{
					for (int i = 0; i < 4; ++i) {

						Vector3 sampleColor;
						
						unsigned int primID = rayPacketHit.pid.eli[i];
						
						//rayPacketHit.t.el[i] == cts.infinity
						if (primID != -1) {

							Primitive& prim = dref(scene.primitives()[primID]);
							sampleColor = prim.material().color();

						} else {

							sampleColor = renderer.renderContext().backColor;
						}

						target.setPixel(pixelX4[i], pixelY4[i], 
							sampleColor.el[0], 
							sampleColor.el[1], 
							sampleColor.el[2], 0.0f);
					}
				}
			}
		}

	} else 
	#else
	{

		for (int pixelY = startPixelY; pixelY < endPixelY; pixelY += pixelSpacing) {
		
			for (int pixelX = startPixelX; pixelX < endPixelX; pixelX += pixelSpacing) {
				
				
				if (pixelX == 154 && pixelY == 49) {

					int x = 0;

				} else {

					//continue;
				}

				if (pixelX == 170 && pixelY == 82) {

					int x = 0;

				} else {

					//continue;
				}

				if (pixelX == 160 && pixelY == 30) {

					int x = 0;

				} else {

					//continue;
				}

				if (pixelX == 135 && pixelY == 87) {

					int x = 0;

				} else {

					//continue;
				}
				
				
				Vector3 pixelColor = Vector3::kZero;
				int sampleIndex = 0;

				//float uBase = -2.0f * ((float) pixelY - renderer.renderContext().windowSizeY / 2) / (float) (renderer.renderContext().windowSizeY);
				//float uStart = uBase - (0.5f * renderer.renderContext().dy);
				//float u = uStart;

				for (int sampleIndexY = 0; sampleIndexY < renderer.renderContext().sampleCountY; ++sampleIndexY) {

					//float rBase = 2.0f * ((float) pixelX - renderer.renderContext().windowSizeX / 2) / (float) (renderer.renderContext().windowSizeX);
					//float rStart = rBase - (0.5f * renderer.renderContext().dx);
					//float r = rStart;

					float pixelSampleY = (((float) pixelY) + renderer.renderContext().startSampleOffsetY) + (((float) sampleIndexY) * renderer.renderContext().sampleSpacingY);

					for (int sampleIndexX = 0; sampleIndexX < renderer.renderContext().sampleCountX; ++sampleIndexX) {

						float pixelSampleX = (((float) pixelX) + renderer.renderContext().startSampleOffsetX) + (((float) sampleIndexX) * renderer.renderContext().sampleSpacingX);

						float viewportRight;
						float viewportUp;
		
						camera.pixelToViewport(pixelSampleX, pixelSampleY, viewportRight, viewportUp);

						Ray ray;
						//camera.setupRayForPixel(r, u, renderer.renderContext().scene, ray);
						camera.setupRayForPixel(viewportRight, viewportUp, renderer.renderContext().scene, ray);

						Vector3 sampleColor;

						#ifdef RayTracerConfig_SpatialAccel_Debug
						{
							RayShot rayShot(ray, NULL, scene.getRayTolerance());


							bumpRayShotCounter();
							unsigned int hitCount = scene.debugShootRay(rayShot);

							static const Vector3 debugColor = RayTracerConfig_SpatialAccel_Debug_Color;

							sampleColor = renderer.renderContext().backColor;
							debugColor.mulAndAdd((float) (hitCount) * RayTracerConfig_SpatialAccel_Debug_Alpha, sampleColor);
						}
						#else

							if (!gatherRayColor(scene, ray, sampleColor, NULL, 0, renderer.renderParams().maxRayDepth)) {

								sampleColor = renderer.renderContext().backColor;
							}

						#endif
				

						Vector3 weightedColor;
						sampleColor.mul(renderer.renderContext().sampleWeight, weightedColor);
						pixelColor.add(weightedColor);

						++sampleIndex;

						//r += renderer.renderContext().sdx;
					}

					//u += renderer.renderContext().sdy;
				}

				{
					applyExposure(pixelColor, pixelX, pixelY);

					target.setPixel(pixelX, pixelY, 
							pixelColor.el[0], 
							pixelColor.el[1], 
							pixelColor.el[2], 0.0f);
					
					++pixelIndex;
				}
			}
		}
	}
	#endif

	stats.renderEndTime = renderContext().clock->getRawTime();
	stats.endTime = stats.renderEndTime;
}

void Renderer::RenderBlock::applyExposure(Vector3& color, const int pixelX, const int pixelY) {

	switch (renderParams().exposureMode) {

		case ExposureMode_Exp: {

			color.el[0] = 1.0f - expf(color.el[0] * renderParams().exposure);
			color.el[1] = 1.0f - expf(color.el[1] * renderParams().exposure);
			color.el[2] = 1.0f - expf(color.el[2] * renderParams().exposure);

		} break;

		case ExposureMode_TestGlow: {

			bool hasGlow = false;
			Vector3 glow;

			if (color.el[0] > 1.0f) {

				if (!hasGlow)
					glow.zero();

				hasGlow = true;
				glow.el[0] = color.el[0] - 1.0f;
			}

			if (color.el[1] > 1.0f) {

				if (!hasGlow)
					glow.zero();

				hasGlow = true;
				glow.el[1] = color.el[1] - 1.0f;
			}

			if (color.el[2] > 1.0f) {

				if (!hasGlow)
					glow.zero();

				hasGlow = true;
				glow.el[2] = color.el[2] - 1.0f;
			}

			if (hasGlow) {

				color.capMax(1.0f);

				renderContext().glowTarget->setPixel(pixelX, pixelY, glow.el[0], glow.el[1], glow.el[2], 0.0f);
			}

		} break;

		default: {

			color.capMax(1.0f);

		} break;
	}
}

void Renderer::RenderBlock::bumpRayShotCounter() {

	//#if RayTracerConfig_ParallelProcessor_OpenMP
	//	#pragma omp atomic
	//#endif
	++stats.raysShot;
}

bool Renderer::RenderBlock::gatherRayColor(Scene& scene, const Ray& ray, Vector3& color, Primitive* pOriginPrimitive, int selfIntersectCount, int maxDepth, float restColorContribution) {

	if (maxDepth == 0)
		return false;

	if (restColorContribution < renderParams().minColorContribution) {

		return false;
	}

	RayShot rayShot(ray, pOriginPrimitive, scene.getRayTolerance());
	RayShotHit rayHit;

	bumpRayShotCounter();
	if (scene.shootRay(rayShot, rayHit)) {

		Primitive& prim = dref(scene.primitives()[rayHit.primID]);

		color.zero();

		rayHit.inters.t -= 0.0001f;
		rayHit.resolvePoint(dref(rayShot.ray));
		prim.resolveIntersectionInfo(rayShot, rayHit);

		//ambient
		if (prim.material().diffuse() > 0.0f)
		{
			//prim.material().color().mulAndAdd(renderParams().ambientCoeff, color);
			prim.material().color().compMul(renderContext().ambient, color);
		}

		//lighting
		{
			switch (renderParams().diffuseMode) {

				case DiffuseMode_Lambert: {

					Vector3 contribColor;

					gatherLambertDiffuse(prim, rayHit.intersInfo, scene, ray, contribColor);

					color.add(contribColor);
									
				} break;

				default: {

					prim.material().color().mulAndAdd(prim.material().diffuse(), color);

				} break;
			}
		}

		//reflections
		{
			Vector3 contribColor;

			gatherReflections(prim, rayHit.intersInfo, scene, ray, contribColor, maxDepth, restColorContribution);

			color.add(contribColor);
		}

		//refractions
		{
			Vector3 contribColor;

			gatherRefractions(pOriginPrimitive, prim, selfIntersectCount, rayHit.intersInfo, scene, ray, contribColor, maxDepth, restColorContribution);

			color.add(contribColor);
		}
		

		return true;
	}

	return false;
}

void Renderer::RenderBlock::gatherRefractions(Primitive* pSourcePrim, Primitive& prim, int selfIntersectCount, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color, int maxDepth, float restColorContribution) {

	if (renderParams().refractionMode == RefractionMode_None) {

		color.zero();
		return;
	}

	if (maxDepth == 0) {

		color.zero();
		return;
	}

	{
		if (prim.material().refraction() > 0.0f) {

			color.zero();

			bool isSelfIntersection = (&prim == pSourcePrim);

			if (isSelfIntersection)
				++selfIntersectCount;
			else
				selfIntersectCount = 0;

			bool isInsideOut = (isSelfIntersection && selfIntersectCount % 2 == 1);

			float refractRatio;

			if (isInsideOut) {

				refractRatio =  prim.material().refractionIndex() / renderParams().voidRefractionIndex;

			} else {

				refractRatio =  renderParams().voidRefractionIndex / prim.material().refractionIndex();
			}

			Vector3 normal = inters.normal;

			if (isInsideOut) {

				normal.negate();
			}
			
			float cosI = -normal.dot(incomingRay.direction);
			float cosT2 = 1.0f - refractRatio * refractRatio * (1.0f - cosI * cosI);

			if (cosT2 > 0.0f) {
			
				Ray refractRay;

				refractRay.origin = inters.point;
				incomingRay.direction.mul(refractRatio, refractRay.direction);
				normal.mulAndAdd(refractRatio * cosI - sqrtf(cosT2), refractRay.direction);
				refractRay.prepare();

				gatherRayColor(scene, refractRay, color, &prim, 
								selfIntersectCount, 
								maxDepth > 0 ? maxDepth - 1 : -1, restColorContribution * prim.material().refraction());
				color.mul(prim.material().refraction());
				color.compMul(prim.material().color());
			}

		} else {

			color.zero();
		}
	}
}

void Renderer::RenderBlock::gatherReflections(Primitive& prim, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color, int maxDepth, float restColorContribution) {

	if (maxDepth == 0) {

		color.zero();
		return;
	}

	{
		if (prim.material().reflection() > 0.0f) {

			color.zero();

			Ray reflectRay;

			inters.reflectRay(incomingRay.direction, reflectRay);
			
			gatherRayColor(scene, reflectRay, color, &prim, 0, maxDepth > 0 ? maxDepth - 1 : -1, restColorContribution * prim.material().reflection());
			color.mul(prim.material().reflection());
			color.compMul(prim.material().color());

		} else {

			color.zero();
		}
	}
}


void Renderer::RenderBlock::gatherLambertDiffuse(Primitive& prim, const RayIntersection& inters, Scene& scene, const Ray& incomingRay, Vector3& color) {

	static const bool splitDirToLightNormal = false;
	#ifdef _DEBUG
		static const bool verifyDirToLightIntersection = false;
	#else
		static const bool verifyDirToLightIntersection = false;
	#endif
	

	{
		bool enableSpecular = (renderParams().specularMode != SpecularMode_None && prim.material().specular() > 0.0f);

		if (prim.material().diffuse() > 0.0f || enableSpecular) {

			color.zero();

			for (int pi = 0; pi < scene.lights().count; ++pi) {

				Primitive& light = dref(scene.lights()[pi]);

				Vector3 dirToLight;
				Vector3 normalizedDirToLight;

				light.getPosition().subtract(inters.point, dirToLight);
						
				float dot = dirToLight.dot(inters.normal);

				if (dot > 0.0f) {

					float lightIntersT = dirToLight.mag();

					dirToLight.div(lightIntersT, normalizedDirToLight);
					
					if (!splitDirToLightNormal)
						dirToLight = normalizedDirToLight;

					float lightShade = 1.0f;

					if (renderParams().shadowMode != ShadowMode_None) {

						Ray rayToLight;

						rayToLight.origin = inters.point;
						rayToLight.direction = dirToLight;
						rayToLight.prepare();

						/*
						if (verifyDirToLightIntersection)
						{
							//test, the dirToLight normalization might break this!
							//that is why we use splitDirToLightNormal, might this overflow the intersection calculations?
							RayIntersectionT intersection;

							if (!light.intersects(rayToLight, intersection, scene.getRayTolerance(), false)) {

								assert(false);
								int x = 0;
							}
						}
						*/

						bumpRayShotCounter();
						RayShot rayShot(rayToLight, &prim, scene.getRayTolerance());
						RayShotHit rayHit;

						if (scene.shootRay(rayShot, rayHit) && rayHit.inters.t < lightIntersT) {

							lightShade = 0.0f;
						}
					}

					if (lightShade > 0.0f) {

						{
							Vector3 lightContributionDiffuseColor;

							dot = normalizedDirToLight.dot(inters.normal);

							prim.material().color().mul((dot * prim.material().diffuse() * lightShade), lightContributionDiffuseColor);
							lightContributionDiffuseColor.compMul(light.material().color());

							color.add(lightContributionDiffuseColor);
						}

						if (light.material().lightProducesSpecular()) {
						
							if (enableSpecular) {
								
								float specularDot;

								switch (renderParams().specularMode) {

									case SpecularMode_Phong: {

										Vector3 reflectDir;

										inters.reflectDir(incomingRay.direction, reflectDir);
										specularDot = reflectDir.dot(normalizedDirToLight);

									} break;

									default: {

										Vector3 halfway;

										dirToLight.subtract(incomingRay.direction, halfway);
										
										float mag;

										halfway.normalize(mag);

										if (mag != 0.0f) {

											specularDot = inters.normal.dot(halfway);

										} else {

											specularDot = 0;
										}

									} break;
								}

								if (specularDot > 0.0f) {
								
									Vector3 lightContributionSpecularColor;
									prim.material().color().mul(powf(dot, prim.material().specularPower()) * prim.material().specular() * lightShade, lightContributionSpecularColor);

									color.add(lightContributionSpecularColor);
								}
							}
						}
					}
				}
			}

		} else {

			color.zero();
		}
	}
}

}