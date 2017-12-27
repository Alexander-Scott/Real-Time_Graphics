#pragma once

#include "GameObject.h"
#include "SceneLight.h"
#include "SceneCamera.h"
#include "GUIHandler.h"
#include "Octree.h"

using namespace std;

class Scene
{
public:
	Scene(string name);
	~Scene();

	void Update(float timeSinceStart, float deltaTime);

	string GetSceneName() { return mSceneName; }

	void AddGameObjects(vector<GameObject*> gos);
	void AddGameObject(GameObject* go);
	vector<GameObject*> GetGameObjects() { return mGameObjects; }
	GameObject* GetGameObject(int index) { return mGameObjects.at(index); }

	void AddSceneLight(SceneLight* sl) { mSceneLights.push_back(sl); mGameObjects.push_back(sl); }
	vector<SceneLight*> GetSceneLights() { return mSceneLights; }
	SceneLight* GetSceneLight(int index) { return mSceneLights.at(index); }
	float GetSceneLightsCount() { return mSceneLights.size(); }

	void OnMouseMove(float x, float y);
	SceneCamera* GetCamera() { return mSceneCamera; }

	std::vector<GameObject*> GetGameObjectsInFrustumOctree();
	std::vector<GameObject*> GetGameObjectsInFrustum();

private:
	void UpdateLightControls(float deltaTime);

	string						mSceneName;
	SceneCamera*				mSceneCamera;
	Octree*						mOctree;

	vector<GameObject*>			mGameObjects;
	vector<SceneLight*>			mSceneLights;
	vector<OctreeItem>			mGameObjects2;
};