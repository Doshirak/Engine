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
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMMATRIX Matrix;
	float time;
	int flag;
	float PHI;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

class Figure {
public:
	Figure();
	Figure(int size);
	virtual SimpleVertex* getVerteces() = 0;
	virtual WORD* getIndices() = 0;
	virtual int getVerticesNumber() = 0;
	virtual int getIndicesNumber() = 0;
	virtual XMMATRIX getMatrix() = 0;
	virtual void setMatrix(const XMMATRIX& matrix) = 0;
	virtual ConstantBuffer getConstantBuffer() = 0;
	virtual void setConstantBuffer(const ConstantBuffer& constantBuffer) = 0;
protected:
	int size;
	XMMATRIX matrix;
	ConstantBuffer constantBuffer;
};