#pragma once
#include <d3dx11.h>
#include <xnamath.h>

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
	XMMATRIX inversedWorld;
	XMFLOAT4 eye;
	XMFLOAT3 lightDirection;
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT4 specularColor;
	XMFLOAT3 pointLightPos;
	XMFLOAT3 specularLightPos;
	INT shininess;
	INT attenuation;
};

class Figure {
public:
	Figure();
	Figure(int size);
	virtual SimpleVertex* getVerteces() = 0;
	virtual WORD* getIndices() = 0;
	virtual int getVerticesNumber() = 0;
	virtual int getIndicesNumber() = 0;
	int size;
	ConstantBuffer cb;
	XMFLOAT4 color;
};