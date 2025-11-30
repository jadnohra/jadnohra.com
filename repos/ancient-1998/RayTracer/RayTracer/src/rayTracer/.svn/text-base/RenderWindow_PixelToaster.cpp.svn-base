#include "RenderWindow_PixelToaster.hpp"

namespace rayTracer {

RenderWindow_PixelToaster::RenderWindow_PixelToaster() : mDoDebugColors(false) {
}

RenderWindow_PixelToaster::~RenderWindow_PixelToaster() {
}

bool RenderWindow_PixelToaster::isValid() {

	return mDisplay.width() > 0;
}

void RenderWindow_PixelToaster::destroy() {

	mDisplay.close();
}

unsigned int RenderWindow_PixelToaster::getWidth() {

	if (isValid()) {

		return mDisplay.width();
	}

	return 0;
}

unsigned int RenderWindow_PixelToaster::getHeight() {

	if (isValid()) {

		return mDisplay.height();
	}

	return 0;
}

bool RenderWindow_PixelToaster::create(unsigned int width, unsigned int height) {

	if (isValid()) {

		if (getWidth() == width && getHeight() == height)
			return true;
	}

	if (mDisplay.open("*", width, height, PixelToaster::Output::Default, PixelToaster::Mode::FloatingPoint)) {

		mDisplay.listener(this);
		mBuffer.resize(width * height);

		wprintf(L"Keys:\n");
		wprintf(L"p: toggle pixel debug\n");
		wprintf(L"\n");

		return true;

	} else {

		destroy();
		return false;
	}
}

void RenderWindow_PixelToaster::setPixel(unsigned int x, unsigned int y, float r, float g, float b, float a) {

	if (isValid()) {

		PixelToaster::FloatingPointPixel pixel(r, g, b, a);

		mBuffer[(y * mDisplay.width()) + x] = pixel;
	}
}

void RenderWindow_PixelToaster::getPixel(unsigned int x, unsigned int y, float& r, float& g, float& b, float& a) {

	if (isValid()) {

		PixelToaster::FloatingPointPixel& pixel = mBuffer[(y * mDisplay.width()) + x];

		r = pixel.r;
		g = pixel.g;
		b = pixel.b;
		a = pixel.a;
	}
}

bool RenderWindow_PixelToaster::flush() {

	return mDisplay.update(mBuffer.el);
}

bool RenderWindow_PixelToaster::update() {

	mDisplay.open();
	flush();

	return isValid();
}

void RenderWindow_PixelToaster::onMouseMove(PixelToaster::DisplayInterface& display, PixelToaster::Mouse mouse) {

	if (mDoDebugColors) {

		if (isValid()) {

			PixelToaster::FloatingPointPixel& pixel = mBuffer[((unsigned int) mouse.y * mDisplay.width()) + (unsigned int) mouse.x];

			wprintf(L"%u,%u -> ", (unsigned int) mouse.x, (unsigned int) mouse.y);
			wprintf(L"%f, %f, %f\n", pixel.r, pixel.g, pixel.b);
		}
	}
}

void RenderWindow_PixelToaster::onMouseButtonDown(PixelToaster::DisplayInterface & display, PixelToaster::Mouse mouse) {

	if (mSourceRenderer.isValid()
		&& mSourceRenderer->getRenderContextScene()
		&& mSourceRenderer->getRenderContextCamera()) {
		
		Camera& cam = dref(mSourceRenderer->getRenderContextCamera());
		Scene& scene = dref(mSourceRenderer->getRenderContextScene());

		Ray ray;
		Vector2 viewportPoint;

		cam.pixelToViewport(mouse.x, mouse.y, viewportPoint.x, viewportPoint.y);
		cam.setupRayForPixel(viewportPoint.x, viewportPoint.y, scene, ray);

		RayShot rayShot(ray, NULL, scene.getRayTolerance());
		RayShotHit rayHit;

		if (scene.shootRay(rayShot, rayHit)) {

			rayHit.resolvePoint(ray);

			wprintf(L"px:%u,%u hit t:%f point:%f,%f,%f\n", (unsigned int) mouse.x, (unsigned int) mouse.y, rayHit.inters.t, rayHit.intersInfo.point.x, rayHit.intersInfo.point.y, rayHit.intersInfo.point.z);

		} else {

			wprintf(L"px:%u,%u no hit\n", (unsigned int) mouse.x, (unsigned int) mouse.y);
		}
	}
}

void RenderWindow_PixelToaster::onKeyPressed(PixelToaster::DisplayInterface& display, PixelToaster::Key key) {

	switch ((PixelToaster::Key::Code) key) {

		case PixelToaster::Key::P: {

			mDoDebugColors = !mDoDebugColors;

		} break;
	}

	if (mManipCamera.isValid()) {

		Vector3 moveVel;
		moveVel.zero();
		bool isDirty = false;

		float theta = 0.0f;
		Vector3 axis;

		switch ((PixelToaster::Key::Code) key) {

			case PixelToaster::Key::Left: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Right).mul(-(mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);

			} break;

			case PixelToaster::Key::Right: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Right).mul((mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);
				
			} break;

			case PixelToaster::Key::Up: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Forward).mul((mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);

			} break;

			case PixelToaster::Key::Down: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Forward).mul(-(mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);
				
			} break;

			case PixelToaster::Key::PageUp: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Up).mul((mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);

			} break;

			case PixelToaster::Key::PageDown: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				Vector3 vel;
				mManipCamera->getWorldTransform().getDirection(Scene_Up).mul((-mManipCamera->getMoveSpeed()), vel);
				moveVel.add(vel);
				
			} break;

			case PixelToaster::Key::NumPad4: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				scene.coordSys().setRUF(0.0f, 1.0f, 0.0f, axis.el);
				theta = -mManipCamera->getRotSpeed() * clock.getFrameTicks() * clock.getTickLength();

			} break;

			case PixelToaster::Key::NumPad6: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				scene.coordSys().setRUF(0.0f, 1.0f, 0.0f, axis.el);
				theta = mManipCamera->getRotSpeed() * clock.getFrameTicks() * clock.getTickLength();

			} break;

			case PixelToaster::Key::NumPad8: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				scene.coordSys().setRUF(1.0f, 0.0f, 0.0f, axis.el);
				theta = -mManipCamera->getRotSpeed() * clock.getFrameTicks() * clock.getTickLength();

			} break;

			case PixelToaster::Key::NumPad5: {

				isDirty = true;

				Scene& scene = dref(mManipCamera->scene());
				Clock& clock = dref(scene.getClock());

				scene.coordSys().setRUF(1.0f, 0.0f, 0.0f, axis.el);
				theta = mManipCamera->getRotSpeed() * clock.getFrameTicks() * clock.getTickLength();

			} break;
		}

		if (!moveVel.isZero()) {

			Scene& scene = dref(mManipCamera->scene());
			Clock& clock = dref(scene.getClock());

			mManipCamera->worldTransform().movePositionByVel(moveVel, clock.getFrameTicks() * clock.getTickLength());
		}

		if (theta != 0.0f) {

			Scene& scene = dref(mManipCamera->scene());
			Clock& clock = dref(scene.getClock());

			mManipCamera->worldTransform().moveOrientation(axis, theta);
		}

		if (isDirty) {

			mManipCamera->markDirty();
		} 
	}
}

}