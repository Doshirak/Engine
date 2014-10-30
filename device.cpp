#include "Device.h"
#include "setCube.h"
#include "cube.h"
#include "sphere.h"

Device::Device(){

}

Device::Device(HINSTANCE g_hInst, HWND g_hWnd) {
	this->g_hInst = g_hInst;
	this->g_hWnd = g_hWnd;
}

Device::~Device() {
}

HRESULT Device::init()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// TARGET VIEW
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// STENCIL TEXTURE
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// STENCIL VIEW
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// VIEWPORT
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// VERTEX SHADER
	ID3DBlob* pVSBlob = NULL;
	hr = compileShader(L"Shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr)) { MessageBox(NULL, L"The FX file cannot be compiled", L"Error", MB_OK); return hr;}
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr)) { pVSBlob->Release(); return hr;}

	// INPUT LAYOUT
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr)) return hr; 
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// PIXEL SHADER
	ID3DBlob* pPSBlob = NULL;
	hr = compileShader(L"Shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr)) { MessageBox(NULL, L"The FX file cannot be compiled", L"Error", MB_OK); return hr;}
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;

	// PRIMITIVE TOPOLOGY
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// GLOBAL MATRICES
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(Eye, At, Up);
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	// LIGHTS
	XMFLOAT3 lightDirection = { 10.0f, 10.0f, 0.0f };
	XMFLOAT4 ambientColor = { 0, 0, 0, 0.1f };
	XMFLOAT4 diffuseColor = { 0.5f, 0.5f, 0.5f, 0.5f };
	XMFLOAT4 specularColor = { 0.5f, 0.5f, 0.5f, 0.5f };

	// CUBE
	Figure* cube = (Figure*) new Cube(20);
	cube->cb.view = XMMatrixTranspose(view);
	cube->cb.projection = XMMatrixTranspose(projection);
	cube->cb.eye = XMFLOAT4(0.0f, 3.0f, -10.0f, 0.0f);
	cube->cb.ambientColor = ambientColor;
	cube->cb.lightDirection = lightDirection;
	cube->cb.diffuseColor = diffuseColor;
	cube->cb.specularColor = specularColor;
	cube->cb.shininess = 200;
	figurePool.add(cube);

	// SPHERE
	Figure* sphere = (Figure*) new Sphere(50);
	sphere->cb.view = XMMatrixTranspose(view);
	sphere->cb.projection = XMMatrixTranspose(projection);
	sphere->cb.eye = XMFLOAT4(0.0f, 3.0f, -10.0f, 0.0f);
	sphere->cb.ambientColor = ambientColor;
	sphere->cb.lightDirection = lightDirection;
	sphere->cb.diffuseColor = diffuseColor;
	sphere->cb.specularColor = specularColor;
	sphere->cb.shininess = 200;
	figurePool.add(sphere);

	// CREATE CONSTANT BUFFER
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);

	return S_OK;
}

void Device::render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red, green, blue, alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Update our time
	static float t = 0.0f;
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;

	XMMATRIX spin, orbit, translate, scale;
	
	// CUBE 

	Figure* cube = figurePool.getFigures(0);
	setVertexBuffer(&bd, &InitData, cube->getVerteces(), cube->getVerticesNumber());
	setIndexBuffer(&bd, &InitData, cube->getIndices(), cube->getIndicesNumber());

	spin = XMMatrixRotationZ(-t);
	orbit = XMMatrixRotationY(0);
	translate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	scale = XMMatrixScaling(0.5f, 0.5, 0.5f);
	cube->cb.word = XMMatrixTranspose(scale * spin * translate * orbit);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cube->cb, 0, 0);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(cube->getIndicesNumber(), 0, 0);


	//// SPHERE

	Figure* sphere = figurePool.getFigures(1);
	setVertexBuffer(&bd, &InitData, sphere->getVerteces(), sphere->getVerticesNumber());
	setIndexBuffer(&bd, &InitData, sphere->getIndices(), sphere->getIndicesNumber());

	spin = XMMatrixRotationZ(-t);
	orbit = XMMatrixRotationY(-t * 2.0f);
	translate = XMMatrixTranslation(-6.0f, 0.0f, 0.0f);
	scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	sphere->cb.word = XMMatrixTranspose(scale * spin * translate * orbit);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &sphere->cb, 0, 0);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(sphere->getIndicesNumber(), 0, 0);

	g_pSwapChain->Present(0, 0);
}

void Device::cleanup()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}

void Device::setVertexBuffer(D3D11_BUFFER_DESC* bd, D3D11_SUBRESOURCE_DATA* InitData, SimpleVertex* vertices, UINT size) {

	bd->Usage = D3D11_USAGE_DEFAULT;
	bd->ByteWidth = sizeof(SimpleVertex) * size;											
	bd->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd->CPUAccessFlags = 0;

	InitData->pSysMem = vertices;
	g_pd3dDevice->CreateBuffer(bd, InitData, &g_pVertexBuffer);
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
}

void Device::setIndexBuffer(D3D11_BUFFER_DESC* bd, D3D11_SUBRESOURCE_DATA* InitData, WORD* indices, UINT size) {
	bd->Usage = D3D11_USAGE_DEFAULT;
	bd->ByteWidth = sizeof(WORD)* size;													
	bd->BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd->CPUAccessFlags = 0;
	InitData->pSysMem = indices;
	g_pd3dDevice->CreateBuffer(bd, InitData, &g_pIndexBuffer);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

HRESULT Device::compileShader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}