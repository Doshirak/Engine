#pragma once 
#include "device.h"

class Cube{
public:
	Cube();
	Cube(int size, float d);
	~Cube();
	SimpleVertex* getVerteces();
	WORD* getIndices();
private:
	// number of vertices in one row
	static const int DEFSIZE = 100;
	int size = DEFSIZE;
	float x0;
	float y0;
	float z0;
	// distance between two vertices in a row 
	float d = 0.3f;
	int verticesN;
	int indicesN;
	SimpleVertex* vertices;
	WORD* indices;
	void setCube();
	void setFaceVertices(SimpleVertex* vertices, int size, float x0, float y0, float z0, float d, int mode);
	void setFaceIndices(WORD* indices, int size, int offset);
};