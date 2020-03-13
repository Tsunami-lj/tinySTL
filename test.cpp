#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include "alloc.h"
using namespace std;
using namespace mystl;
class Test {
	static int count;
	int No;
public:
	Test() {
		No = count;
		count++;
	} 
	void print() {
		cout << this << ":";
		cout << "the " << No << "th object" << endl;
	}
	void* operator new(size_t size);
	void operator delete(void* p);
};

void *Test::operator new(size_t n) {
	return Alloc().allocate(n); 
}

void Test::operator delete(void* p) {
	Alloc().deallocate(p, sizeof(p));
}

int Test::count = 0;
int main() {
	Test* p1 = new Test();
	p1->print();

	Test* p2 = new Test();
	p2->print();
	delete p1;

	p1 = new Test();
	p1->print();

	Test *p3 = new Test();
	p3->print();
	delete p1;
	delete p2;
	delete p3;
	return 0;
}
