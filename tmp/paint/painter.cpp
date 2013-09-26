#include <painter.h>
#include <paintengine.h>
#include <iostream>


template<typename T>
int Painter<T>::version()
{
	return 0;
}

template<typename T>
void Painter<T>::paint()
{
	for (Bits::iterator it = b.begin(); it != b.end(); it++)
		std::cout << *it;

	std::cout << "\n";
}
