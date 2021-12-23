#pragma once

class base
{
	template<typename T>
	T* add(T*);
};

template<typename T>
inline T* base::add(T*)
{
	return nullptr;
}
