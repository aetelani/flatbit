#include <vector>

#pragma once

typedef std::vector<char> Bits;

class PaintEngine { 
	void paint(int x, int y, int width, int height, Bits & bits ) {}
};

class PaintInterface {
public:
	virtual	void paint() = 0;
	virtual int version() = 0;
};

class Painter : public PaintInterface {
public:
	Painter();
	virtual ~Painter();
	virtual int version();
	virtual	void paint();
private:
	int width, height, x, y;
	friend class PaintEngine;

};
