#pragma once
#include <variant>
#include "Function.h"
#include "FunctionTraits.h"
#include "TypeList.h"

// Dummy structs for special return types
struct VOID {};
struct NO_CALL {};

// Store any function given a list of signatures, of the format RT(Args...)
// Duplicate signatures are removed from the variant
// Reference return types are converted to pointer return types (T& -> T*)
template<typename... Sigs>
class FunctionAny
{
private:
	//Convert references to pointers and void to VOID
	template<typename RT>
	using TO_RETURN_TYPE = std::conditional_t<std::is_lvalue_reference_v<RT>, std::add_pointer_t<std::remove_reference_t<RT>>, std::conditional_t<std::is_void_v<RT>, VOID, RT>>;
public:
	using SIGS_UNIQUE    = t_list::type_list_unique<Sigs...>;
	using RTS_UNIQUE     = t_list::type_list_unique<t_list::type_list_apply_t<t_list::type_list_apply_t<SIGS_UNIQUE, f_traits::sig_rt_t>, TO_RETURN_TYPE>, NO_CALL>;
	using RT_VARIANT     = t_list::rebind_t<RTS_UNIQUE, std::variant>;

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

	// Invokes func IF all Args are convertible to that of the function signature and calls std::visit on the visitor with the return value
	template<typename Visitor, typename... Args>
	decltype(auto) Invoke(Visitor&& visitor, Args&&... args) const
	{
		auto ret = operator()(std::forward<Args>(args)...);
		return std::visit(std::forward<Visitor>(visitor), ret);
	}

	// Invokes function with supplied parameters if Args are convertible to that of the function signature
	// Returns VOID instead of void, and NO_CALL if the function expected arguments
	template<typename... Args>
	auto operator()(Args&&... args) const -> RT_VARIANT
	{
		auto f = [](const auto& func, auto&&... args) -> decltype(auto)
		{
			return InvokeFunction(func, std::forward<decltype(args)>(args)...);
		};

		auto call = [f, tup = std::make_tuple(std::forward<Args>(args)...)](const auto& func) mutable -> decltype(auto) 
		{ 
			using Sig = f_traits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			if constexpr (f_traits::sig_convertible_args_v<Sig, Args...>)
				return RT_VARIANT(apply_first(f, func, tup));
			else
				return RT_VARIANT(NO_CALL{});
		};

		return std::visit(call, func);
	}

	// Invokes func if it has EXACTLY zero args
	// Returns VOID for void, and NO_CALL if the function expected arguments
	auto operator()() const -> RT_VARIANT
	{
		auto call = [](const auto& func) -> decltype(auto)
		{
			using Sig = f_traits::Function_to_sig_t<std::decay_t<decltype(func)>>;
			if constexpr (f_traits::sig_no_args_v<Sig>)
				return InvokeFunction(func);
			else
				return RT_VARIANT(NO_CALL{});
		};

		return std::visit(call, func);
	}

	// Checks to see if a valid function is stored
	operator bool() const
	{
		auto call = [](const auto& func) -> bool
		{
			return func.operator bool();
		};

		return std::visit(call, func);
	}

private:
	using FSIGS_UNIQUE = t_list::type_list_apply_t<SIGS_UNIQUE, Function>;
	t_list::rebind_t<FSIGS_UNIQUE, std::variant> func;

	// Invokes function with supplied parameters if Args are convertible to that of the function signature
	// Returns VOID instead of void and pointer instead of reference
	template<typename Sig, typename... Args>
	static decltype(auto) InvokeFunction(const Function<Sig>& func, Args&&... args)
	{
		using RT = f_traits::sig_rt_t<Sig>;
		if constexpr (!std::is_same_v<RT, void>)
		{
			decltype(auto) ret = func(std::forward<Args>(args)...);
			if constexpr (!std::is_lvalue_reference_v<RT>)
				return RT_VARIANT(ret);
			else
				return RT_VARIANT(&ret);
		}
		else
		{
			func(std::forward<Args>(args)...);
		}

		return RT_VARIANT(VOID{});
	}

	template <class F, class T, class Tuple, std::size_t... I>
	static constexpr decltype(auto) apply_first_impli(F&& f, T&& first, Tuple&& t, std::index_sequence<I...>)
	{
		return std::invoke(std::forward<F>(f), std::forward<T>(first), std::get<I>(std::forward<Tuple>(t))...);
	}

	template <class F, class T, class Tuple>
	static constexpr decltype(auto) apply_first(F&& f, T&& first, Tuple&& t)
	{
		return apply_first_impli(std::forward<F>(f), std::forward<T>(first), std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
	}
};

namespace FAny_Utili
{
	template <typename SigPair> struct pair_create_sig;
	template <typename RT, typename... Args>
	struct pair_create_sig<t_list::pair<RT, t_list::type_list<Args...>>>
	{
		using type = f_traits::sig_create<RT, Args...>;
	};

	template <typename SigPair>
	using pair_create_sig_t = typename pair_create_sig<SigPair>::type;
}

// FunctionAny alias, taking any signatures contained in Sig_TLists
// Sig_TLists are type_list<Sigs...>....
template <class... Sig_Tists>
using FunctionAny_Sig_Lists = t_list::rebind_t<t_list::type_list_unique<Sig_Tists...>, FunctionAny>;

// FunctionAny alias, taking all signatures in the cartesian product of all RTs in RTTList and all Args in ArgTLists
// RTList is a type_list<RTs...>, ArgTLists is a type_list<type_list<Args...>....>
template <class RTTList, class ArgTLists>
using FunctionAny_RT_Args = t_list::rebind_t<t_list::type_list_apply_t<t_list::cartesian_product_t<RTTList, ArgTLists>, FAny_Utili::pair_create_sig_t>, FunctionAny>;