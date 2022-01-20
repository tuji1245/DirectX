#include <stdio.h>
#include <iostream>
#include <vector>

#define TO_STRING(valiablename) # valiablename

class MyEnum
{
public:
	enum class Base
	{
		end_of_enum = 0
	};

public:

	std::vector<std::string> m_list;

};

class MyEnum2 : public MyEnum
{
	enum class Drive
	{
		One = static_cast<uint8_t>(Base::end_of_enum),
		Two,
	};
};

int main()
{
	using std::cout;
	using std::endl;

	int* base = new int(1);
	const int* pointer = base;

	auto printBase = [&]() {
		cout << "base: " << base << ": " << *base << endl;
	};
	auto printPointer = [&]() {
		if (pointer)
			cout << "pointer: " << pointer << ": " << *pointer << endl;
		else
			cout << "NOOOOO" << endl;
	};

	printBase();
	printPointer();

	delete pointer, pointer = nullptr;
	cout << " -- pointer is deleted -- " << endl;

	printBase();
	printPointer();

	std::system("pause");

	return 0;
}
