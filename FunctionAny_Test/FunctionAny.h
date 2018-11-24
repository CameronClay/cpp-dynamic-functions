#pragma once
#include <variant>
#include <any>
#include "Function.h"

template <class F, class T, class Tuple, std::size_t... I>
inline constexpr decltype(auto) apply_first_impli(F&& f, T&& first, Tuple&& t, std::index_sequence<I...>)
{
	return std::invoke(std::forward<F>(f), std::forward<T>(first), std::get<I>(std::forward<Tuple>(t))...);
}

template <class F, class T, class Tuple>
inline constexpr decltype(auto) apply_first(F&& f, T&& first, Tuple&& t)
{
	return apply_first_impli(std::forward<F>(f), std::forward<T>(first), std::forward<Tuple>(t),
		std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template<typename... Sigs>
class FunctionAny
{
public:
	FunctionAny() = default;

	template<typename Sig>
	FunctionAny(Function<Sig>&& rhs)
		:
		func(std::move(rhs))
	{}

	template<typename Sig, typename... Args>
	explicit FunctionAny(std::in_place_type_t<Sig>, Args&&... args)
		:
		func(std::in_place_type<Function<Sig>>, std::forward<Args>(args)...)
	{}

	template<std::size_t I, typename... Args>
	explicit FunctionAny(std::in_place_index_t<I>, Args&&... args)
		:
		func(std::in_place_index<I>, std::forward<Args>(args)...)
	{}

	template<typename... Args>
	decltype(auto) operator()(Args&&... args) const
	{
		auto f = [](const auto& func, auto&&... args) -> decltype(auto)
		{
			return func(std::forward<decltype(args)>(args)...);
			//return std::any( func(std::forward<decltype(args)>(args)...) );
		};

		auto call = [f, tup = std::make_tuple(std::forward<Args>(args)...)](const auto& func) mutable -> decltype(auto) 
		{ 
			using Sig = ftraits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			if constexpr (ftraits::sig_nparams_v<Sig> == sizeof...(Args))
				return apply_first(f, func, tup); 
		};

		return std::visit(call, func);
	}

	decltype(auto) operator()() const
	{
		auto call = [](const auto& func) -> decltype(auto)
		{
			using Sig = ftraits::Function_to_sig_t<std::decay_t<decltype(func)>>;

			if constexpr (ftraits::is_funcs_v<Sig>)
				return func();
				//return std::any( func() );

			//return std::any{};
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
	std::variant<Function<Sigs>...> func;
};