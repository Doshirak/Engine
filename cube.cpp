#include "cube.h"

Cube::Cube(){
	size = DEFSIZE;
	verticesN = size * size * 6;
	indicesN = (size - 1) * (size - 1) * 36;
	x0 = -(size / 2) * d;
	y0 = -(size / 2) * d;
	z0 = -(size / 2) * d;
	vertices = new SimpleVertex[size * size * 6];
	indices = new WORD[size * size * 36];
	setCube();
}

Cube::Cube(int size, float d){
	this->size = size;
	verticesN = size * size * 6;
	indicesN = (size - 1) * (size - 1) * 36;
	this->d = d;
	x0 = -(size / 2) * d;
	y0 = -(size / 2) * d;
	z0 = -(size / 2) * d;
	vertices = new SimpleVertex[size * size * 6];
	indices = new WORD[size * size * 36];
	setCube();
}

Cube::~Cube(){
	delete[] vertices;
	delete[] indices;
}

int Cube::getVerticesNumber(){
	return verticesN;
}

int Cube::getIndicesNumber(){
	return indicesN;
}

SimpleVertex* Cube::getVerteces(){
	return vertices;
}

WORD* Cube::getIndices(){
	return indices;
}

void Cube::setCube(){
	int arrSize1 = size * size;
	int arrSize2 = (size - 1) * (size - 1) * 6;
	setFaceVertices(vertices, size, x0, y0, z0, d, XY);
	setFaceIndices(indices, size, 0);
	setFaceVertices(vertices + arrSize1, size, x0, y0, z0, d, XZ);
	setFaceIndices2(indices + arrSize2, size, arrSize1);
	setFaceVertices(vertices + arrSize1 * 2, size, x0, y0, z0, d, YZ);
	setFaceIndices2(indices + arrSize2 * 2, size, arrSize1 * 2);

	setFaceVertices(vertices + arrSize1 * 3, size, x0, y0, z0 + (size - 1) * d, d, XY);
	setFaceIndices2(indices + arrSize2 * 3, size, arrSize1 * 3);
	setFaceVertices(vertices + arrSize1 * 4, size, x0, y0 + (size - 1) * d, z0, d, XZ);
	setFaceIndices(indices + arrSize2 * 4, size, arrSize1 * 4);
	setFaceVertices(vertices + arrSize1 * 5, size, x0 + (size - 1) * d, y0, z0, d, YZ);
	setFaceIndices(indices + arrSize2 * 5, size, arrSize1 * 5);
}

void Cube::setFaceVertices(SimpleVertex* vertices, int size, float x0, float y0, float z0, float d, int mode)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			XMFLOAT3 n = {0.0f, 0.0f, 0.0f};
			int x, y, z;
			if (mode == XY) {
				x = x0 + d * j;
				y = y0 + i * d;
				z = z0;
				n.z = 1;
			}
			else if (mode == XZ) {
				x = x0 + d * j;
				y = y0;
				z = z0 + d * i;
				n.y = 1;
			}
			else if (mode == YZ) {
				x = x0;
				y = y0 + i * d;
				z = z0 + j * d;
				n.x = 1;
			}
			vertices[i * size + j] = { XMFLOAT3(x, y, z), n };
		}
	}
}

void Cube::setFaceIndices(WORD* indices, int size, int offset)
{
	for (int h = 0; h < size - 1; ++h)
	{
		int j = size * (h + 1) + offset;
		for (int i = 0; i < size - 1; ++i)
		{
			indices[(h * (size - 1) + i) * 6] = j;
			indices[(h * (size - 1) + i) * 6 + 1] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 2] = j - size;
			indices[(h * (size - 1) + i) * 6 + 3] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 4] = j;
			indices[(h * (size - 1) + i) * 6 + 5] = (j + 1);
			++j;
		}
	}
}

void Cube::setFaceIndices2(WORD* indices, int size, int offset)
{
	for (int h = 0; h < size - 1; ++h)
	{
		int j = size * (h + 1) + offset;
		for (int i = 0; i < size - 1; ++i)
		{
			indices[(h * (size - 1) + i) * 6] = j;
			indices[(h * (size - 1) + i) * 6 + 1] = j - size;
			indices[(h * (size - 1) + i) * 6 + 2] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 3] = (j - size + 1);
			indices[(h * (size - 1) + i) * 6 + 4] = (j + 1);
			indices[(h * (size - 1) + i) * 6 + 5] = j;
			++j;
		}
	}
}