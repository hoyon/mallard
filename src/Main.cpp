#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>

#include "Scene.h"
#include "Cube.h"
#include "Ground.h"

namespace M = Magnum;

class MallardApplication: public M::Platform::Application {

    public:
        explicit MallardApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void tickEvent() override;
        void keyPressEvent(KeyEvent& event) override;

        Scene3D _scene;
        M::SceneGraph::Camera3D* _camera;
        M::SceneGraph::DrawableGroup3D _drawables;

        Object3D* _cube;

        M::Vector3 _cameraVelocity;
};

namespace {
    auto windowConfiguration() {
        return M::Platform::Application::Configuration{}
            .setTitle("Mallard")
            .setSize({1280, 720});
    }
}

MallardApplication::MallardApplication(const Arguments& arguments)
    : M::Platform::Application{arguments, windowConfiguration()}
{
    M::GL::Renderer::enable(M::GL::Renderer::Feature::DepthTest);
    M::GL::Renderer::enable(M::GL::Renderer::Feature::FaceCulling);

    using namespace M::Math::Literals;

    auto& cameraObject = _scene.addChild<Object3D>();
    cameraObject.translate(M::Vector3::zAxis(50.0f));

    auto& camera = cameraObject.addFeature<M::SceneGraph::Camera3D>();
    camera.setAspectRatioPolicy(M::SceneGraph::AspectRatioPolicy::Extend)
          .setProjectionMatrix(M::Matrix4::perspectiveProjection(35.0_degf, 16.0f/9.0f, 0.001f, 100.0f))
          .setViewport(M::GL::defaultFramebuffer.viewport().size());

    _camera = &camera;

    auto& cube = _scene.addChild<Object3D>();
    cube.addFeature<Cube>(&_drawables);

    _cube = &cube;

    auto& groundObject = _scene.addChild<Object3D>();
    groundObject.addFeature<Ground>(&_drawables);
}

void MallardApplication::drawEvent() {
    M::GL::defaultFramebuffer.clear(M::GL::FramebufferClear::Color | M::GL::FramebufferClear::Depth);

    _camera->draw(_drawables);

    swapBuffers();
    redraw();
}

void MallardApplication::keyPressEvent(KeyEvent& event) {
    using namespace M::Math::Literals;

    const auto cameraAcceleration = 0.4f;

    Object3D& cameraObject = static_cast<Object3D&>(_camera->object());

    switch (event.key()) {
        case KeyEvent::Key::Q:
            exit();
            break;

        case KeyEvent::Key::W:
        {
            _cameraVelocity.y() += cameraAcceleration;
            break;
        }

        case KeyEvent::Key::S:
        {
            _cameraVelocity.y() -= cameraAcceleration;
            break;
        }

        case KeyEvent::Key::D:
        {
            _cameraVelocity.x() += cameraAcceleration;
            break;
        }

        case KeyEvent::Key::A:
        {
            _cameraVelocity.x() -= cameraAcceleration;
            break;
        }

        default:
            break;

    }

    _cameraVelocity.x() = M::Math::clamp(_cameraVelocity.x(), -1.f, 1.f);
    _cameraVelocity.y() = M::Math::clamp(_cameraVelocity.y(), -1.f, 1.f);

    cameraObject.translate(_cameraVelocity);

    event.setAccepted();
}

void MallardApplication::tickEvent() {
    if (_cameraVelocity.x() > 0) {
        _cameraVelocity.x() -= 0.1f;
        _cameraVelocity.x() = M::Math::max(_cameraVelocity.x(), 0.f);
    } else if (_cameraVelocity.x() < 0) {
        _cameraVelocity.x() += 0.1f;
        _cameraVelocity.x() = M::Math::min(_cameraVelocity.x(), 0.f);
    }

    if (_cameraVelocity.y() > 0) {
        _cameraVelocity.y() -= 0.1f;
        _cameraVelocity.y() = M::Math::max(_cameraVelocity.y(), 0.f);
    } else if (_cameraVelocity.y() < 0) {
        _cameraVelocity.y() += 0.1f;
        _cameraVelocity.y() = M::Math::min(_cameraVelocity.y(), 0.f);
    }

    Object3D& cameraObject = static_cast<Object3D&>(_camera->object());
    cameraObject.translate(_cameraVelocity);
}

MAGNUM_APPLICATION_MAIN(MallardApplication)
