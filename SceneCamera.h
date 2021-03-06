//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Simple first person style camera class that lets the viewer explore the 3D scene.
//   -It keeps track of the camera coordinate system relative to the world space
//    so that the view matrix can be constructed.  
//   -It keeps track of the viewing frustum of the camera so that the projection
//    matrix can be obtained.
//***************************************************************************************

#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <sstream>
#include "resource.h"
#include <iostream>

#include <vector>
#include <memory>

#include "GameObject.h"
#include "DirectXInstance.h"

using namespace DirectX;

class SceneCamera : public GameObject
{
public:
	SceneCamera(float nearDepth, float farDepth, float windowWidth, float windowHeight, bool canFly,
		std::string type, Geometry geometry, Material material);
	~SceneCamera();

	// Get view/proj matricies
	XMMATRIX GetViewMatrix()const { return mViewMatrix; }
	XMMATRIX GetProjectionMatrix()const { return mProjectionMatrix; }

	// Get the bounding frustum of this camera
	BoundingFrustum GetBoundingFrustum();

	void OnMouseMove(int x, int y);

	void UpdateCameraViewMatrix();

private:
	// Create the cameras projection matrix
	void CreateProjectionMatrix();

	float mCameraPitch = 0;
	float mCameraYaw = 0;

	float mStrafeVelocity = 0.0f;
	float mForwardVelocity = 0.0f;

	POINT mLastMousePos;

	// Cache frustum properties.
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	// If the camera can fly or just walk
	bool mCanFly;

	// Cache View/Proj matrices.
	XMMATRIX mViewMatrix = Identity4x4();
	XMMATRIX mProjectionMatrix = Identity4x4();

	XMMATRIX Identity4x4()
	{
		static XMMATRIX I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}
};