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

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
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

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the 
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = compileShader(L"Shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
<<<<<<< HEAD
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
=======
>>>>>>> origin/master
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = compileShader(L"Shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

<<<<<<< HEAD
	pPSBlob = NULL;
	hr = compileShader(L"Shader2.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	 //Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader2);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;


	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	g_View = XMMatrixLookAtLH(Eye, At, Up);
	
	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	Figure* cube = (Figure*) new Cube(20);
	Figure* sphere = (Figure*) new Sphere(20);

	cube->setMatrix(XMMatrixIdentity());
	sphere->setMatrix(XMMatrixIdentity());

=======
	Figure* cube = (Figure*) new Cube(20);
	Figure* sphere = (Figure*) new Sphere(20);

>>>>>>> origin/master
	figurePool.add(cube);
	figurePool.add(sphere);

	return S_OK;
}

void Device::render()
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));
<<<<<<< HEAD
=======


	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
>>>>>>> origin/master

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);

<<<<<<< HEAD
=======
	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
>>>>>>> origin/master

	// Update our time
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	// cube
	Figure* cube = figurePool.getFigures()[0];

	// 1st Cube: Rotate around the origin
	cube->setMatrix(XMMatrixIdentity());
	g_World1 = XMMatrixRotationY(t);
	cube->setMatrix(XMMatrixRotationY(t));

	// 2nd Cube:  Rotate around origin
	XMMATRIX mSpin = XMMatrixRotationZ(-t);
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
	XMMATRIX mTranslate = XMMatrixTranslation(-6.0f, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);

	g_World2 = mScale * mSpin * mTranslate * mOrbit;

	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red, green, blue, alpha
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// LIGHST

	XMFLOAT4 vLightDirs[2] =
	{
		XMFLOAT4(0.0f, 3.0f, 5.0f, 0.0f),
		XMFLOAT4(0.0f, 3.0f, 5.0f, 0.0f),
	};
	XMFLOAT4 vLightColors[2] =
	{
		XMFLOAT4(0.5f, 0.1f, 0.1f, 0.1f),
		XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)
	};
	//XMMATRIX mRotate = XMMatrixRotationX(-2.0f * t);
	//XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
	//vLightDir = XMVector3Transform(vLightDir, mRotate);
	//XMStoreFloat4(&vLightDirs[1], vLightDir);
<<<<<<< HEAD
=======

	ConstantBuffer cb1;
>>>>>>> origin/master

	// FIRST CUBE 
	XMMATRIX temp = cube->getMatrix();
	XMMATRIX matrix = XMMatrixMultiply(XMMatrixTranspose(g_Projection), XMMatrixTranspose(g_View));
<<<<<<< HEAD
	matrix = XMMatrixMultiply(matrix, XMMatrixTranspose(temp));
	cube->setMatrix(matrix);

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(temp);
=======
	matrix = XMMatrixMultiply(matrix, XMMatrixTranspose(g_World1));
	Figure* cube = figurePool.getFigures()[0];
	cube->setMatrix(matrix);

	cb1.mWorld = XMMatrixTranspose(g_World1);
>>>>>>> origin/master
	cb1.mView = XMMatrixTranspose(g_View);
	cb1.mProjection = XMMatrixTranspose(g_Projection);
	cb1.Matrix = cube->getMatrix();
	cb1.flag = 1;
	cb1.time = abs(sin(t));
	cb1.PHI = XM_PI;
	cb1.vLightDir[0] = vLightDirs[0];
	cb1.vLightDir[1] = vLightDirs[1];
	cb1.vLightColor[0] = vLightColors[0];
	cb1.vLightColor[1] = vLightColors[1];
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
<<<<<<< HEAD
	cube->setConstantBuffer(cb1);

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cube->getConstantBuffer(), 0, 0);
	setVertexBuffer(&bd, &InitData, cube->getVerteces(), cube->getVerticesNumber());
	setIndexBuffer(&bd, &InitData, cube->getIndices(), cube->getIndicesNumber());

=======
	
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	setVertexBuffer(&bd, &InitData, cube->getVerteces(), cube->getVerticesNumber());
	setIndexBuffer(&bd, &InitData, cube->getIndices(), cube->getIndicesNumber());
>>>>>>> origin/master
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
<<<<<<< HEAD
	
	//g_pImmediateContext->PSSetShader(g_pPixelShader2, NULL, 0);
	//g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
=======
>>>>>>> origin/master
	g_pImmediateContext->DrawIndexed(cube->getIndicesNumber(), 0, 0);									

	// SPHERE
	Sphere sphere(100, 3.0f);
	setVertexBuffer(&bd, &InitData, sphere.getVerteces(), sphere.getVerticesNumber());
	setIndexBuffer(&bd, &InitData, sphere.getIndices(), sphere.getIndicesNumber());
	matrix = XMMatrixMultiply(XMMatrixTranspose(g_Projection), XMMatrixTranspose(g_View));
	matrix = XMMatrixMultiply(matrix, XMMatrixTranspose(g_World2));
	cb1.mWorld = XMMatrixTranspose(g_World2);
	cb1.mView = XMMatrixTranspose(g_View);
	cb1.mProjection = XMMatrixTranspose(g_Projection);
	cb1.Matrix = matrix;
	cb1.flag = 0;
	cb1.vLightDir[0] = vLightDirs[0];
	cb1.vLightDir[1] = vLightDirs[1];
	cb1.vLightColor[0] = vLightColors[0];
	cb1.vLightColor[1] = vLightColors[1];
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	cb1.time = abs(sin(t));
	cb1.PHI = XM_PI;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(sphere.getIndicesNumber(), 0, 0);

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