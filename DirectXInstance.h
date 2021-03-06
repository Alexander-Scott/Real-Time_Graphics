#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>
#include "resource.h"

#include "Structures.h"

using namespace DirectX;

class DirectXInstance
{
public:
	DirectXInstance();
	~DirectXInstance();

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();

	HWND					_hWnd;
	ID3D11Device*			_pd3dDevice;
	ID3D11DeviceContext*	_pImmediateContext;
	IDXGISwapChain*			_pSwapChain;

	ID3D11Buffer*			_pCubeVertexBuffer;
	ID3D11Buffer*			_pCubeIndexBuffer;

	ID3D11Buffer*			_pPlaneVertexBuffer;
	ID3D11Buffer*			_pPlaneIndexBuffer;

	ID3D11Buffer*			_pTerrainVertexBuffer;
	ID3D11Buffer*			_pTerrainIndexBuffer;

	ID3D11Buffer*			_pConstantBuffer;
	ID3D11Buffer*			_pSMConstantBuffer;

	// Render dimensions
	UINT					_pRenderHeight;
	UINT					_pRenderWidth;

	static DirectXInstance& Instance()
	{
		static DirectXInstance Instance;
		return Instance;
	}

private:
	HRESULT InitConstantBuffers();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();
	HRESULT InitTerrainBuffers();
	HRESULT InitRasterizerState();

	bool HeightMapLoad(char* filename, HeightMapInfo &hminfo);

	HINSTANCE					_hInst;
	D3D_DRIVER_TYPE				_pDriverType;
	D3D_FEATURE_LEVEL			_pFeatureLevel;

	UINT						_pWindowHeight;
	UINT						_pWindowWidth;

	ID3D11DepthStencilState*	_pDSLessEqual;
	ID3D11RasterizerState*		_pRSCullMode;
};