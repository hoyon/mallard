#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>

#include "Camera.h"
#include "Cube.h"
#include "Debug/Debug.h"
#include "Ground.h"
#include "Scene.h"

namespace M = Magnum;

class MallardApplication : public M::Platform::Application
{
public:
    explicit MallardApplication(const Arguments& arguments);

private:
    void drawEvent() override;
    void tickEvent() override;
    void keyPressEvent(KeyEvent& event) override;
    void keyReleaseEvent(KeyEvent& event) override;
    void viewportEvent(ViewportEvent& event) override;

    Scene3D _scene;
    Camera* _camera;
    M::SceneGraph::DrawableGroup3D _drawables;

    Object3D* _cube;
};

namespace
{
auto windowConfiguration()
{
    return M::Platform::Application::Configuration{}
        .setTitle("Mallard")
        .setSize({1280, 720});
}
} // namespace

MallardApplication::MallardApplication(const Arguments& arguments)
    : M::Platform::Application{arguments, windowConfiguration()}
{
    M::GL::Renderer::enable(M::GL::Renderer::Feature::DepthTest);
    M::GL::Renderer::enable(M::GL::Renderer::Feature::FaceCulling);

    M::GL::Renderer::enable(M::GL::Renderer::Feature::Blending);
    M::GL::Renderer::setBlendFunction(M::GL::Renderer::BlendFunction::SourceAlpha, M::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
    M::GL::Renderer::setBlendEquation(M::GL::Renderer::BlendEquation::Add, M::GL::Renderer::BlendEquation::Add);

    _camera = &_scene.addChild<Camera>();

    auto& cube = _scene.addChild<Object3D>();
    cube.addFeature<Cube>(&_drawables);

    _cube = &cube;

    auto& groundObject = _scene.addChild<Object3D>();
    groundObject.addFeature<Ground>(&_drawables);

    Debug::get();
}

void MallardApplication::drawEvent()
{
    M::GL::defaultFramebuffer.clear(M::GL::FramebufferClear::Color | M::GL::FramebufferClear::Depth);

    _camera->draw(_drawables);

    Debug::get().draw();

    swapBuffers();
    redraw();
}

void MallardApplication::keyPressEvent(KeyEvent& event)
{
    switch (event.key()) {
        case KeyEvent::Key::Q:
            exit();
            break;

        default:
            break;
    }

    if (!event.isRepeated()) {
        _camera->keyPressed(event.key());
    }

    event.setAccepted();
}

void MallardApplication::keyReleaseEvent(KeyEvent& event)
{
    _camera->keyReleased(event.key());

    event.setAccepted();
}

void MallardApplication::tickEvent()
{
    _camera->tick();
}

void MallardApplication::viewportEvent(ViewportEvent& event)
{
    M::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
}

MAGNUM_APPLICATION_MAIN(MallardApplication)
