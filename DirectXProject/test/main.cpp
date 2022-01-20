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


int List[] = { 0,1,2,3,4 };

int* pList[5];
void Init()
{
	for (int cnt = 0; auto & element : List)
	{
		pList[cnt] = List + cnt;
		cnt++;
	}
}

const int*& Get(int index)
{
	return pList[index];
}

int main()
{
	using std::cout;
	using std::endl;
	Init();

	const int* buff = Get(0);
	buff = Get(1);
	for (int cnt = 0; auto& elem : List)
	{
		cout << cnt << ": " << &(Get(elem)) << endl;
		cout << cnt << ": " << &List[cnt] << endl;
		cnt++;
	}

	std::system("pause");

	return 0;
}
