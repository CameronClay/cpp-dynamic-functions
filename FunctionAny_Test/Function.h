#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdlib.h>
#include <functional>
#include "FunctionTraits.h"

template<typename Sig> class Function;

//Store function call without arugments bound
template<typename RT, typename... Args>
class Function<RT(Args...)>
{
public:
	template<typename Func>
	Function(Func func)
		: action(func)
	{}

	template<typename Func, typename O>
	Function(Func func, O* o, typename std::enable_if_t<std::is_member_function_pointer_v<Func>>* = nullptr)
		: action([func, o](Args&&... args)->RT {return (o->*func)(std::forward<Args>(args)...); })
	{}

	template<typename Func, typename O>
	Function(Func func, O& o, typename std::enable_if_t<std::is_member_function_pointer_v<Func>>* = nullptr)
		: action([func, &o](Args&&... args)->RT {return (o.*func)(std::forward<Args>(args)...); })
	{}

	operator bool() const
	{
		return bool(action);
	}

	decltype(auto) operator()(Args&&... args) const
	{
		return action(std::forward<Args>(args)...);
	}

private:
	std::function<RT(Args...)> action;
};

//Store function call with all arguments bound
template<typename RT>
class Function<RT()>
{
public:
	template<typename... Args>
	Function(Args&&... boundArgs)
		: action(MakeFunc(std::forward<Args>(boundArgs)...))
	{}

	operator bool() const
	{
		return bool(action);
	}

	decltype(auto) operator()() const
	{
		return action();
	}

private:
	using Action = std::function<RT()>;

	template<typename Func, typename... Args>
	static std::enable_if_t<!f_traits::is_function_ptr_v<Func>, Action> MakeFunc(Func func, Args&&... args)
	{
		return [func, ...args = std::forward<Args>(args)]() mutable -> RT {return func(std::forward<decltype(args)>(args)...);};
	}

	template<typename Func, typename... Args>
	static std::enable_if_t<f_traits::is_function_ptr_v<Func>, Action> MakeFunc(Func func, Args&&... args)
	{
		return [func, ...args = std::forward<Args>(args)]() mutable -> RT {return (*func)(std::forward<decltype(args)>(args)...);};
	}

	template<typename Func, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer_v<Func>, Action> MakeFunc(Func func, O* o, Args&&... args)
	{
		return [o, func, ...args = std::forward<Args>(args)]() mutable -> RT {return (o->*func)(std::forward<decltype(args)>(args)...);};
	}

	template<typename Func, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer_v<Func>, Action> MakeFunc(Func func, O& o, Args&&... args)
	{
	    return [&o, func, ...args = std::forward<Args>(args)]() mutable -> RT {return (o.*func)(std::forward<decltype(args)>(args)...);};
	}

	// Used for constructors
	static Action MakeFunc()
	{
		return {};
	}
	static Action MakeFunc(const Function<RT>& func)
	{
		return func;
	}
	static Action MakeFunc(Function<RT>&& func)
	{
		return func;
	}

	Action action;
};

#endif