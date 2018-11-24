#pragma once

template<typename Sig> class Function;

namespace ftraits
{
	enum class CallingConvention { DEFAULT, CDECLR, STDCALL, FASTCALL, THISCALL };
	template<CallingConvention> struct CCHelper;

	template<> struct CCHelper<CallingConvention::DEFAULT>
	{
		template<typename RT, typename... Args>
		using PFunc =   RT(   *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::CDECLR>
	{
		template<typename RT, typename... Args>
		using PFunc =   RT(__cdecl    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__cdecl O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__cdecl O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::STDCALL>
	{
		template<typename RT, typename... Args>
		using PFunc =   RT(__stdcall    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__stdcall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__stdcall O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::FASTCALL>
	{
		template<typename RT, typename... Args>
		using PFunc =  RT(__fastcall     *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__fastcall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__fastcall O::*)(Args...) const;
	};
	template<> struct CCHelper<CallingConvention::THISCALL>
	{
		template<typename RT, typename... Args>
		using PFunc =   RT(__thiscall    *)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncM =  RT(__thiscall O::*)(Args...);
		template<typename RT, typename O, typename... Args>
		using PFuncMC = RT(__thiscall O::*)(Args...) const;
	};

	template<CallingConvention CC, typename RT,             typename... Args>
	using PFuncCC =	  typename CCHelper<CC>::template PFunc<RT, Args...>;
	template<CallingConvention CC, typename RT, typename O, typename... Args>
	using PFuncMCC =  typename CCHelper<CC>::template PFuncM<RT, O, Args...>;
	template<CallingConvention CC, typename RT, typename O, typename... Args>
	using PFuncMCCC = typename CCHelper<CC>::template PFuncMC<RT, O, Args...>;

	template<typename RT, typename... Args>
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
	template<typename RT>  struct is_funcs<RT()> : std::true_type {};
	template<typename Sig> bool constexpr is_funcs_v = is_funcs<Sig>::value;

	template<typename RT, typename... Args>
	using sig_create = RT(Args...);

	template<typename Func> struct sig_nparams;
	template<typename RT, typename... Args>
	struct sig_nparams<RT(Args...)> : std::integral_constant<int, sizeof...(Args)> {};
	template<typename Func> constexpr int sig_nparams_v = sig_nparams<Func>::value;

	template<typename FuncPT> struct sig_f;
	template<typename RT, typename... Args>
	struct sig_f<RT(Args...)>			  { using type = sig_create<RT, Args...>; };
	template<typename RT, typename... Args>
	struct sig_f<RT(*)(Args...)>		  { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct sig_f<RT(O::*)(Args...)>		  { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct sig_f<RT(O::*)(Args...) const> { using type = sig_create<RT, Args...>; };
	template<typename FuncPT> using sig_f_t = typename sig_f<FuncPT>::type;

	template<typename FuncPT> struct sig_s;
	template<typename RT, typename... Args>
	struct sig_s<RT(Args...)>			  { using type = sig_create<RT>; };
	template<typename RT, typename... Args>
	struct sig_s<RT(*)(Args...)>		  { using type = sig_create<RT>; };
	template<typename RT, typename O, typename... Args>
	struct sig_s<RT(O::*)(Args...)>		  { using type = sig_create<RT>; };
	template<typename RT, typename O, typename... Args>
	struct sig_s<RT(O::*)(Args...) const> { using type = sig_create<RT>; };
	template<typename FuncPT> using sig_s_t = typename sig_s<FuncPT>::type;

	template<typename... Ts> struct type_list {};
	template <bool...> struct bool_pack;
	template <bool... v>
	using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

	template<typename Sig> struct sig_helper;
	template<typename RT, typename... Args> 
	struct sig_helper<RT(Args...)>
	{
		using return_t = RT;

		static constexpr int  n_args = sizeof...(Args);

		template<typename... Ts>
		static constexpr bool same_args = std::is_same_v<type_list<Args...>, type_list<Ts...>>;

		template<typename... Ts>
		static constexpr bool convertable_args()
		{
			if constexpr (sizeof...(Ts) == n_args)
				return all_true<std::is_convertible<std::decay_t<Ts>, std::decay_t<Args>>{}...>::value;

			return false;
		}
	};
	template<typename Sig> using sig_rt_t = typename sig_helper<Sig>::return_t;
	template<typename Sig> constexpr int sig_n_args_v = sig_helper<Sig>::n_args;
	template<typename Sig, typename... Args> constexpr bool sig_same_args_v = sig_helper<Sig>::template same_args<Args...>;
	template<typename Sig, typename... Args> constexpr bool sig_convertible_args_v = sig_helper<Sig>::template convertable_args<Args...>();
}