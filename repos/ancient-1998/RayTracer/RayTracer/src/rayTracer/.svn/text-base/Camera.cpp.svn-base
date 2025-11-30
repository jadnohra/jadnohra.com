#include "Camera.hpp"
#include "Scene.hpp"
#include "Math.hpp"

namespace rayTracer {

Camera::Camera() {

	mIsDirty = true;
}

void Camera::initBasic(Scene& scene, float moveSpeed, float rotSpeed, unsigned int windowWidth, unsigned int windowHeight) {

	mScene = &scene;

	mWindowSize[Render_Right] = (float) windowWidth;
	mWindowSize[Render_Up] = (float) windowHeight;

	initProjection(scene);

	mCamWorldTransform.identity();
	mCamWorldTransform.setPosition(Vector3(scene.coordSys(), 0.0f, 3.0f, -10.0f));

	mMoveSpeed = moveSpeed;
	mRotSpeed = rotSpeed;

	markDirty();
}

void Camera::initProjection(Scene& scene, const float& nearZ, const float& farZ, const float& fovDeg) { 

	mNearZ = nearZ;	
	mFarZ = farZ;	

	mProjMatrix.setupPerspectiveFov(degToRad(fovDeg), (float) mWindowSize[0] / (float) mWindowSize[1], nearZ, farZ, scene.isRotationLeftHanded());
	inverse(getProjMatrix(), mInvProjMatrix);
}

void Camera::markDirty() { 
	
	mIsDirty = true; 

	inverse(getWorldTransform(), mViewMatrix);
}

void Camera::pixelToViewport(const int& pixelX, const int& pixelY, float& viewportRight, float& viewportUp) {

	viewportRight = 2.0f * ((pixelX / mWindowSize[0]) - 0.5f);
	viewportUp = -2.0f * ((pixelY / mWindowSize[1]) - 0.5f);
}

void Camera::pixelToViewport(const float& pixelX, const float& pixelY, float& viewportRight, float& viewportUp) {

	viewportRight = 2.0f * ((pixelX / mWindowSize[0]) - 0.5f);
	viewportUp = -2.0f * ((pixelY / mWindowSize[1]) - 0.5f);
}

void Camera::getRayDirForPixel(const float& viewportRight, const float& viewportUp, Scene& scene, Vector3& nonNormalizeDir) {

	Vector3 pixelPosProjSpace;
	Vector3 pixelPosCamSpace;
	Vector3 pixelPosWorldSpace;

	scene.coordSys().setRUF(viewportRight, viewportUp, 0.0f, pixelPosProjSpace.el);

	float w = 1.0f;
	mInvProjMatrix.transformPoint(pixelPosProjSpace, pixelPosCamSpace, w);
	pixelPosCamSpace.div(w);

	mCamWorldTransform.transformPoint(pixelPosCamSpace, pixelPosWorldSpace);
	pixelPosWorldSpace.subtract(mCamWorldTransform.getPosition(), nonNormalizeDir);
}

void Camera::setupRayForPixel(const float& viewportRight, const float& viewportUp, Scene& scene, Ray& ray) {

	Vector3 pixelPosProjSpace;
	Vector3 pixelPosCamSpace;
	Vector3 pixelPosWorldSpace;

	scene.coordSys().setRUF(viewportRight, viewportUp, 0.0f, pixelPosProjSpace.el);

	float w = 1.0f;
	mInvProjMatrix.transformPoint(pixelPosProjSpace, pixelPosCamSpace, w);
	pixelPosCamSpace.div(w);

	mCamWorldTransform.transformPoint(pixelPosCamSpace, pixelPosWorldSpace);
	ray.setupByTarget(mCamWorldTransform.getPosition(), pixelPosWorldSpace);
}

#ifdef RayTracerConfig_EnablePackets

void Camera::getRaysForPacket(const Constants& cts, const int pixelX[4], const int pixelY[4], Scene& scene, Packet4_3& packetRayDirs) {

	float temp;
	Packet4_3 temp4_1;
	Packet4_3 temp4_2;
	
	Packet4_3& pixelPosProjSpace = temp4_1;

	temp = (2.0f / mWindowSize[0]);
	pixelPosProjSpace[Scene_Right] =  (_mm_set_ps((temp * (float) pixelX[3]) - 1.0f, (temp * (float) pixelX[2]) - 1.0f, (temp * (float) pixelX[1]) - 1.0f, (temp * (float) pixelX[0]) - 1.0f));
	temp = (-2.0f / mWindowSize[1]);
	pixelPosProjSpace[Scene_Up] = (_mm_set_ps((temp * (float) pixelY[3]) + 1.0f, (temp * (float) pixelY[2]) + 1.0f, (temp * (float) pixelY[1]) + 1.0f, (temp * (float) pixelY[0]) + 1.0f));
	pixelPosProjSpace[Scene_Forward] = cts.zero4;

	Packet4 w = cts.one4;

	Packet4_3& pixelPosCamSpace = temp4_2;

	_mm_transform(mInvProjMatrix, pixelPosProjSpace, pixelPosCamSpace, w);
	
	//w = _mm_safercp(w);
	w = _mm_div_ps(cts.one4, w); 
	pixelPosCamSpace.x4 = _mm_mul_ps(pixelPosCamSpace.x4, w);
	pixelPosCamSpace.y4 = _mm_mul_ps(pixelPosCamSpace.y4, w);
	pixelPosCamSpace.z4 = _mm_mul_ps(pixelPosCamSpace.z4, w);

	Packet4_3& pixelPosWorldSpace = temp4_1;

	_mm_transform(mCamWorldTransform, pixelPosCamSpace, pixelPosWorldSpace);

	packetRayDirs.x4 = _mm_sub_ps(pixelPosWorldSpace.x4, _mm_set1_ps(mCamWorldTransform.getPosition().x));
	packetRayDirs.y4 = _mm_sub_ps(pixelPosWorldSpace.y4, _mm_set1_ps(mCamWorldTransform.getPosition().y));
	packetRayDirs.z4 = _mm_sub_ps(pixelPosWorldSpace.z4, _mm_set1_ps(mCamWorldTransform.getPosition().z));
}

#endif

}