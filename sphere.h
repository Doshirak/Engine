#pragma once 
#include <d3dx11.h>
#include <xnamath.h>

#include "device.h"
#include "figure.h"

class Sphere : Figure{
public:
	Sphere();	
	Sphere(int size);
	Sphere(int size, float radius);
	~Sphere();
	int getVerticesNumber();
	int getIndicesNumber();
	SimpleVertex* getVerteces();
	WORD* getIndices();
	XMMATRIX getMatrix();
	void setMatrix(const XMMATRIX& matrix);
	ConstantBuffer getConstantBuffer();
	void setConstantBuffer(const ConstantBuffer& constantBuffer);
private:
	// number of vertices in one row
	static const int DEFSIZE = 100;
	// number of rows
	static const int DEFHEIGHT = 100;
	int height = DEFHEIGHT;
	float x0;
	float y0;
	float z0;
	// distance between two vertices in a row 
	float radius = 3;
	int verticesN;
	int indicesN;
	SimpleVertex* vertices;
	WORD* indices;
	XMMATRIX matrix;
	XMFLOAT4 color = { 0.1f, 0.1f, 0.1f, 0.1f };
	void setSphere();
	void setVertices();
	void setIndices();
};