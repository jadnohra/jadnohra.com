#ifndef _RayTracer_Scene_hpp
#define _RayTracer_Scene_hpp

#include "../WE3/math/WEAAB.h"
#include "Renderer.hpp"
#include "SpatialAccelerator.hpp"

namespace rayTracer {

	class Camera;

	class Scene {
	public:

		Scene();

		inline bool isRotationLeftHanded() const { return mCoordSysBase.rotationLeftHanded == 1; }
		inline const FastUnitCoordSys& coordSys() const { return mCoordSys; }

		inline Primitives& primitives() { return mPrimitives; }
		inline Primitives& lights() { return mLights; }

		inline RayTolerance& rayTolerance() { return mRayTolerance; }
		inline const RayTolerance& getRayTolerance() { return mRayTolerance; }

		inline RayTolerance& selfRayTolerance() { return mSelfRayTolerance; }
		inline const RayTolerance& getSelfRayTolerance() { return mSelfRayTolerance; }

		inline const bool& hasAmbient() { return mHasAmbient; }
		inline const Vector3& getAmbient() { return mAmbient; }

		void initSimple(Clock* pClock = NULL);
	
		Clock* getClock() { return mClock; }

		void destroyScene();
		void appendScene1();
		void appendScene2();
		void appendScene3();

		void appendScene_wavefrontObj(const char* filePath, Camera* pCamera, bool autoPlaceCamIfNeeded, bool autoPlaceLightIfNeeded, bool autoGenerateAmbientIfNeeded);
		void appendScene_ra2(const char* filePath, const char* extFilePath, Camera* pCamera, bool autoPlaceCamIfNeeded, bool autoPlaceLightIfNeeded, bool autoGenerateAmbientIfNeeded);
	
		void finalizeScene(bool enableAcceleration, int hintMaxLeafPrimCount);

	public:

		void declareVertices(unsigned int count);
		Vertex& newVertex();

		inline Vertex& declareAndNewVertex() { declareVertices(1); return newVertex(); }
		
		inline const unsigned int& getTriangleCount() const { return mTriangleCount; }
		inline const Triangle& getTriangle(const unsigned int& index) { return mTriangles[index]; }

		void declareTriangles(unsigned int count);
		Triangle& newTriangle();
		inline Triangle& declareAndNewTriangle() { declareTriangles(1); return newTriangle(); }

		inline const AAB& getBoundingVolume() const { return mSceneAAB; }

	public:

		bool shootRay(const RayShot& rayShot, RayShotHit& result);
		unsigned int debugShootRay(const RayShot& rayShot);

		#ifdef RayTracerConfig_EnablePackets
			bool shootRayPacket(const Constants& cts, const PacketRayShot& rayPacket, PacketRayHit& t);
		#endif

	protected:

		typedef WETL::SizeAllocT<Vertex, unsigned int, true> Vertices;
		typedef WETL::SizeAllocT<Triangle, unsigned int, true> Triangles;

	protected:

		CoordSys mCoordSysBase;
		FastUnitCoordSys mCoordSys;

		Primitives mPrimitives;
		Primitives mLights;
		Materials mMaterials;
		Material mDefaultMaterial;

		Vertices mVertices;
		Triangles mTriangles;

		unsigned int mVertexCount;
		unsigned int mTriangleCount;

		RayTolerance mRayTolerance;
		RayTolerance mSelfRayTolerance;

		SoftPtr<Clock> mClock;

		bool mHasAmbient;
		Vector3 mAmbient;

		AAB mSceneAAB;

		HardPtr<SpatialAccelerator> mSpatialAccelerator;
	};
}

#endif