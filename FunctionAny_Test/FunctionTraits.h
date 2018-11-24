#pragma once

template<typename Sig> class Function;

namespace ftraits
{
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

	template <typename Func>
	struct is_function_ptr :
		std::integral_constant<bool, std::is_pointer_v<Func> &&
		std::is_function_v<std::remove_pointer_t<Func>> &&
		!std::is_member_function_pointer_v<Func>>
	{};

	template <typename Func>
	bool constexpr is_function_ptr_v = is_function_ptr<Func>::value;

	template<typename Func> struct Function_to_sig { using type = Func; };
	template<typename Sig>  struct Function_to_sig<Function<Sig>> { using type = Sig; };
	template<typename Func> using Function_to_sig_t = typename Function_to_sig<Func>::type;

	template<typename Sig> struct is_funcs : std::false_type {};
	template<typename RT> struct is_funcs<RT()> : std::true_type {};

	template<typename RT, typename... Args>
	using sig_create = RT(Args...);

	template<typename Func> struct sig_nparams;
	template<typename RT, typename... Args>
	struct sig_nparams<RT(Args...)> : std::integral_constant<int, sizeof...(Args)> {};

	template<typename FuncPT> struct funcpt_to_sig;
	template<typename RT, typename... Args>
	struct funcpt_to_sig<RT(Args...)> { using type = sig_create<RT, Args...>; };
	template<typename RT, typename... Args>
	struct funcpt_to_sig<RT(*)(Args...)> { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct funcpt_to_sig<RT(O::*)(Args...)> { using type = sig_create<RT, Args...>; };
	template<typename RT, typename O, typename... Args>
	struct funcpt_to_sig<RT(O::*)(Args...) const> { using type = sig_create<RT, Args...>; };

	template<typename FuncPT> using funcpt_to_sig_t = typename funcpt_to_sig<FuncPT>::type;
}