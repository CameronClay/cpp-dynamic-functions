#pragma once
#include <stdlib.h>
#include <functional>

enum class CallingConvention { DEFAULT, CDECLR, STDCALL, FASTCALL, THISCALL };
template<CallingConvention> struct CCHelper;

template<> struct CCHelper<CallingConvention::DEFAULT>
{
	template<typename RT, typename... Args>
	using PFunc = RT(*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(O::*)(Args...) const;
};
template<> struct CCHelper<CallingConvention::CDECLR>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__cdecl *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__cdecl O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__cdecl O::*)(Args...) const;
};
template<> struct CCHelper<CallingConvention::STDCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__stdcall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__stdcall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__stdcall O::*)(Args...) const;
};
template<> struct CCHelper<CallingConvention::FASTCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__fastcall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__fastcall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__fastcall O::*)(Args...) const;
};
template<> struct CCHelper<CallingConvention::THISCALL>
{
	template<typename RT, typename... Args>
	using PFunc = RT(__thiscall *)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncM = RT(__thiscall O::*)(Args...);

	template<typename RT, typename O, typename... Args>
	using PFuncMC = RT(__thiscall O::*)(Args...) const;
};

template<CallingConvention CC, typename RT, typename... Args>
using PFuncCC = typename CCHelper<CC>::template PFunc<RT, Args...>;

template<CallingConvention CC, typename RT, typename O, typename... Args>
using PFuncMCC = typename CCHelper<CC>::template PFuncM<RT, O, Args...>;

template<CallingConvention CC, typename RT, typename O, typename... Args>
using PFuncMCCC = typename CCHelper<CC>::template PFuncMC<RT, O, Args...>;

template<typename RT, typename... Args>
using PFunc = PFuncCC<CallingConvention::DEFAULT, RT, Args...>;

template<typename RT, typename O, typename... Args>
using PFuncM = PFuncMCC<CallingConvention::DEFAULT, RT, O, Args...>;

template<typename RT, typename O, typename... Args>
using PFuncMC = PFuncMCCC<CallingConvention::DEFAULT, RT, O, Args...>;

template <typename FUNC>
struct is_function_ptr
	:
	std::integral_constant<bool, std::is_pointer<FUNC>::value &&
	std::is_function<std::remove_pointer_t<FUNC>>::value      &&
	!std::is_member_function_pointer<FUNC>::value>
{};

template<typename Signature> class Function;

//Store function call without arugments bound
template<typename RT, typename... Args>
class Function<RT(Args...)>
{
public:
	template<typename FUNC>
	Function(FUNC func, typename std::enable_if_t<!is_function_ptr<FUNC>::value>* = nullptr)
		: action(func)
	{}

	template<typename FUNC>
	Function(FUNC func, typename std::enable_if_t<is_function_ptr<FUNC>::value>* = nullptr)
		: action([func](Args&&... args)->RT {return (*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O>
	Function(FUNC func, O* o, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
		: action([func, o](Args&&... args)->RT {return (o->*func)(std::forward<Args>(args)...); })
	{}

	template<typename FUNC, typename O>
	Function(FUNC func, O& o, typename std::enable_if_t<std::is_member_function_pointer<FUNC>::value>* = nullptr)
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
		:
		action(MakeFunc(std::forward<Args>(boundArgs)...))
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

	template<typename FUNC, typename... Args>
	static std::enable_if_t<!is_function_ptr<FUNC>::value, Action> MakeFunc(FUNC func, Args&&... args)
	{
		auto f = [func](auto&&... args) -> RT
		{
			return func(std::forward<Args>(args)...);
		};
		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable { return std::apply(f, tup); };
	}

	template<typename FUNC, typename... Args>
	static std::enable_if_t<is_function_ptr<FUNC>::value, Action> MakeFunc(FUNC func, Args&&... args)
	{
		auto f = [func](auto&&... args) -> RT
		{
			return (*func)(std::forward<Args>(args)...);
		};
		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable { return std::apply(f, tup); };
	}

	template<typename FUNC, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer<FUNC>::value, Action> MakeFunc(FUNC func, O* o, Args&&... args)
	{
		auto f = [func, o](auto&&... args) -> RT
		{
			return (o->*func)(std::forward<Args>(args)...);
		};
		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable { return std::apply(f, tup); };
	}

	template<typename FUNC, typename O, typename... Args>
	static std::enable_if_t<std::is_member_function_pointer<FUNC>::value, Action> MakeFunc(FUNC func, O& o, Args&&... args)
	{
		auto f = [func, &o](auto&&... args) -> RT
		{
			return (o.*func)(std::forward<Args>(args)...);
		};
		return [f, tup = std::make_tuple(std::forward<Args>(args)...)]() mutable { return std::apply(f, tup); };
	}

	Action action;
};