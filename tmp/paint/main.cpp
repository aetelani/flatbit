
#include <painter.h>
#include <iostream>
#include <assert.h>

using namespace std;
int main()
{
		
	Painter * p = new Painter();
	assert(p);
	std::cout << "version: " << p->version() << endl;
	p->paint();
	delete p;
}
