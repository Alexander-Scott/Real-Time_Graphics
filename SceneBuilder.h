#pragma once

#include <fstream>

#include "Scene.h"

#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

namespace SceneBuilder
{
	Scene* BuildScene(string fileName, Geometry cubeGeometry, Material noSpecMaterial);
	inline GameObject* BuildGameObject(xml_node<>* node, Geometry cubeGeometry, Material noSpecMaterial);
	inline SceneLight* BuildSceneLight(xml_node<>* node, Geometry cubeGeometry, Material noSpecMaterial);
	inline vector<GameObject*> BuildPlane(xml_node<>* node, Geometry cubeGeometry, Material noSpecMaterial);
};
