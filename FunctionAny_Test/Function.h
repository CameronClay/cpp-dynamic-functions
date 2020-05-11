#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdlib.h>
#include <functional>
#include "FunctionTraits.h"

template <class Func>
concept MemFn = std::is_member_function_pointer_v<Func>;

template<typename Sig> class Function;

//Store function call with some arguments bound
template<typename RT, typename... UnboundArgs>
class Function<RT(UnboundArgs...)>
{
public:
	template<typename... BoundArgs>
	Function(BoundArgs&&... args)
		: action(MakeFunc(std::forward<BoundArgs>(args)...))
	{}

	operator bool() const
	{
		return bool(action);
	}

	decltype(auto) operator()(UnboundArgs&&... args) const
	{
		return action(std::forward<UnboundArgs>(args)...);
	}

private:
	using Action = std::function<RT(UnboundArgs...)>;

	template<typename Func, typename... BoundArgs>
	static Action MakeFunc(Func func, BoundArgs&&... args)
	{
		return [func, ...args = std::forward<BoundArgs>(args)](UnboundArgs&&... rest) mutable -> decltype(auto) {
			return func(std::forward<decltype(args)>(args)..., std::forward<UnboundArgs>(rest)...);
		};
	}

	template<typename Func, typename O, typename... BoundArgs>
	static Action MakeFunc(Func func, O* o, BoundArgs&&... args) requires MemFn<Func>
	{
		return [o, func, ...args = std::forward<BoundArgs>(args)](UnboundArgs&&... rest) mutable -> decltype(auto) {
			return (o->*func)(std::forward<decltype(args)>(args)..., std::forward<UnboundArgs>(rest)...);
		};
	}

	template<typename Func, typename O, typename... BoundArgs>
	static Action MakeFunc(Func func, O& o, BoundArgs&&... args) requires MemFn<Func>
	{
	    return [&o, func, ...args = std::forward<BoundArgs>(args)](UnboundArgs&&... rest) mutable -> decltype(auto) {
			return (o.*func)(std::forward<decltype(args)>(args)..., std::forward<UnboundArgs>(rest)...);
		};
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