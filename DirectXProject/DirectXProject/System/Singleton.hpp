#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__


template <class T>
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
public:
	static T* GetInstance()
	{
		return m_pInstance;
	}
	static void CreateInstance()
	{
		DestroyInstance();
		m_pInstance = new T;
	}
	static void DestroyInstance()
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static T* m_pInstance;
};

template<class T> T* Singleton<T>::m_pInstance = nullptr;


#endif // __SINGLETON_HPP__