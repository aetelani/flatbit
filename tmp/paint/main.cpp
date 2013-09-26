
#include <painter.h>
#include <iostream>
#include <assert.h>
#include <paintengine.h>

using namespace std;
int main()
{
		
	Painter<int> * p = new Painter<int>();
	assert(p);
//	std::cout << "version: " << p->version() << endl;
	p->paint();
	delete p;
}
