#include "Camera.h"

#include <Corrade/Utility/Format.h>
#include <Magnum/GL/DefaultFramebuffer.h>

#include "Debug/Debug.h"
#include "Input.h"

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
    if (Input::get().isKeyPressed(Input::Key::W)) {
        _velocity.y() += CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::S)) {
        _velocity.y() -= CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::D)) {
        _velocity.x() += CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::A)) {
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

    if (Input::get().isKeyPressed(Input::Key::Up)) {
        this->rotateXLocal(1.0_degf);
        this->translate(M::Vector3::zAxis(-0.5f));
    }

    if (Input::get().isKeyPressed(Input::Key::Down)) {
        this->rotateXLocal(-1.0_degf);
        this->translate(M::Vector3::zAxis(0.5f));
    }

    if (Input::get().isKeyPressed(Input::Key::Left)) {
        this->rotateZLocal(-1.0_degf);
    }

    if (Input::get().isKeyPressed(Input::Key::Right)) {
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
