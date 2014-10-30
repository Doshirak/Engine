#pragma once 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11.h>
#include "figurePool.h"

#define HEIGHT 100
#define RADIUS 2
#define RADVERT 100
#define SIZE 20
#define FACES 6
#define XY 1
#define XZ 2
#define YZ 3

const int arrSize1 = SIZE * SIZE;
const int arrSize2 = (SIZE - 1) * (SIZE - 1) * 6;

class Device {
public:
	Device();
	Device(HINSTANCE g_hInst, HWND g_hWnd);
	~Device();
	HRESULT init();
	void render();
	void cleanup();
	void setVertexBuffer(D3D11_BUFFER_DESC* bd, D3D11_SUBRESOURCE_DATA* InitData, SimpleVertex* vertices, UINT size);
	void setIndexBuffer(D3D11_BUFFER_DESC* bd, D3D11_SUBRESOURCE_DATA* InitData, WORD* indices, UINT size);
	HINSTANCE               g_hInst = NULL;
	HWND                    g_hWnd = NULL;
private:
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           g_pd3dDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	IDXGISwapChain*         g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_pRenderTargetView = NULL;
	ID3D11Texture2D*        g_pDepthStencil = NULL;
	ID3D11DepthStencilView* g_pDepthStencilView = NULL;
	ID3D11VertexShader*     g_pVertexShader = NULL;
	ID3D11PixelShader*      g_pPixelShader = NULL;
	ID3D11PixelShader*      g_pPixelShader2 = NULL;
	ID3D11InputLayout*      g_pVertexLayout = NULL;
	ID3D11Buffer*           g_pVertexBuffer = NULL;
	ID3D11Buffer*           g_pIndexBuffer = NULL;
	ID3D11Buffer*           g_pConstantBuffer = NULL;
	FigurePool figurePool;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT compileShader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};