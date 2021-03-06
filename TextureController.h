#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "DDSTextureLoader.h"

using namespace std;
using namespace DirectX;

struct TextureSet
{
	TextureSet(string newName, ID3D11ShaderResourceView* newTexture, ID3D11ShaderResourceView* newNormalMap, ID3D11ShaderResourceView* newHeightMap)
	{
		texture = newTexture;
		normalMap = newNormalMap;
		heightMap = newHeightMap;
	}

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;
	ID3D11ShaderResourceView* heightMap;
};

class TextureController
{
public:
	TextureController();
	~TextureController();

	void AddTexture(ID3D11Device* d3dDevice, string textureSetName, LPCWSTR texturePath, LPCWSTR normalMapPath, LPCWSTR heightMapPath);
	TextureSet* GetTextureSet(string s) { return mTextureList[s].get(); }

private:
	unordered_map<string, unique_ptr<TextureSet>> mTextureList;
};
