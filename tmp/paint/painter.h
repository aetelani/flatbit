
class PainterInterface { virtual int paint() = 0; };

template<typename T>
class Painter : public PainterInterface
{
	public:
	Painter() { tp = new T(); }
	virtual ~Painter() {};
	T * tp;
	virtual int paint() { 
		
	}
	friend class PaintEngine;
};
