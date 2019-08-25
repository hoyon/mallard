#ifndef MALLARD_SCENE_H
#define MALLARD_SCENE_H

#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

namespace M = Magnum;

typedef M::SceneGraph::Object<M::SceneGraph::MatrixTransformation3D> Object3D;
typedef M::SceneGraph::Scene<M::SceneGraph::MatrixTransformation3D> Scene3D;

#endif // MALLARD_SCENE_H
