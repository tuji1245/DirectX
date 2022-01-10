#include <stdio.h>
#include <iostream>
#include <vector>

class base
{
public:
	base() = default;
	virtual ~base() = default;
};

class drive : public base
{
public:
	drive() = default;
	~drive() = default;
};

std::vector<base*> g_listpBase;

template <typename T>
bool comp()
{
	for (auto element : g_listpBase)
	{
		std::cout << typeid(*element).name() << std::endl;
		if (typeid(*element) == T)
		{
			std::cout << "succes" << std::endl;
			return true;
		}
	}
	std::cout << "failed" << std::endl;
	return false;
}

int main()
{


	std::system("pause");

	return 0;
}
