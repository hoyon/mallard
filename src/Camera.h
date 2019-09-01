#ifndef MALLARD_CAMERA_H
#define MALLARD_CAMERA_H

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "Scene.h"

namespace M = Magnum;

class Camera: public Object3D
{
    public:
        explicit Camera(Object3D* object);

        void keyPressed(M::Platform::Sdl2Application::KeyEvent::Key key);
        void keyReleased(M::Platform::Sdl2Application::KeyEvent::Key key);

        void tick();
        void draw(M::SceneGraph::DrawableGroup3D& drawables);

    private:
        M::SceneGraph::Camera3D* _cameraFeature;

        M::Vector3 _velocity;

        bool _upPressed{false};
        bool _downPressed{false};
        bool _leftPressed{false};
        bool _rightPressed{false};
};

#endif // MALLARD_CAMERA_H
