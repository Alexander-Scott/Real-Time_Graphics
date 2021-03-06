#include "GUIController.h"

#pragma region Static Member Initialisation

ID3D11SamplerState* GUIController::_pFontSampler = NULL;

ID3DBlob* GUIController::_pVSBlob = NULL;
ID3DBlob* GUIController::_pPSBlob = NULL;
ID3D11VertexShader* GUIController::_pVertexShader = NULL;
ID3D11PixelShader* GUIController::_pPixelShader = NULL;
ID3D11InputLayout* GUIController::_pInputLayout = NULL;
ID3D11Buffer* GUIController::_pVertexConstantBuffer = NULL;
ID3D11BlendState* GUIController::_pBlendState = NULL;
ID3D11RasterizerState* GUIController::_pRasterizerState = NULL;
ID3D11DepthStencilState* GUIController::_pDepthStencilState = NULL;
ID3D11ShaderResourceView* GUIController::_pFontTextureView = NULL;
ID3D11Buffer* GUIController::_pVB = NULL;
ID3D11Buffer* GUIController::_pIB = NULL;

int GUIController::_pVertexBufferSize = 5000;
int GUIController::_pIndexBufferSize = 10000;

INT64 GUIController::_pTime = 0;
INT64 GUIController::_pTicksPerSecond = 0;

bool GUIController::BlurOn = false;
bool GUIController::BlurWasOn = false;
int GUIController::BlurEffectPasses = 0;
float GUIController::BlurIntensity = 0;

bool GUIController::WhiteLightOn = true;
bool GUIController::RedLightOn = true;
bool GUIController::GreenLightOn = false;
bool GUIController::BlueLightOn = false;

int GUIController::ControlledLight = 0;
int GUIController::SceneLightingMode = 0;

bool GUIController::SelfShadingOn = true;

bool GUIController::FlyingCameraEnabled = false;

ImGuiIO& GUIController::io = ImGui::GetIO();

#pragma endregion

GUIController::GUIController()
{
}

GUIController::~GUIController()
{
}

void GUIController::SetupGUI()
{
	InitGUI(DirectXInstance::Instance()._hWnd, DirectXInstance::Instance()._pd3dDevice, DirectXInstance::Instance()._pImmediateContext);
}

void GUIController::UpdateGUI()
{
	GUINewFrame();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Press ESC to exit this application."); // Yellow colour

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	//////////////////////////////////////////////////////////////
	// Camera Controls
	//////////////////////////////////////////////////////////////
	ImGui::Text("Camera Controls");
	ImGui::Spacing();
	ImGui::Text("Use the arrow keys to move around the scene.");
	ImGui::Text("Right click and move the mouse to change camera rotation.");
	ImGui::Text("Press"); ImGui::SameLine(50); 
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "C"); ImGui::SameLine(65);
	ImGui::Text("to switch between flying and walking camera.");
	ImGui::Text("Current Camera: "); ImGui::SameLine(120);
	if (FlyingCameraEnabled == true)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Flying"); 
	else
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Walking"); 

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	//////////////////////////////////////////////////////////////
	// Light Controls
	//////////////////////////////////////////////////////////////
	ImGui::Text("Light Controls");
	ImGui::Spacing();

	ImGui::Combo("Lighting Mode", &SceneLightingMode, "Omni-Directional\0Directional");
	ImGui::Spacing();

	if (SceneLightingMode == 1) // Omni directional light
	{
		ImGui::Text("Press"); ImGui::SameLine(50);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "T"); ImGui::SameLine(65);
		ImGui::Text("to toggle on and off the flashlight.");
		ImGui::Text("Move the camera to change flashlight direction.");
	}
	else // Directional light
	{
		ImGui::Text("Toggle lights on and off using the"); ImGui::SameLine(250);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "checkboxes.");
		ImGui::Spacing();
		ImGui::Checkbox("White", &WhiteLightOn); ImGui::SameLine(100);
		ImGui::Checkbox("Red", &RedLightOn); ImGui::SameLine(200);
		ImGui::Checkbox("Green", &GreenLightOn); ImGui::SameLine(300);
		ImGui::Checkbox("Blue", &BlueLightOn);

		ImGui::Spacing();

		ImGui::Text("Press"); ImGui::SameLine(50);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "1, 2, 3 or 4"); ImGui::SameLine(140);
		ImGui::Text("to switch between the lights.");

		ImGui::Text("Use"); ImGui::SameLine(35);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "WASD"); ImGui::SameLine(70);
		ImGui::Text("to move the selected light.");
		ImGui::Text("Current Selected Light: "); ImGui::SameLine(175);
		if (ControlledLight == 0)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "White");
		else if (ControlledLight == 1)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Red");
		else if (ControlledLight == 2)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Green");
		else if (ControlledLight == 3)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Blue");
	}

	ImGui::Spacing();

	// Self Shadowing Options
	ImGui::Checkbox("Self Shadowing", &SelfShadingOn);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	// Blur Controls
	ImGui::Text("Blur Effect Controls");
	ImGui::Checkbox("Blur On/Off", &BlurOn);

	if (BlurOn && !BlurWasOn)
	{
		BlurIntensity += 0.1f;
		BlurWasOn = true;
	}
	else if (!BlurOn && BlurWasOn)
	{
		BlurIntensity = 0.0f;
		BlurWasOn = false;
	}

	ImGui::SliderInt("Blur Pass Count", &BlurEffectPasses, 0, 150);
	ImGui::Spacing();
	ImGui::SliderFloat("Blur Intensity", &BlurIntensity, 0.0f, 1.0f);

	if (BlurOn && BlurIntensity < 0.1f)
	{
		BlurIntensity = 0.1f;
	}

	if (!BlurOn && BlurIntensity > 0.0f)
	{
		BlurIntensity = 0.0f;
	}

	if (!BlurOn && BlurEffectPasses > 1)
	{
		BlurEffectPasses = 1;
	}

	ImGui::Spacing();
}

void GUIController::ExitGUI()
{
	InvalidateDeviceObjects();
	ImGui::Shutdown();
}

void GUIController::GUINewFrame()
{
	if (!_pFontSampler)
	{
		CreateDeviceObjects();
	}

	io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	RECT rect;
	GetClientRect(DirectXInstance::Instance()._hWnd, &rect);
	io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

	// Setup time step
	INT64 _currentTime;
	QueryPerformanceCounter((LARGE_INTEGER *)&_currentTime);
	io.DeltaTime = 1 / 60.0f;
	_pTime = _currentTime;

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(DirectXInstance::Instance()._hWnd, &p);
	ImVec2 mousePos;
	/*mousePos.x = p.x / 2.0f;
	mousePos.y = p.y / 1.91f - 20.0f;*/

	mousePos.x = p.x;
	mousePos.y = p.y;

	if (io.WantCaptureMouse)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			io.MouseDown[0] = true;
		}
		else
		{
			io.MouseDown[0] = false;
		}
	}

	// Read keyboard modifiers inputs
	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;
	io.MousePos = mousePos;
	//io.MouseDown = filled by WM_*BUTTON* events
	//io.MouseWheel = filled by WM_MOUSEWHEEL events

	// Hide OS mouse cursor if ImGui is drawing it
	if (io.MouseDrawCursor)
		SetCursor(NULL);

	// Start the frame
	ImGui::NewFrame();
}

bool GUIController::InitGUI(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&_pTicksPerSecond))
		return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&_pTime))
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.RenderDrawListsFn = RenderDrawLists;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.ImeWindowHandle = DirectXInstance::Instance()._hWnd;

	return true;
}

void GUIController::RenderDrawLists(ImDrawData* draw_data)
{
	ID3D11DeviceContext* ctx = DirectXInstance::Instance()._pImmediateContext;

	// Create and grow vertex/index buffers if needed
	if (!_pVB || _pVertexBufferSize < draw_data->TotalVtxCount)
	{
		if (_pVB) 
		{ 
			_pVB->Release(); _pVB = NULL;
		}
		_pVertexBufferSize = draw_data->TotalVtxCount + 5000;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = _pVertexBufferSize * sizeof(ImDrawVert);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		if (DirectXInstance::Instance()._pd3dDevice->CreateBuffer(&desc, NULL, &_pVB) < 0)
		{
			return;
		}
			
	}
	if (!_pIB || _pIndexBufferSize < draw_data->TotalIdxCount)
	{
		if (_pIB) { _pIB->Release(); _pIB = NULL; }
		_pIndexBufferSize = draw_data->TotalIdxCount + 10000;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = _pIndexBufferSize * sizeof(ImDrawIdx);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (DirectXInstance::Instance()._pd3dDevice->CreateBuffer(&desc, NULL, &_pIB) < 0)
			return;
	}

	// Copy and convert all vertices into a single contiguous buffer
	D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
	if (ctx->Map(_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
		return;
	if (ctx->Map(_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
		return;
	ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
	ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		vtx_dst += cmd_list->VtxBuffer.Size;
		idx_dst += cmd_list->IdxBuffer.Size;
	}
	ctx->Unmap(_pVB, 0);
	ctx->Unmap(_pIB, 0);

	// Setup orthographic projection matrix into our constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		if (ctx->Map(_pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
		{
			return;
		}
			
		VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
		float L = 0.0f;
		float R = ImGui::GetIO().DisplaySize.x;
		float B = ImGui::GetIO().DisplaySize.y;
		float T = 0.0f;
		float mvp[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
		};
		memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
		ctx->Unmap(_pVertexConstantBuffer, 0);
	}

	// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
	struct BACKUP_DX11_STATE
	{
		UINT                        ScissorRectsCount, ViewportsCount;
		D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		ID3D11RasterizerState*      RS;
		ID3D11BlendState*           BlendState;
		FLOAT                       BlendFactor[4];
		UINT                        SampleMask;
		UINT                        StencilRef;
		ID3D11DepthStencilState*    DepthStencilState;
		ID3D11ShaderResourceView*   PSShaderResource;
		ID3D11SamplerState*         PSSampler;
		ID3D11PixelShader*          PS;
		ID3D11VertexShader*         VS;
		UINT                        PSInstancesCount, VSInstancesCount;
		ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
		D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
		ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
		UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
		DXGI_FORMAT                 IndexBufferFormat;
		ID3D11InputLayout*          InputLayout;
	};
	BACKUP_DX11_STATE old;
	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
	ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
	ctx->RSGetState(&old.RS);
	ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
	ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
	ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);
	ctx->PSGetSamplers(0, 1, &old.PSSampler);
	old.PSInstancesCount = old.VSInstancesCount = 256;
	ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
	ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
	ctx->IAGetInputLayout(&old.InputLayout);

	// Setup viewport
	D3D11_VIEWPORT vp;
	memset(&vp, 0, sizeof(D3D11_VIEWPORT));
	vp.Width = ImGui::GetIO().DisplaySize.x;
	vp.Height = ImGui::GetIO().DisplaySize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = vp.TopLeftY = 0.0f;
	ctx->RSSetViewports(1, &vp);

	// Bind shader and vertex buffers
	unsigned int stride = sizeof(ImDrawVert);
	unsigned int offset = 0;
	ctx->IASetInputLayout(_pInputLayout);
	ctx->IASetVertexBuffers(0, 1, &_pVB, &stride, &offset);
	ctx->IASetIndexBuffer(_pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ctx->VSSetShader(_pVertexShader, NULL, 0);
	ctx->VSSetConstantBuffers(0, 1, &_pVertexConstantBuffer);
	ctx->PSSetShader(_pPixelShader, NULL, 0);
	ctx->PSSetSamplers(0, 1, &_pFontSampler);

	// Setup render state
	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	ctx->OMSetBlendState(_pBlendState, blend_factor, 0xffffffff);
	ctx->OMSetDepthStencilState(_pDepthStencilState, 0);
	ctx->RSSetState(_pRasterizerState);

	// Render command lists
	int vtx_offset = 0;
	int idx_offset = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
				ctx->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
				ctx->RSSetScissorRects(1, &r);
				ctx->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.Size;
	}

	// Restore modified DX state
	ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
	ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
	ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
	ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
	ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
	ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
	ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
	ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
	ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
	ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
}

bool GUIController::CreateDeviceObjects()
{
	if (!DirectXInstance::Instance()._pd3dDevice)
	{
		return false;
	}

	if (_pFontSampler)
	{
		InvalidateDeviceObjects();
	}

	// By using D3DCompile() from <d3dcompiler.h> / d3dcompiler.lib, we introduce a dependency to a given version of d3dcompiler_XX.dll (see D3DCOMPILER_DLL_A)
	// If you would like to use this DX11 sample code but remove this dependency you can: 
	//  1) compile once, save the compiled shader blobs into a file or source code and pass them to CreateVertexShader()/CreatePixelShader() [preferred solution]
	//  2) use code to detect any version of the DLL and grab a pointer to D3DCompile from the DLL. 
	// See https://github.com/ocornut/imgui/pull/638 for sources and details.

	// Create the vertex shader
	{
		static const char* vertexShader =
			"cbuffer vertexBuffer : register(b0) \
            {\
            float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
            float2 pos : POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
            PS_INPUT output;\
            output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
            output.col = input.col;\
            output.uv  = input.uv;\
            return output;\
            }";

		D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &_pVSBlob, NULL);

		if (_pVSBlob == NULL)
		{
			return false;
		}

		if (DirectXInstance::Instance()._pd3dDevice->CreateVertexShader((DWORD*)_pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), NULL, &_pVertexShader) != S_OK)
		{
			return false;
		}

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC local_layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (DirectXInstance::Instance()._pd3dDevice->CreateInputLayout(local_layout, 3, _pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), &_pInputLayout) != S_OK)
			return false;

		// Create the constant buffer
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			DirectXInstance::Instance()._pd3dDevice->CreateBuffer(&desc, NULL, &_pVertexConstantBuffer);
		}
	}

	// Create the pixel shader
	{
		static const char* pixelShader =
			"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

		D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &_pPSBlob, NULL);

		if (_pPSBlob == NULL)
		{
			return false;
		}
			
		if (DirectXInstance::Instance()._pd3dDevice->CreatePixelShader((DWORD*)_pPSBlob->GetBufferPointer(), _pPSBlob->GetBufferSize(), NULL, &_pPixelShader) != S_OK)
		{
			return false;
		}
	}

	// Create the blending setup
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		DirectXInstance::Instance()._pd3dDevice->CreateBlendState(&desc, &_pBlendState);
	}

	// Create the rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		DirectXInstance::Instance()._pd3dDevice->CreateRasterizerState(&desc, &_pRasterizerState);
	}

	// Create depth-stencil State
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		DirectXInstance::Instance()._pd3dDevice->CreateDepthStencilState(&desc, &_pDepthStencilState);
	}

	CreateFontsTexture();

	return true;
}

void GUIController::InvalidateDeviceObjects()
{
	if (!DirectXInstance::Instance()._pd3dDevice)
		return;

	if (_pFontSampler) { _pFontSampler->Release(); _pFontSampler = NULL; }
	if (_pFontTextureView) { _pFontTextureView->Release(); _pFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = 0; }
	if (_pIB) { _pIB->Release(); _pIB = NULL; }
	if (_pVB) { _pVB->Release(); _pVB = NULL; }

	if (_pBlendState) { _pBlendState->Release(); _pBlendState = NULL; }
	if (_pDepthStencilState) { _pDepthStencilState->Release(); _pDepthStencilState = NULL; }
	if (_pRasterizerState) { _pRasterizerState->Release(); _pRasterizerState = NULL; }
	if (_pPixelShader) { _pPixelShader->Release(); _pPixelShader = NULL; }
	if (_pPSBlob) { _pPSBlob->Release(); _pPSBlob = NULL; }
	if (_pVertexConstantBuffer) { _pVertexConstantBuffer->Release(); _pVertexConstantBuffer = NULL; }
	if (_pInputLayout) { _pInputLayout->Release(); _pInputLayout = NULL; }
	if (_pVertexShader) { _pVertexShader->Release(); _pVertexShader = NULL; }
	if (_pVSBlob) { _pVSBlob->Release(); _pVSBlob = NULL; }
}

void GUIController::CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	// Upload texture to graphics system
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D *pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		DirectXInstance::Instance()._pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		DirectXInstance::Instance()._pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &_pFontTextureView);
		pTexture->Release();
	}

	// Store our identifier
	io.Fonts->TexID = (void *)_pFontTextureView;

	// Create texture sampler
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.f;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0.f;
		desc.MaxLOD = 0.f;
		DirectXInstance::Instance()._pd3dDevice->CreateSamplerState(&desc, &_pFontSampler);
	}
}