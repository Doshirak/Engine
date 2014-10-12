#pragma once
#include <vector>
#include "figure.h"

class FigurePool {
public:
	~FigurePool();
	void add(Figure* figure);
	int getSize();
	Figure** getFigures();
private:
	std::vector<Figure*> figures;
};