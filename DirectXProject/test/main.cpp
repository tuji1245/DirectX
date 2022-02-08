#include <stdio.h>
#include <iostream>
#include <memory>

#define TO_STRING(valiablename) # valiablename

void print(void* vp)
{
	std::cout << "vp: " << vp << std::endl;
	int* ip = static_cast<int*>(vp);
	std::cout << "ip: " << ip << std::endl;
	std::cout << "*ip: " << *ip << std::endl;
}

int main()
{
	using std::cout;
	using std::endl;

	std::shared_ptr<int> sp = std::make_shared<int>(10);
	std::weak_ptr<int> wp = sp;

	cout << "sp: " << *sp << endl;
	cout << "wp: " << *wp.lock() << endl;

	print(&*wp.lock());
	print(wp.lock().get());

	std::system("pause");

	return 0;
}
