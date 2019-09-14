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

    void tick();
    void draw(M::SceneGraph::DrawableGroup3D& drawables);

private:
    void tickPosition();
    void tickAngle();

    M::SceneGraph::Camera3D* _cameraFeature;

    M::Vector3 _velocity;
    M::Vector3 _position;
    M::Math::Vector3<M::Deg> _rotation;
};

#endif // MALLARD_CAMERA_H
