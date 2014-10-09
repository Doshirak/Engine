#pragma once 
#include "device.h"

class Sphere{
public:
	Sphere();
	Sphere(int size, float radius);
	~Sphere();
	int getVerticesNumber();
	int getIndicesNumber();
	SimpleVertex* getVerteces();
	WORD* getIndices();
private:
	// number of vertices in one row
	static const int DEFSIZE = 100;
	// number of rows
	static const int DEFHEIGHT = 100;
	int size = DEFSIZE;
	int height = DEFHEIGHT;
	float x0;
	float y0;
	float z0;
	// distance between two vertices in a row 
	float radius;
	int verticesN;
	int indicesN;
	SimpleVertex* vertices;
	WORD* indices;
	void setSphere();
	void setVertices();
	void setIndices();
};