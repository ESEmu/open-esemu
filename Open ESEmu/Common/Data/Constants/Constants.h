#pragma once
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

// http://stackoverflow.com/questions/24702235/c-stdmap-holding-any-type-of-value
// By Neil Kirk

class AnyBase
{
public:
	virtual ~AnyBase() = 0;
};
inline AnyBase::~AnyBase() {}

template<class T>
class Any : public AnyBase
{
public:
	typedef T Type;
	explicit Any(const Type& data) : data(data) {}
	Any() {}
	Type data;
};

class ConstantsHolder
{
private:

	map<string, unique_ptr<AnyBase>> _container;

public:


	ConstantsHolder()
	{
		this->set<string>("OPENESEMU_VERSION", "1.01");
		this->set<string>("OPENESEMU_AUTHOR", "d3vil401");
	}

	~ConstantsHolder()
	{
		this->_container.clear();
	}

	template<class T>
	T get(string _field)
	{
		try {
			return dynamic_cast<Any<T>&>(*_container[_field]).data;
		} catch (std::bad_cast ex) {
			printf("[!] %s bad cast thrown, %s.\n", _field.c_str(), ex.what());
			return nullptr;
		}
	}

	template<class T>
	void set(string _field, T data)
	{
		_container[_field].reset(new Any<T>(data));
	}

	void clear()
	{
		this->_container.clear();
	}

};

extern ConstantsHolder* g_ConstantsHolder;

#endif