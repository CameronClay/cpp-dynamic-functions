#pragma once
#include <variant>
#include "Function.h"

template<typename Signature> struct is_funcs : std::false_type {};

template<typename RT>
struct is_funcs<RT()> : std::true_type {};

template<typename... SigList>
class FunctionAny
{
public:
	FunctionAny() = default;

	template<typename Sig, typename... Args>
	FunctionAny(Args&&... args)
		:
		func(std::in_place_type_t<Sig>, std::forward<Args>(args)...)
	{}

	template<typename Signature>
	FunctionAny(Function<Signature>&& rhs)
		:
		func(std::move(rhs))
	{}

	template<typename... Args>
	auto operator()(Args&&... args) const
	{
		auto f = [](const auto& func, auto&&... args)
		{
			using sig = std::decay_t<decltype(func)>;
			if constexpr (!is_funcs<sig>::value)
				return func(std::forward<Args>(args)...);
		};

		auto call = [f, tup = std::make_tuple(std::forward<Args>(args)...)](const auto& func) mutable { return std::apply(f, std::tuple_cat(std::make_tuple(std::ref(func)), tup)); };

		return std::visit(call, func);
	}

	auto operator()() const
	{
		auto call = [](const auto& func)
		{
			using sig = std::decay_t<decltype(func)>;
			//static_assert(!is_funcs<sig>::value, "Error Function does not take 0 arguments");
			if constexpr (is_funcs<sig>::value)
			{
				std::cout << "sfunc" << std::endl;
				return func();
			}
			else
			{
				std::cout << "not sfunc" << std::endl;
			}
		};

		return std::visit(call, func);
	}

	operator bool() const
	{
		auto call = [](const auto& func) -> bool
		{
			return func.operator bool();
		};

		return std::visit(call, func);
	}

private:
	std::variant<Function<SigList>...> func;
};