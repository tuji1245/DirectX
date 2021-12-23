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

void Print(int buff)
{
	std::cout << buff << std::endl;
}

int main()
{
	drive a[10];
	for (auto element : a)
		g_listpBase.push_back(&element);
	
	Print(g_listpBase.size());

	const int start = 3;
	const int max = 7;
	for (int i = start; i < max; i++)
		g_listpBase[i] = nullptr;

	
	Print(g_listpBase.size());
	
	g_listpBase.shrink_to_fit();
	Print(g_listpBase.size());


	std::system("pause");

	return 0;
}
