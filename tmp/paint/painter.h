#include <paintengine.h>

#pragma once

class PaintInterface {
public:
	virtual	void paint() = 0;
	virtual int version() = 0;
};

//template<typename T>
class Painter : public PaintInterface
{
public:
	Painter();
	virtual ~Painter();
	virtual int version();
	virtual	void paint();
private:
	int width, height, x, y;
	friend class PaintEngine;

};
