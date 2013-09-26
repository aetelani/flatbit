#include <paintengine.h>

class PaintInterface {
public:
	virtual	void paint() = 0;
	virtual int version() = 0;
};

template<typename T>
class Painter : public PaintInterface
{
public:
	Painter() {
		for (int i=10*10; i; i--)
		{
			b.push_back('-');
		}
	}
	virtual ~Painter() { b.clear(); }
	virtual int version();
	virtual	void paint();
private:
	int width, height, x, y;
	friend class PaintEngine;
	Bits b;
};
