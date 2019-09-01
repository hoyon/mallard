#ifndef MALLARD_CUBE_H
#define MALLARD_CUBE_H

#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/SceneGraph/AbstractObject.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData3D.h>

#include "Scene.h"

namespace M = Magnum;

class Cube : public M::SceneGraph::Drawable3D
{
public:
    explicit Cube(M::SceneGraph::AbstractObject3D& object, M::SceneGraph::DrawableGroup3D* group)
        : M::SceneGraph::Drawable3D{object, group}
    {
        _mesh = M::MeshTools::compile(M::Primitives::cubeSolid());
    }

private:
    void draw(const M::Matrix4& transformation, M::SceneGraph::Camera3D& camera) override
    {
        using namespace M::Math::Literals;

        _shader.setDiffuseColor(0xa5c9ea_rgbf)
            .setLightPosition(camera.cameraMatrix().transformPoint({5.0f, 5.0f, 7.0f}))
            .setTransformationMatrix(transformation)
            .setNormalMatrix(transformation.rotation())
            .setProjectionMatrix(camera.projectionMatrix());

        _mesh.draw(_shader);
    }

    M::GL::Mesh _mesh;
    M::Shaders::Phong _shader;
};

#endif // MALLARD_CUBE_H
