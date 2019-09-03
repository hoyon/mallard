#ifndef MALLARD_CAMERA_H
#define MALLARD_CAMERA_H

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>

#include "Scene.h"

namespace M = Magnum;

class Camera : public Object3D
{
public:
    explicit Camera(Object3D* object);

    void keyPressed(M::Platform::Sdl2Application::KeyEvent::Key key);
    void keyReleased(M::Platform::Sdl2Application::KeyEvent::Key key);

    void tick();
    void draw(M::SceneGraph::DrawableGroup3D& drawables);

private:
    void tickPosition();
    void tickAngle();

    M::SceneGraph::Camera3D* _cameraFeature;

    M::Vector3 _velocity;

    bool _upPressed{false};
    bool _downPressed{false};
    bool _leftPressed{false};
    bool _rightPressed{false};

    bool _angleUpPressed{false};
    bool _angleDownPressed{false};
    bool _angleLeftPressed{false};
    bool _angleRightPressed{false};
};

#endif // MALLARD_CAMERA_H
