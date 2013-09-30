#include <exception>
#include <iostream>
#include <limits>

namespace {
	static volatile int lock; 
}

class PainterInterface { virtual int paint() = 0; };

template<typename T>
class Painter : public PainterInterface
{
	T * tp;
	public:
	Painter() { tp = new T(); lock=1; }
	virtual ~Painter() {};	
	virtual int paint() { 
		char * arr [4];

		try {
			arr[0] = new char[std::numeric_limits<int>::max()];
			arr[1] = new char[std::numeric_limits<int>::max()];
			arr[2] = new char[std::numeric_limits<int>::max()];
			arr[3] = new char[std::numeric_limits<int>::max()];
			arr[4] = new char[std::numeric_limits<int>::max()];
		} catch (std::exception & e)
		{
			std::cout << "OOM\n" ;
			delete [] arr[0];
			delete [] arr[1];
			delete [] arr[2];
			delete [] arr[3];
			delete [] arr[4];
			throw 0;
		}

		int i;
		for (i = 0; i < std::numeric_limits<int>::max();i++)
		{
			arr[0][0] = 'a';
			arr[1][0] = 'a';
			arr[2][0] = 'a';
			arr[3][0] = 'a';
			arr[4][0] = 'a';			
		}
		std::cout << i << std::endl;
		delete [] arr[0];
		delete [] arr[1];
		delete [] arr[2];
		delete [] arr[3];
		delete [] arr[4];
	}
	friend class PaintEngine;
};
