#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Math/Color.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>

#include "Scene.h"
#include "Cube.h"

namespace M = Magnum;

class MallardApplication: public M::Platform::Application {

    public:
        explicit MallardApplication(const Arguments& arguments);

    private:
        void drawEvent() override;
        void keyPressEvent(KeyEvent& event) override;

        Scene3D _scene;
        Object3D* _cameraObject;
        M::SceneGraph::Camera3D* _camera;
        M::SceneGraph::DrawableGroup3D _drawables;

        Object3D* _cube;
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

    _cameraObject = new Object3D{&_scene};
    _cameraObject->translate(M::Vector3::zAxis(50.0f));
    _camera = new M::SceneGraph::Camera3D{*_cameraObject};
    _camera->setAspectRatioPolicy(M::SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(M::Matrix4::perspectiveProjection(35.0_degf, 16.0f/9.0f, 0.001f, 100.0f))
        .setViewport(M::GL::defaultFramebuffer.viewport().size());

    Object3D& cube = _scene.addChild<Object3D>();
    cube.addFeature<Cube>(&_drawables);

    _cube = &cube;
}

void MallardApplication::drawEvent() {
    M::GL::defaultFramebuffer.clear(M::GL::FramebufferClear::Color | M::GL::FramebufferClear::Depth);

    _camera->draw(_drawables);

    swapBuffers();
    redraw();
}

void MallardApplication::keyPressEvent(KeyEvent& event) {
    using namespace M::Math::Literals;
    switch (event.key()) {
        case KeyEvent::Key::Q:
            exit();
            break;

        case KeyEvent::Key::W:
            _cube->rotateX(1.0_degf);
            break;
        case KeyEvent::Key::S:
            _cube->rotateX(-1.0_degf);
            break;

        case KeyEvent::Key::A:
            _cube->rotateY(-1.0_degf);
            break;
        case KeyEvent::Key::D:
            _cube->rotateY(1.0_degf);
            break;

        default:
            break;

    }
    event.setAccepted();
}

MAGNUM_APPLICATION_MAIN(MallardApplication)
