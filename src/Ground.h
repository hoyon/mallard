#ifndef MALLARD_GROUND_H
#define MALLARD_GROUND_H

#include <Magnum/GL/Mesh.h>
#include <Magnum/SceneGraph/AbstractObject.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Shaders/Phong.h>

namespace M = Magnum;

class Ground : public M::SceneGraph::Drawable3D
{
public:
    explicit Ground(M::SceneGraph::AbstractObject3D& object, M::SceneGraph::DrawableGroup3D* group);

private:
    void draw(const M::Matrix4& transformation, M::SceneGraph::Camera3D& camera) override;

    M::GL::Mesh _mesh;
    M::Shaders::Phong _shader;
};

#endif // MALLARD_GROUND_H
