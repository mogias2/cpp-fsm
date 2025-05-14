#pragma once

class NoneClass
{
};

template <class T, class TBase = NoneClass>
class Container : public TBase
{
public:
	explicit Container(T& _container) : TBase(), container_(_container), p_container_(&_container) {}
	explicit Container(T* _p_container) : TBase(), container_(*_p_container), p_container_(_p_container) {}
	~Container() {}

	T& GetContainer() { return container_; }
	const T& GetContainer() const { return container_; }
	T* GetContainerPtr() { return p_container_; }
	const T* GetContainerPtr() const { return p_container_; }

protected:
	T& container_;
	T* p_container_;
};
