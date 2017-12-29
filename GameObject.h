#pragma once

#include "TextureManager.h"
#include "Structures.h"

using namespace DirectX;
using namespace std;

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	// Setters for position/rotation/scale
	void SetPosition(XMFLOAT3 position) { mPosition = position; mHasChanged = true; }
	void SetPosition(float x, float y, float z) { mPosition.x = x; mPosition.y = y; mPosition.z = z; mHasChanged = true; }
	void SetScale(float x, float y, float z) { mScale.x = x; mScale.y = y; mScale.z = z; mHasChanged = true;}
	void SetRotation(float x, float y, float z) { mRotation.x = x; mRotation.y = y; mRotation.z = z; mHasChanged = true;}

	XMFLOAT3 GetPosition() { return mPosition; }
	XMFLOAT3 GetScale() { return mScale; }

	// Getters for object details
	string GetType() const { return mType; }
	Geometry GetGeometryData() const { return mGeometry; }
	Material GetMaterial() const { return mMaterial; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&mWorld); }

	void SetTextures(TextureSet* textureSet);

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { mTextureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return mTextureRV; }
	bool HasTexture() const { return mTextureRV ? true : false; }

	void SetNormalMap(ID3D11ShaderResourceView* normalMap) { mNormalMap = normalMap; }
	ID3D11ShaderResourceView* GetNormalMap() const { return mNormalMap; }
	bool HasNormalMap() const { return mNormalMap ? true : false; }

	void SetHeightMap(ID3D11ShaderResourceView* heightMap) { mHeightMap = heightMap; }
	ID3D11ShaderResourceView* GetHeightMap() const { return mHeightMap; }
	bool HasHeightMap() const { return mHeightMap ? true : false; }

	void SetParent(GameObject* parent) { mParent = parent; }
	void AddChild(GameObject* child) { mChildren.push_back(child); }

	bool Update(float t, float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext);
	
protected:
	string						mType;

	Geometry					mGeometry;
	Material					mMaterial;

private:
	XMFLOAT3					mPosition;
	XMFLOAT3					mRotation;
	XMFLOAT3					mScale;

	XMFLOAT4X4					mWorld;

	ID3D11ShaderResourceView*	mTextureRV;
	ID3D11ShaderResourceView*	mNormalMap;
	ID3D11ShaderResourceView*	mHeightMap;

	// Children in Scene Graph
	vector<GameObject*>			mChildren;

	GameObject *				mParent;

	bool						mHasChanged;
};

