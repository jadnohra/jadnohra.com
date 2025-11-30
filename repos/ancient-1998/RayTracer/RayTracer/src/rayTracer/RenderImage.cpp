#include "RenderImage.hpp"

namespace rayTracer {

RenderImage::RenderImage() : mAutoIncFileName(false), mFlushIndex(0), mAutoCapColorToRange(true) {
}

RenderImage::~RenderImage() {

	destroy();
}

bool RenderImage::isValid() {

	return mImage.isValid() == TRUE;
}

void RenderImage::destroy() {

	mImage.clear();
}

bool RenderImage::create(unsigned int width, unsigned int height) {

	if (isValid()) {

		destroy();
	}

	mImage.setSize(FIT_BITMAP, width, height, sizeof(int) * 8);

	return false;
}

unsigned int RenderImage::getWidth() {

	if (isValid()) {

		return mImage.getWidth();
	}

	return 0;
}

unsigned int RenderImage::getHeight() {

	if (isValid()) {

		return mImage.getHeight();
	}

	return 0;
}

void RenderImage::getPixel(unsigned int x, unsigned int y, float& r, float& g, float& b, float& a) {

	if (isValid()) {

		RGBQUAD rgb;

		mImage.getPixelColor(x, (mImage.getHeight() - y) - 1, &rgb);

		r = (float) rgb.rgbRed / 255.0f;
		g = (float) rgb.rgbGreen / 255.0f;
		b = (float) rgb.rgbBlue / 255.0f;
	}
}

void RenderImage::setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a) {

	if (isValid()) {

		RGBQUAD rgb;

		if (mAutoCapColorToRange) {

			rgb.rgbRed = (int) (std::min(1.0f, r) * 255.0f);
			rgb.rgbGreen = (int) (std::min(1.0f, g) * 255.0f);
			rgb.rgbBlue = (int) (std::min(1.0f, b) * 255.0f);

		} else {

			rgb.rgbRed = (int) (r * 255.0f);
			rgb.rgbGreen = (int) (g * 255.0f);
			rgb.rgbBlue = (int) (b * 255.0f);
		}

		mImage.setPixelColor(x, (mImage.getHeight() - y) - 1, &rgb);
	}
}

bool RenderImage::load(const TCHAR* filePath) {

	destroy();

	return mImage.loadU(filePath) == TRUE;
}

bool RenderImage::write(const TCHAR* filePath) {

	if (isValid()) {

		return mImage.saveU(filePath, 0) == TRUE;
	}

	return false;
}

void RenderImage::setFlushFilePath(const TCHAR* filePath, const TCHAR* fileName, const TCHAR* fileExt, bool autoIncFileName) {

	mFlushPath.assign(filePath);
	mFlushFile.assign(fileName);
	mFlushExt.assign(fileExt);

	mAutoIncFileName = autoIncFileName;

	updateFlushFullPath();
}

void RenderImage::updateFlushFullPath() {

	mFlushFullPath.assign(mFlushPath);
	mFlushFullPath.append(mFlushFile);
	
	if (mAutoIncFileName) {

		String indexStr;
		indexStr.assignEx(L"_%u", mFlushIndex);
		mFlushFullPath.append(indexStr);
	}

	mFlushFullPath.append(mFlushExt);
}

bool RenderImage::flush() {

	if (mFlushFullPath.isValid()) {

		if (write(mFlushFullPath.c_tstr())) {

			if (mAutoIncFileName) {

				++mFlushIndex;

				updateFlushFullPath();
			}

			return true;
		}
	}

	return true;
}

}