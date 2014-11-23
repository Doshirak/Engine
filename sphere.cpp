#include <d3dx11.h>
#include <xnamath.h>
#include "device.h"
#include "sphere.h"

Sphere::Sphere(){
	size = DEFSIZE;
	height = size;
	radius = 1;
	verticesN = size * height;
	indicesN = (size - 1) * height * 6;
	x0 = 0;
	y0 = 0;
	z0 = 0;
	vertices = new SimpleVertex[verticesN];
	indices = new WORD[indicesN];
	setSphere();
}

Sphere::Sphere(int size){
	this->size = size;
	height = this->size;
	verticesN = size * height;
	indicesN = (size - 1) * height * 6;
	x0 = 0;
	y0 = 0;
	z0 = 0;
	vertices = new SimpleVertex[verticesN];
	indices = new WORD[indicesN];
	setSphere();
}

Sphere::Sphere(int size, float radius){
	this->size = size;
	height = this->size;
	this->radius = radius;
	verticesN = size * height;
	indicesN = (size - 1) * height * 6;
	x0 = 0;
	y0 = 0;
	z0 = 0;
	vertices = new SimpleVertex[verticesN];
	indices = new WORD[indicesN];
	setSphere();
}

Sphere::~Sphere(){
	delete[] vertices;
	delete[] indices;
}

int Sphere::getVerticesNumber(){
	return verticesN;
}

int Sphere::getIndicesNumber(){
	return indicesN;
}

SimpleVertex* Sphere::getVerteces(){
	return vertices;
}

WORD* Sphere::getIndices(){
	return indices;
}

void Sphere::setSphere(){
	setVertices();
	setIndices();
}

void Sphere::setVertices()
{
	for (int h = 0; h < height - 1; ++h)
	{
		double theta = (float)XM_PI / height * h;
		for (int i = 0; i < size; ++i)
		{
			double phi = (float)2 * XM_PI / size * i;
			XMVECTOR coord = { x0 + radius * cosf(phi) * sinf(theta), y0 + radius * cosf(theta), z0 + radius * sinf(phi) * sinf(theta) };
			XMVECTOR norm = XMVector3Normalize(coord);
			XMFLOAT3 coordF;
			XMFLOAT3 normF;
			XMStoreFloat3(&coordF, coord);
			XMStoreFloat3(&normF, norm);
			vertices[h * size + i] = { coordF, normF, color };
		}
	}
	double theta = XM_PI / height * (height - 1);
	for (int i = 0; i < size; ++i)
	{
		double phi = 2 * XM_PI / size * i;
		vertices[(height - 1) * size + i] = { XMFLOAT3(x0, y0 + radius * cosf(theta), z0), XMFLOAT3(x0, y0 + radius * cosf(theta), z0), color };
	}
}

void Sphere::setIndices()
{
	for (int h = 0; h < height - 1; ++h)
	{
		int j = size * (h + 1);
		for (int i = 0; i < size - 1; ++i)
		{
			indices[(h * size + i) * 6] = j;
			indices[(h * size + i) * 6 + 1] = j - size;
				indices[(h * size + i) * 6 + 2] = (j - size + 1);
			indices[(h * size + i) * 6 + 3] = (j - size + 1);
			indices[(h * size + i) * 6 + 4] = (j + 1);
			indices[(h * size + i) * 6 + 5] = j;
			++j;
		}
		indices[((h + 1) * size - 1) * 6] = j;
		indices[((h + 1) * size - 1) * 6 + 1] = (h + 1) * size - 1;
		indices[((h + 1) * size - 1) * 6 + 2] = h * size;
		indices[((h + 1) * size - 1) * 6 + 3] = h * size;
		indices[((h + 1) * size - 1) * 6 + 4] = (h + 1) * size;
		indices[((h + 1) * size - 1) * 6 + 5] = j;
	}
}