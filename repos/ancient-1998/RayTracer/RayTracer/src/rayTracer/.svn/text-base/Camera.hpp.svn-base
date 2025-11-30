#ifndef _RayTracer_Camera_hpp
#define _RayTracer_Camera_hpp

#include "RenderTarget.hpp"
#include "Ray.hpp"

namespace rayTracer {

	class Scene;

	class Camera {
	public:

		Camera();
		void initBasic(Scene& scene, float moveSpeed, float rotSpeed, unsigned int windowWidth = 640, unsigned int windowHeight = 480);
		void initProjection(Scene& scene, const float& nearZ = 0.1f, const float& farZ = 100.0f, const float& fovDeg = 45.0f);

		const Matrix4x4& getProjMatrix() { return mProjMatrix; }
		const Vector2& getWindowSize() { return mWindowSize; }
		const SceneTransform& getWorldTransform() { return mCamWorldTransform; }
		SceneTransform& worldTransform() { return mCamWorldTransform; }

		const Matrix4x3& getViewMatrix() const { return mViewMatrix; }
		const Matrix4x4& getInvProjMatrix() const { return mInvProjMatrix; }

		inline Scene* scene() { return mScene; }
		inline float& moveSpeed() { return mMoveSpeed; }
		inline const float& getMoveSpeed() { return mMoveSpeed; }
		inline const float& getRotSpeed() { return mRotSpeed; }

		bool isDirty() { return mIsDirty; }
		void markDirty();
		void resetDirty() { mIsDirty = false; }

		void pixelToViewport(const float& pixelX, const float& pixelY, float& viewportRight, float& viewportUp);
		void pixelToViewport(const int& pixelX, const int& pixelY, float& viewportRight, float& viewportUp);
		void setupRayForPixel(const float& viewportRight, const float& viewportUp, Scene& scene, Ray& ray);
		
		void getRayDirForPixel(const float& viewportRight, const float& viewportUp, Scene& scene, Vector3& nonNormalizeDir);

		#ifdef RayTracerConfig_EnablePackets
			//non normalized
			void getRaysForPacket(const Constants& cts, const int pixelX[4], const int pixelY[4], Scene& scene, Packet4_3& packetRayDirs);
		#endif

		inline const Vector3& getCommonRayOrigin() {

			return mCamWorldTransform.getPosition();
		}

		inline const float& getNearZ() { return mNearZ; }
		inline const float& getFarZ() { return mFarZ; }

	protected:

		bool mIsDirty;
		SoftPtr<Scene> mScene;

		float mNearZ;
		float mFarZ;

		Matrix4x4 mProjMatrix;
		Vector2 mWindowSize;
		SceneTransform mCamWorldTransform;

		Matrix4x3 mViewMatrix;
		Matrix4x4 mInvProjMatrix;

		float mMoveSpeed;
		float mRotSpeed;
	};
}

#endif