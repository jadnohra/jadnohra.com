#ifndef _RayTracer_RenderWindow_hpp
#define _RayTracer_RenderWindow_hpp

#include "RayTracer.hpp"
#include "RenderTarget.hpp"
#include "../3rdparty/PixelToaster1.4/PixelToaster.h"
#include "Camera.hpp"
#include "Renderer.hpp"

namespace rayTracer {

	class RenderWindow_PixelToaster : public RenderTarget, public PixelToaster::Listener {
	public:

		RenderWindow_PixelToaster();
		virtual ~RenderWindow_PixelToaster();

		virtual bool isValid();

		virtual unsigned int getWidth();
		virtual unsigned int getHeight();

		virtual void destroy();
		virtual bool create(unsigned int width, unsigned int height);
		virtual void setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a);
		virtual void getPixel(unsigned int x, unsigned int y, float& r, float& g, float& b, float& a);
		
		virtual bool flush();

		void setSourceRenderer(Renderer* pRenderer) { mSourceRenderer = pRenderer; }
		void setManipCamera(Camera* pCamera) { mManipCamera = pCamera; }
		bool update();

	public:

		virtual void onKeyPressed(PixelToaster::DisplayInterface& display, PixelToaster::Key key);
		virtual void onMouseMove(PixelToaster::DisplayInterface& display, PixelToaster::Mouse mouse);
		virtual void onMouseButtonDown(PixelToaster::DisplayInterface & display, PixelToaster::Mouse mouse);

	protected:

		PixelToaster::Display mDisplay;
		WETL::SizeAllocT<PixelToaster::FloatingPointPixel, size_t, false> mBuffer;

		SoftPtr<Camera> mManipCamera;
		SoftPtr<Renderer> mSourceRenderer;

		bool mDoDebugColors;
	};

}

#endif