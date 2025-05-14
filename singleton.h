#pragma once

template <class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

	static const T& GetConstInstance()
	{
		return GetInstance();
	}

	static T* GetPointer()
	{
		return &GetInstance();
	}

protected:
	Singleton() {}
	~Singleton() {}

private:
	DISALLOW_COPY_AND_ASSIGN(Singleton);
};
