#pragma once 
#include <d3dx11.h>
#include <xnamath.h>
#include "device.h"
#include "figure.h"

class Cube : Figure {
public:
	Cube();
	Cube(int size);
	~Cube();
	int getVerticesNumber();
	int getIndicesNumber();
	SimpleVertex* getVerteces();
	WORD* getIndices();
	XMMATRIX getMatrix();
	void setMatrix(const XMMATRIX& matrix);
	ConstantBuffer getConstantBuffer();
	void setConstantBuffer(const ConstantBuffer& constantBuffer);
private:
	static const int DEFSIZE = 100;
	// number of vertices in one row
	float x0;
	float y0;
	float z0;
	// distance between two vertices in a row 
	float d = 0.3f;
	int verticesN;
	int indicesN;
	SimpleVertex* vertices;
	WORD* indices;
	XMMATRIX matrix;
	void setCube();
	void setFaceVertices(SimpleVertex* vertices, int size, float x0, float y0, float z0, float d, int mode, XMFLOAT3 n);
	void setFaceIndices(WORD* indices, int size, int offset);
	void setFaceIndices2(WORD* indices, int size, int offset);
};