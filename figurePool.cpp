#include "figurePool.h"

FigurePool::~FigurePool() {
	for (int i = 0; i < figures.size(); ++i) {
		delete figures[i];
	}
}

void FigurePool::add(Figure* figure) {
	figures.push_back(figure);
}

int FigurePool::getSize() {
	return figures.size();
}

Figure* FigurePool::getFigures(int i) {
	return figures.at(i);
}