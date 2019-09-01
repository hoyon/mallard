#include "Camera.h"
#include <Magnum/GL/DefaultFramebuffer.h>

const float CameraAcceleration = 0.3f;
const float CameraDeceleration = 0.1f;
const float MaxVelocity = 1.f;

Camera::Camera(Object3D* object)
    : Object3D(object)
{
    using namespace M::Math::Literals;

    translate(M::Vector3::zAxis(50.0f));

    _cameraFeature = &addFeature<M::SceneGraph::Camera3D>();

    _cameraFeature->setAspectRatioPolicy(M::SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(M::Matrix4::perspectiveProjection(35.0_degf, 16.0f / 9.0f, 0.001f, 100.0f))
        .setViewport(M::GL::defaultFramebuffer.viewport().size());
}

void Camera::keyPressed(M::Platform::Sdl2Application::KeyEvent::Key key)
{
    switch (key) {
        case M::Platform::Sdl2Application::KeyEvent::Key::W: {
            _upPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::S: {
            _downPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::D: {
            _leftPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::A: {
            _rightPressed = true;
            break;
        }

        default:
            break;
    }
}

void Camera::keyReleased(M::Platform::Sdl2Application::KeyEvent::Key key)
{
    switch (key) {
        case M::Platform::Sdl2Application::KeyEvent::Key::W: {
            _upPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::S: {
            _downPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::D: {
            _leftPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::A: {
            _rightPressed = false;
            break;
        }

        default:
            break;
    }
}

static void decelerateElement(float& elem)
{
    if (elem > 0) {
        elem -= CameraDeceleration;
        elem = M::Math::max(elem, 0.f);
    } else if (elem < 0) {
        elem += CameraDeceleration;
        elem = M::Math::min(elem, 0.f);
    }
}

void Camera::tick()
{
    if (_upPressed) {
        _velocity.y() += CameraAcceleration;
    }

    if (_downPressed) {
        _velocity.y() -= CameraAcceleration;
    }

    if (_leftPressed) {
        _velocity.x() += CameraAcceleration;
    }

    if (_rightPressed) {
        _velocity.x() -= CameraAcceleration;
    }

    decelerateElement(_velocity.x());
    decelerateElement(_velocity.y());

    _velocity.x() = M::Math::clamp(_velocity.x(), -MaxVelocity, MaxVelocity);
    _velocity.y() = M::Math::clamp(_velocity.y(), -MaxVelocity, MaxVelocity);

    translate(_velocity);
}

void Camera::draw(M::SceneGraph::DrawableGroup3D& drawables)
{
    _cameraFeature->draw(drawables);
}
