#pragma once

namespace exemodel {


// base class for something we can "call"
class callable {
public:
	virtual void operator()() = 0;
	virtual ~callable() {}
};

// wraps pointer-to-members
template<typename T>
class callable_from_object : public callable {
public:
	callable_from_object(T& object, void (T::*method)(uint32_t evts))
	: o(object), m(method) {}

	void operator()() {
		(&o->*m) ();
	}
private:
	T & o;
	void (T::*m)();
};

// wraps pointer-to-functions or pointer-to-static-members
class callable_from_function : public callable {
public:
	callable_from_function(void (*function)())
	: f(function) {}

	void operator()() {
		f();
	};
private:
	void (*f)();
};

}
