#pragma once
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
	Function(Func func, typename std::enable_if_t<!f_traits::is_function_ptr_v<Func>>* = nullptr)
		: action(func)
	{}

	template<typename Func>
	Function(Func func, typename std::enable_if_t<f_traits::is_function_ptr_v<Func>>* = nullptr)
		: action([func](Args&&... args)->RT {return (*func)(std::forward<Args>(args)...); })
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
		return action.operator bool();
	}

	auto operator()(Args... args) const -> RT
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
		return action.operator bool();
	}

	auto operator()() const -> RT
	{
		return action();
	}

public:
	using Action = std::function<RT()>;

	template<typename Func, typename... Args>
	static std::enable_if_t<!f_traits::is_function_ptr_v<Func>, Action> MakeFunc(Func func, Args&&... args)
	{
		auto f = [func](auto&&... args) -> RT {return func(std::forward<decltype(args)>(args)...);};

		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable -> RT { return std::apply(f, tup); };
	}

	template<typename Func, typename... Args>
	static std::enable_if_t<f_traits::is_function_ptr_v<Func>, Action> MakeFunc(Func func, Args&&... args)
	{
		auto f = [func](auto&&... args) -> RT {return (*func)(std::forward<decltype(args)>(args)...);};

		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable -> RT { return std::apply(f, tup); };
	}

	template<typename Func, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer_v<Func>, Action> MakeFunc(Func func, O* o, Args&&... args)
	{
		auto f = [func, o](auto&&... args) -> RT {return (o->*func)(std::forward<decltype(args)>(args)...);};

		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable -> RT { return std::apply(f, tup); };
	}

	template<typename Func, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer_v<Func>, Action> MakeFunc(Func func, O& o, Args&&... args)
	{
		auto f = [func, &o](auto&&... args) -> RT {return (o.*func)(std::forward<decltype(args)>(args)...);};

		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable -> RT { return std::apply(f, tup); };
	}

	Action action;
};