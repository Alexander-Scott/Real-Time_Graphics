#pragma once

#include <d3d11_1.h>
#include <unordered_map>
#include <memory>

#include "DirectXInstance.h"
#include "RenderToTextureProcess.h"
#include "GUIController.h"

using namespace std;
using namespace DirectX;

class ShaderController
{
public:
	ShaderController();
	~ShaderController();

	RenderToTextureProcess* GetRenderToTextureProcess(string s) { return mShaderList[s].get(); }

	HRESULT AddDepthBufferShader(string shaderName, float renderWidth, float renderHeight, WCHAR* szFileName, vector<D3D11_INPUT_ELEMENT_DESC> layoutDescVec, ID3D11Device* d3dDevice);
	HRESULT AddCommonShader(string shaderName, float renderWidth, float renderHeight, WCHAR* szFileName, vector<D3D11_INPUT_ELEMENT_DESC> layoutDescVec, ID3D11Device* d3dDevice);
	HRESULT AddRenderFromQuadShader(string shaderName, float renderWidth, float renderHeight, WCHAR* szFileName, vector<D3D11_INPUT_ELEMENT_DESC> layoutDescVec, ID3D11Device* d3dDevice);
	HRESULT AddRenderToBackBufferShader(string shaderName, float renderWidth, float renderHeight, WCHAR* szFileName, vector<D3D11_INPUT_ELEMENT_DESC> layoutDescVec, ID3D11Device* d3dDevice);
	void AddCustomShader(string shaderName, float renderWidth, float renderHeight);

	void UpdateShaderSelection(int selectedShaderOption);

	void ExecuteShadersInOrder(ConstantBuffer* cb, vector<SceneLight*> lights, vector<GameObject*> gameObjects);

private:
	HRESULT SetupShader(string shaderName, string rtvName, float renderWidth, float renderHeight, WCHAR* szFileName, vector<D3D11_INPUT_ELEMENT_DESC> layoutDescVec, ID3D11Device* d3dDevice);

	unordered_map<string, unique_ptr<RenderToTextureProcess>>	mShaderList;

	RenderToTextureProcess*										mCurrentSceneRenderProcess;

	int															mCurrentShaderOptionSelected;
};