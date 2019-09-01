#include "Ground.h"

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Plane.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/SceneGraph/Camera.h>

Ground::Ground(M::SceneGraph::AbstractObject3D& object, M::SceneGraph::DrawableGroup3D* group)
    : M::SceneGraph::Drawable3D{object, group}
{
    _mesh = M::MeshTools::compile(M::Primitives::planeSolid());
}

void Ground::draw(const M::Matrix4& transformation, M::SceneGraph::Camera3D& camera)
{
    using namespace M::Math::Literals;

    auto scaleMatrix = transformation.scaling({10.f, 10.f, 1.f});
    auto scaled = transformation * scaleMatrix;

    _shader.setDiffuseColor(0xa5c9ea_rgbf)
        .setLightPosition(camera.cameraMatrix().transformPoint({5.0f, 5.0f, 7.0f}))
        .setShininess(9000.f)
        .setTransformationMatrix(scaled)
        .setNormalMatrix(scaled.rotation())
        .setProjectionMatrix(camera.projectionMatrix());

    _mesh.draw(_shader);
}
