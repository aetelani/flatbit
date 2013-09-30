
#include <painter.h>
#include <iostream>
#include <assert.h>


#include <list>

struct hop { char * say; };


template <typename T> class Elain
{
	int sanoo() { std::cout << "jotain" << std::endl; }
} pop;

int main()
{
	struct hop kana;
	kana.say = "kotkot";
	
	Painter<int> * p = new Painter<int>();
	p && std::cout << p << std::endl;
	std::list<int> * l = new std::list<int>();
	for (int i=10; i; i--)
		l->push_back(i);
	
	for (std::list<int>::iterator it = l->begin(); it != l->end(); it++)
		std::cout << *it << std::endl;
		
	l->sort();

	for (std::list<int>::iterator it = l->begin(); it != l->end(); it++)
		std::cout << *it << std::endl;	
	
	/*assert(p);
//	std::cout << "version: " << p->version() << endl;
	p->paint();
	delete p;*/
}
