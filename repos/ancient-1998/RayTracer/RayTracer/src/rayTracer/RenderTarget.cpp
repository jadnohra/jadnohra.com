#include "RenderTarget.hpp"

namespace rayTracer {

VirtualRenderTarget::~VirtualRenderTarget() {
}

void VirtualRenderTarget::add(RenderTarget* pTarget) {

	mTargets.addOne(pTarget);
}

void VirtualRenderTarget::addWeak(RenderTarget* pTarget) {

	mWeakTargets.addOne(pTarget);
}

bool VirtualRenderTarget::isValid() {

	for (int i = 0; i < mTargets.count; ++i) {

		if (!mTargets[i]->isValid())
			return false;
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		if (!mWeakTargets[i]->isValid())
			return false;
	}

	return true;
}

void VirtualRenderTarget::destroy() {

	mTargets.destroy();
}

unsigned int VirtualRenderTarget::getWidth() {

	for (int i = 0; i < mTargets.count; ++i) {

		return mTargets[i]->getWidth();
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		return mWeakTargets[i]->getWidth();
	}

	return 0;
}

unsigned int VirtualRenderTarget::getHeight() {

	for (int i = 0; i < mTargets.count; ++i) {

		return mTargets[i]->getHeight();
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		return mWeakTargets[i]->getHeight();
	}

	return 0;
}

bool VirtualRenderTarget::create(unsigned int width, unsigned int height) {

	bool success = true;

	for (int i = 0; i < mTargets.count; ++i) {

		if (!mTargets[i]->create(width, height)) {

			success = false;
		}
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		if (!mWeakTargets[i]->create(width, height)) {

			success = false;
		}
	}

	return success;
}

void VirtualRenderTarget::setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a) {

	for (int i = 0; i < mTargets.count; ++i) {

		mTargets[i]->setPixel(x, y, r, g, b, a);
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		mWeakTargets[i]->setPixel(x, y, r, g, b, a);
	}
}

void VirtualRenderTarget::getPixel(unsigned int x, unsigned int y, float& r, float& g, float& b, float& a) {

	if (mTargets.count) {

		mTargets[0]->getPixel(x, y, r, g, b, a);

	} else if (mWeakTargets.count) {

		mWeakTargets[0]->getPixel(x, y, r, g, b, a);
	}
}

void VirtualRenderTarget::fill(float r, float g, float b, float a) {

	for (int i = 0; i < mTargets.count; ++i) {

		mTargets[i]->fill(r, g, b, a);
	}

	for (int i = 0; i < mWeakTargets.count; ++i) {

		mWeakTargets[i]->fill(r, g, b, a);
	}
}

bool VirtualRenderTarget::flush() {

	bool success = true;

	for (int i = 0; i < mTargets.count; ++i) {

		if (!mTargets[i]->flush()) {

			success = false;
		}
	}

	
	for (int i = 0; i < mWeakTargets.count; ++i) {

		if (!mWeakTargets[i]->flush()) {

			success = false;
		}
	}

	return success;
}

}