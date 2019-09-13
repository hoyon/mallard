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

void Camera::translateCamera(const M::Vector3& translation)
{
    auto transformation = this->transformation();
    auto& currentTranslation = transformation.translation();

    currentTranslation.x() = M::Math::clamp(currentTranslation.x() + translation.x(), -MaxPosition, MaxPosition);
    currentTranslation.y() = M::Math::clamp(currentTranslation.y() + translation.y(), -MaxPosition, MaxPosition);
    currentTranslation.z() = M::Math::clamp(currentTranslation.z() + translation.z(), -MaxPosition, MaxPosition);

    this->setTransformation(transformation);
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

    this->translateCamera(_velocity);
}

void Camera::tickAngle()
{
    using namespace M::Math::Literals;

    auto rotationX = 0._degf;
    auto rotationZ = 0._degf;
    M::Vector3 translation;

    if (Input::get().isKeyPressed(Input::Key::Up)) {
        rotationX = 1.0_degf;
        translation = M::Vector3::zAxis(-0.5f);
    }

    if (Input::get().isKeyPressed(Input::Key::Down)) {
        rotationX = -1.0_degf;
        translation = M::Vector3::zAxis(0.5f);
    }

    if (Input::get().isKeyPressed(Input::Key::Left)) {
        rotationZ = -1.0_degf;
    }

    if (Input::get().isKeyPressed(Input::Key::Right)) {
        rotationZ = 1.0_degf;
    }

    this->rotateXLocal(rotationX);
    this->rotateZLocal(rotationZ);
    this->translateCamera(translation);
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
