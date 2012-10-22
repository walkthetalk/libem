#include <stdio.h>
#include <map>

using namespace std;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename Result, typename ...Args>
class Slot
{
public:
	virtual Result call(Args... args) = 0;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename T, typename Result, typename ...Args>
class SlotObjectHandle : public Slot<Result, Args...>
{
private:
	typedef Result (T::*MemPtr)(Args...);
	MemPtr memPtr;
	T* handlePtr;
	
public:
	SlotObjectHandle(T* _handlePtr, MemPtr _memPtr)
	{
		handlePtr = _handlePtr;
		memPtr = _memPtr;
	}
	
	Result call(Args... args)
	{
		return ((handlePtr)->*(memPtr))(args...);
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename Result, typename ...Args>
class SlotFunctionHandle : public Slot<Result, Args...>
{
private:
	typedef Result (*FuncPtr)(Args...);
	FuncPtr funcPtr;
	
public:
	SlotFunctionHandle(FuncPtr _funcPtr)
	{
		funcPtr = _funcPtr;
	}
	
	Result call(Args... args)
	{
		return (*funcPtr)(args...);
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename Result, typename ... Args>
class SlotProxy
{
private:
	typedef Result (Slot<Result,Args...>::*SlotMemPtr)(Args...);
	typedef Result (*FuncPtr)(Args...);
	void*      objP;
	SlotMemPtr slotMemPtr;
	FuncPtr    funcPtr;
	
public:
	SlotProxy(void* _objP, SlotMemPtr ptr)
	{
		objP = _objP;
		slotMemPtr = ptr;
		funcPtr = NULL;
	}
	SlotProxy(FuncPtr ptr)
	{
		objP = NULL;
		funcPtr = ptr;
	}
	
	bool operator < (const SlotProxy<Result, Args...>& proxyRef) const
	{
		bool result = false;
		
		if(funcPtr && proxyRef.funcPtr && (funcPtr != proxyRef.funcPtr))
		{
			result = this < &proxyRef;
		}
		else if(objP && proxyRef.objP)
		{
			if(objP != proxyRef.objP)
				return objP < proxyRef.objP;
			else
				return slotMemPtr != proxyRef.slotMemPtr;
		}
		else
			result = funcPtr ? true: false;
		
		return result;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template<typename Result, typename ... Args>
class Signal
{
private:
	typedef Result (Slot<Result,Args...>::*SlotMemPtr)(Args...);
	typedef map<SlotProxy<Result,Args...>, Slot<Result, Args...>*> SlotPtrMap;
	
	SlotPtrMap slotPtrMap;
	
public:
	Signal()
	{
	}
	
	~Signal()
	{
		disconnect();
	}
	
	Result operator ()(Args... args)
	{
		Result r;
		for(typename SlotPtrMap::iterator i = slotPtrMap.begin();
		    i != slotPtrMap.end();
		i++)
		    r = i->second->call(args...);
		    
		    return r;
	}
	
	template<class T>
	void connect(T* objPtr, Result (T::*memPtr)(Args...))
	{
		SlotObjectHandle<T,Result,Args...>* handleP
		= new SlotObjectHandle<T,Result,Args...>(objPtr, memPtr);
		slotPtrMap[SlotProxy<Result,Args...>(objPtr, (SlotMemPtr)memPtr)] = handleP;
	}
	
	void connect(Result (*funcPtr)(Args...))
	{
		slotPtrMap[SlotProxy<Result, Args...>(funcPtr)]
		= new SlotFunctionHandle<Result,Args...>(funcPtr);
	}
	
	void disconnect()
	{
		slotPtrMap.erase(slotPtrMap.begin(), slotPtrMap.end());
	}
	
	template<class T>
	void disconnect(T* objPtr, Result (T::*memPtr)(Args...))
	{
		slotPtrMap.erase(SlotProxy<Result,Args...>(objPtr, (SlotMemPtr)memPtr));
	}
	
	void disconnect(Result (*funcPtr)(Args...))
	{
		slotPtrMap.erase(SlotProxy<Result,Args...>(funcPtr));
	}
}; 
