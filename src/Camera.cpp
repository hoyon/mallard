#include "Camera.h"
#include "Debug/Debug.h"
#include <Corrade/Utility/Format.h>
#include <Magnum/GL/DefaultFramebuffer.h>

const float CameraAcceleration = 0.3f;
const float CameraDeceleration = 0.1f;
const float MaxVelocity = 1.f;
const float MaxPosition = 100.f;

Camera::Camera(Object3D* object)
    : Object3D(object)
{
    using namespace M::Math::Literals;

    this->translate(M::Vector3::zAxis(50.0f));

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

        case M::Platform::Sdl2Application::KeyEvent::Key::Up: {
            _angleUpPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Down: {
            _angleDownPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Left: {
            _angleLeftPressed = true;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Right: {
            _angleRightPressed = true;
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

        case M::Platform::Sdl2Application::KeyEvent::Key::Up: {
            _angleUpPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Down: {
            _angleDownPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Left: {
            _angleLeftPressed = false;
            break;
        }

        case M::Platform::Sdl2Application::KeyEvent::Key::Right: {
            _angleRightPressed = false;
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

void Camera::tickPosition()
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

    // clamp velocity
    _velocity.x() = M::Math::clamp(_velocity.x(), -MaxVelocity, MaxVelocity);
    _velocity.y() = M::Math::clamp(_velocity.y(), -MaxVelocity, MaxVelocity);

    this->translateLocal(_velocity);

    // clamp position
    auto transformation = this->transformation();
    auto& translation = transformation.translation();

    translation.x() = M::Math::clamp(translation.x(), -MaxPosition, MaxPosition);
    translation.y() = M::Math::clamp(translation.y(), -MaxPosition, MaxPosition);

    this->setTransformation(transformation);
}

void Camera::tickAngle()
{
    using namespace M::Math::Literals;

    if (_angleUpPressed) {
        this->rotateXLocal(1.0_degf);
        this->translate(M::Vector3::zAxis(-0.5f));
    }

    if (_angleDownPressed) {
        this->rotateXLocal(-1.0_degf);
        this->translate(M::Vector3::zAxis(0.5f));
    }

    if (_angleLeftPressed) {
        this->rotateZLocal(-1.0_degf);
    }

    if (_angleRightPressed) {
        this->rotateZLocal(1.0_degf);
    }
}

void Camera::tick()
{
    tickPosition();
    tickAngle();

    auto trans = this->transformation().translation();

    Debug::get().addMessage(M::Utility::formatString("({:.6f}, {:.6f}, {:.6f})", trans.x(), trans.y(), trans.z()));
}

void Camera::draw(M::SceneGraph::DrawableGroup3D& drawables)
{
    _cameraFeature->draw(drawables);
}
