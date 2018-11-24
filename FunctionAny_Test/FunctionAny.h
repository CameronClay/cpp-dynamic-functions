#pragma once
#include <variant>
#include <any>
#include "Function.h"
#include "FunctionTraits.h"
#include "TypeList.h"

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

struct VOID {};
struct NO_CALL {};

template<typename... Sigs>
class FunctionAny
{
private:
	template<typename RT>
	using TO_VARIANT_TYPE = std::conditional_t<std::is_void_v<RT>, VOID, RT>;

	using SIGS_UNIQUE = t_list::type_list_unique<Function<Sigs>...>;
	using RTS_UNIQUE = t_list::type_list_unique<NO_CALL, TO_VARIANT_TYPE<ftraits::sig_rt_t<Sigs>>...>;

public:
	using RTs = t_list::rebind_t<RTS_UNIQUE, std::variant>;

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

	// Invokes func IF all Args match EXACTLY that of the function signature (eval at compile time)
	// Returns VOID for void, and NO_CALL if the function signature did not match
	template<typename... Args>
	auto operator()(Args&&... args) const -> RTs
	{
		auto f = [](const auto& func, auto&&... args) -> decltype(auto)
		{
			using Sig = ftraits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			using RT = ftraits::sig_rt_t<Sig>;
			if constexpr (!std::is_same_v<RT, void>)
				return RTs(func(std::forward<decltype(args)>(args)...));
			else
				func(std::forward<decltype(args)>(args)...);

			return RTs( VOID{} );
		};

		auto call = [f, tup = std::make_tuple(std::forward<Args>(args)...)](const auto& func) mutable -> decltype(auto) 
		{ 
			using Sig = ftraits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			//if constexpr (ftraits::sig_nparams_v<Sig> == sizeof...(Args))
			if constexpr (ftraits::sig_same_args_v<Sig, Args...>)
				return apply_first(f, func, tup); 

			return RTs( NO_CALL{} );
		};

		return std::visit(call, func);
	}

	// Invokes func if it has EXACTLY zero args (eval at compile time)
	// Returns VOID for void, and NO_CALL if the function expected arguments
	auto operator()() const -> RTs
	{
		auto call = [](const auto& func) -> decltype(auto)
		{
			using Sig = ftraits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			using RT = ftraits::sig_rt_t<Sig>;

			if constexpr (ftraits::sig_n_args_v<Sig> == 0)
			{
				if constexpr (!std::is_same_v<RT, void>)
					return RTs(func());
				else
					func();

				return RTs( VOID{} );
			}

			return RTs( NO_CALL{} );
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
	t_list::rebind_t<SIGS_UNIQUE, std::variant> func;
};