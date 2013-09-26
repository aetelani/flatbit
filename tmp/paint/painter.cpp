#include <painter.h>
#include <paintengine.h>
#include <iostream>

namespace  {
	Bits b;
}

Painter::Painter()
{
	/*for (int i=10*10; i; i--)
	{
			b.push_back('-');
	}*/
}


Painter::~Painter()
{
	b.clear();
}

int Painter::version()
{
	return 0;
}

void Painter::paint()
{
	for (Bits::iterator it = b.begin(); it != b.end(); it++)
		std::cout << *it;

	std::cout << "\n";
}
