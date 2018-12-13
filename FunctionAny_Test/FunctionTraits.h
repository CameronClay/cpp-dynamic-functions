#pragma once
#include <type_traits>
#include "TypeList.h"

template<typename Sig> class Function;

namespace f_traits
{
	enum class CallingConvention { DEFAULT, CDECLR, STDCALL, FASTCALL, THISCALL };
	template<CallingConvention> struct CCHelper;

	template<> struct CCHelper<CallingConvention::DEFAULT>
	{
		template<typename RT, typename... Args>
		using PFunc  =  RT(   *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::CDECLR>
	{
		template<typename RT, typename... Args>
		using PFunc  =  RT(__cdecl    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__cdecl O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__cdecl O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::STDCALL>
	{
		template<typename RT, typename... Args>
		using PFunc  =  RT(__stdcall    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__stdcall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__stdcall O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::FASTCALL>
	{
		template<typename RT, typename... Args>
		using PFunc  =  RT(__fastcall    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__fastcall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__fastcall O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::THISCALL>
	{
		template<typename RT, typename... Args>
		using PFunc  =  RT(__thiscall    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__thiscall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__thiscall O::*)(Args...) const;
	};

	template<CallingConvention CC, typename RT,             typename... Args>
	using PFuncCC =	  typename CCHelper<CC>::template PFunc<RT,      Args...>;
	template<CallingConvention CC, typename RT, typename O, typename... Args>
	using PFuncMCC =  typename CCHelper<CC>::template PFuncM<RT, O,  Args...>;
	template<CallingConvention CC, typename RT, typename O, typename... Args>
	using PFuncMCCC = typename CCHelper<CC>::template PFuncMC<RT, O, Args...>;

	template<typename RT,             typename... Args>
	using PFunc =   PFuncCC<CallingConvention::DEFAULT,   RT,    Args...>;
	template<typename RT, typename O, typename... Args>
	using PFuncM =  PFuncMCC<CallingConvention::DEFAULT,  RT, O, Args...>;
	template<typename RT, typename O, typename... Args>
	using PFuncMC = PFuncMCCC<CallingConvention::DEFAULT, RT, O, Args...>;

	template <typename Func>
	struct is_function_ptr :
		std::integral_constant<bool, std::is_pointer_v<Func> &&
		std::is_function_v<std::remove_pointer_t<Func>>      &&
		!std::is_member_function_pointer_v<Func>>
	{};
	template <typename Func> constexpr bool is_function_ptr_v = is_function_ptr<Func>::value;

	template<typename Func> struct Function_to_sig				  { using type = Func; };
	template<typename Sig>  struct Function_to_sig<Function<Sig>> { using type = Sig; };
	template<typename Func> using Function_to_sig_t = typename Function_to_sig<Func>::type;

	template<typename Sig> struct is_funcs       : std::false_type {};
	template<typename RT>  struct is_funcs<RT()> : std::true_type  {};
	template<typename Sig> constexpr bool is_funcs_v = is_funcs<Sig>::value;

	template<typename RT, typename... Args>
	using sig_create = RT(Args...);

	template<typename FuncObj> struct sig_f { using type = typename sig_f<decltype(&FuncObj::operator())>::type; };
	template<typename RT, typename... Args>
	struct sig_f<RT(Args...)>			    { using type = sig_create<RT, Args...>; };
	template<typename RT, typename... Args>
	struct sig_f<RT(*)(Args...)>		    { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct sig_f<RT(O::*)(Args...)>		    { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct sig_f<RT(O::*)(Args...) const>   { using type = sig_create<RT, Args...>; };
	template<typename FuncPT> using sig_f_t = typename sig_f<FuncPT>::type;

	template<typename FuncObj> struct sig_s { using type = typename sig_s<decltype(&FuncObj::operator())>::type; };
	template<typename RT, typename... Args>
	struct sig_s<RT(Args...)>			    { using type = sig_create<RT>; };
	template<typename RT, typename... Args>
	struct sig_s<RT(*)(Args...)>		    { using type = sig_create<RT>; };
	template<typename RT, typename O, typename... Args>
	struct sig_s<RT(O::*)(Args...)>		    { using type = sig_create<RT>; };
	template<typename RT, typename O, typename... Args>
	struct sig_s<RT(O::*)(Args...) const>   { using type = sig_create<RT>; };
	template<typename FuncPT> using sig_s_t = typename sig_s<FuncPT>::type;

#define SIG_F_T(Func) \
	sig_f_t<decltype(Func)>

#define SIG_S_T(Func) \
	sig_s_t<decltype(Func)>

	template<typename Sig> struct is_sig : std::false_type {};
	template<typename RT, typename... Args>
	struct is_sig<RT(Args...)> : std::true_type {};
	template<typename Sig>
	constexpr bool is_sig_v = is_sig<Sig>::value;

	template<typename Sig> struct sig_helper;
	template<typename RT, typename... Args> 
	struct sig_helper<RT(Args...)>
	{
		using return_t                         = RT;
		using args                             = t_list::type_list<Args...>;

		static constexpr int  n_args           = args::n_types;
		static constexpr bool no_args          = args::empty;

		template<typename T>
		static constexpr bool has_arg          = args::template contains<T>;
		template<typename... Ts>
		static constexpr bool same_args        = args::template is_same<Ts...>;
		template<typename... Ts>
		static constexpr bool convertable_args = args::template is_convertible<Ts...>();
	};

	template<typename Sig> using                  sig_rt_t      = typename sig_helper<Sig>::return_t;
	template<typename Sig> constexpr std::size_t  sig_n_args_v  = sig_helper<Sig>::n_args;
	template<typename Sig> constexpr bool         sig_no_args_v = sig_helper<Sig>::no_args;

	template<typename Sig, typename Arg> 
	constexpr bool sig_has_arg_v          = sig_helper<Sig>::template has_arg<Arg>;
	template<typename Sig, typename... Args> 
	constexpr bool sig_same_args_v        = sig_helper<Sig>::template same_args<Args...>;
	template<typename Sig, typename... Args> 
	constexpr bool sig_convertible_args_v = sig_helper<Sig>::template convertable_args<Args...>;
}