#include "Camera.h"

#include <Corrade/Utility/Format.h>
#include <Magnum/GL/DefaultFramebuffer.h>

#include "Debug/Debug.h"
#include "Input.h"

const float CameraAcceleration = 0.3f;
const float CameraDeceleration = 0.1f;
const float MaxVelocity = 1.f;

Camera::Camera(Object3D* object)
    : Object3D(object)
    , _position(0, 0, 50)
{
    using namespace M::Math::Literals;

    this->translate(_position);

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
        _velocity.z() -= CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::S)) {
        _velocity.z() += CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::D)) {
        _velocity.x() += CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::A)) {
        _velocity.x() -= CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::Space)) {
        _velocity.y() += CameraAcceleration;
    }

    if (Input::get().isKeyPressed(Input::Key::LeftShift)) {
        _velocity.y() -= CameraAcceleration;
    }

    decelerateElement(_velocity.x());
    decelerateElement(_velocity.y());
    decelerateElement(_velocity.z());

    // clamp velocity
    _velocity.x() = M::Math::clamp(_velocity.x(), -MaxVelocity, MaxVelocity);
    _velocity.y() = M::Math::clamp(_velocity.y(), -MaxVelocity, MaxVelocity);
    _velocity.z() = M::Math::clamp(_velocity.z(), -MaxVelocity, MaxVelocity);

    auto translation = M::Matrix4::translation(_velocity);

    auto x = M::Matrix4::rotationX(M::Deg(_rotation.x()));
    auto y = M::Matrix4::rotationY(M::Deg(_rotation.y()));
    auto z = M::Matrix4::rotationZ(M::Deg(_rotation.z()));

    M::Matrix4 rotation = z * x * y;

    _position += (rotation * translation).translation();
}

void Camera::tickAngle()
{
    using namespace M::Math::Literals;

    if (Input::get().isKeyPressed(Input::Key::Up)) {
        _rotation.x() += 1;
    }

    if (Input::get().isKeyPressed(Input::Key::Down)) {
        _rotation.x() -= 1;
    }

    if (Input::get().isKeyPressed(Input::Key::Left)) {
        _rotation.z() += 1;
    }

    if (Input::get().isKeyPressed(Input::Key::Right)) {
        _rotation.z() -= 1;
    }
}

void Camera::tick()
{
    tickAngle();
    tickPosition();

    auto x = M::Matrix4::rotationX(M::Deg(_rotation.x()));
    auto y = M::Matrix4::rotationY(M::Deg(_rotation.y()));
    auto z = M::Matrix4::rotationZ(M::Deg(_rotation.z()));

    M::Matrix4 rotation = z * x * y;
    M::Matrix4 translation = M::Matrix4::translation(_position);

    auto transformation = translation * rotation;

    this->setTransformation(transformation);

    Debug::get().addMessage(M::Utility::formatString("({:.3f}, {:.3f}, {:.3f})", _position.x(), _position.y(), _position.z()));
    Debug::get().addMessage(M::Utility::formatString("({:.3f}, {:.3f}, {:.3f})", _rotation.x(), _rotation.y(), _rotation.z()));
}

void Camera::draw(M::SceneGraph::DrawableGroup3D& drawables)
{
    _cameraFeature->draw(drawables);
}
