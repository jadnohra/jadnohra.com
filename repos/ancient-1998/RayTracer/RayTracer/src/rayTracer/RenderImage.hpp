#ifndef _RayTracer_RenderImage_hpp
#define _RayTracer_RenderImage_hpp

#include "RayTracer.hpp"
#include "FreeImagePlus.h"
#include "RenderTarget.hpp"

namespace rayTracer {

	class RenderImage : public RenderTarget {
	public:

		RenderImage();
		virtual ~RenderImage();

		virtual bool isValid();

		virtual unsigned int getWidth();
		virtual unsigned int getHeight();

		virtual void destroy();
		virtual bool create(unsigned int width, unsigned int height);
		virtual void setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a);
		virtual void getPixel(unsigned int x, unsigned int y, float& r, float& g, float& b, float& a);

		virtual bool flush();

		void setFlushFilePath(const TCHAR* filePath, const TCHAR* fileName, const TCHAR* fileExt, bool autoIncFileName);
		
			
		bool load(const TCHAR* filePath);
		bool write(const TCHAR* filePath);

		String& getFullFlushPath() { return mFlushFullPath; }

	protected:

		void updateFlushFullPath();

	protected:

		fipImage mImage;

		bool mAutoCapColorToRange;

		bool mAutoIncFileName;
		String mFlushPath;
		String mFlushFile;
		String mFlushExt;
		String mFlushFullPath;
		unsigned int mFlushIndex;
	};

}

#endif