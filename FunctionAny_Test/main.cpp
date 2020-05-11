#include "FunctionAny.h"
#include "FunctionTraits.h"
#include "TypeList.h"
#include <iostream>
#include <vector>
#include <string>

struct A
{
	A(int i, float f)
		:
		m_i(i),
		m_f(f)
	{}

	void Out(int v1, float v2)
	{
		std::cout << v1 << ' ' << v2 << std::endl;
	}


	int Partial(int v1, float v2, double j, double k)
	{
		std::cout << "Partial " << v1 << ' ' << v2 << ' ' << j << ' ' << k << std::endl;
		return v1 + v2;
	}

	void Out2(int v1) const
	{
		std::cout << v1 << std::endl;
	}

	static std::string Moo()
	{
		return "MOOOOOOOO!";
	}

	int m_i;
	float m_f;
};

struct Functor
{
	int operator()(int i)
	{
		return i + 1;
	}
};

int Add(A& a, int v1, int v2)
{
	const auto res = v1 + v2;
	std::cout << v1 << " + " << v2 << " = " <<  res << std::endl;
	return res;
}

float Add2(float v1, float v2, float v3)
{
	const auto res = v1 + v2 + v3;
	std::cout << v1 << " + " << v2 << " + " << v3 << " = " << res << std::endl;
	return res;
}
A MakeCopy(const A& a)
{
	return a;
}

A& ReturnRef(A& a)
{
	return a;
}

using namespace f_traits;

template<typename... Ts>
using tl = t_list::type_list<Ts...>;

template<typename T1>
struct IS_TYPE
{
	template<typename T2>
	static constexpr bool value = std::is_same_v<T1, T2>;
};

int main()
{
	std::string local = "the world";
	auto hello_world = [&local](const char* str)
	{
		std::cout << local << " says " << str << std::endl;
	};

	Functor functor;

	A a{ 5, 2.34f };

	//using TL_1 = tl<int, bool, char, float, const char, void(), void*, int, float, char, char*, int*, double, int, char, wchar_t, float*, double*, float**, double**, int(), int(int)>;
	//using TL_2 = tl<int, char, float, int, double, int*, const char, void*, int, float, void(), char, char*, std::string, void**, A, Functor, A&, A**, A*>;
	//using TL_3 = tl<std::vector<float>, char, float, double, void*, int, float, const char, const wchar_t*, int*, const char*, int()>;

	using L_FUNC_S   = tl<SIG_S(hello_world), SIG_S(&A::Out), SIG_S(&A::Out2)>;
	using L_FUNC_F   = tl<SIG_F(&A::Moo), SIG_F(Add), SIG_F(Add2), SIG_F(MakeCopy), SIG_F(ReturnRef), SIG_F(functor)>;
	using L_FUNC_P   = tl<SIG_PB(&A::Partial, 2u)>;
	using FUNC_ANY   = FunctionAny_Sig_Lists<L_FUNC_S, L_FUNC_F, L_FUNC_P/*, L_FUNC_R*/>;

	// Declare FunctionAny taking any combination of the following RTs and Arg lists (warning this can be slow to compile with large type lists!)
	using RT_LIST    = tl<void, std::string, int, float, A, A&>;
	using ARG_LISTS  = tl<tl<>, tl<int>, tl<int, float>, tl<float, float, float>, tl<A&, int, int>, tl<const A&>, tl<A&>, tl<const char*>, tl<double, double>/*, tl<const A&, int, float>*/>;
	using FUNC_ANY_2 = FunctionAny_RT_Args<RT_LIST, ARG_LISTS>;
	
	// Create a vector of functions that match any of the above signatures in L_FUNC_S or L_FUNC_F
	std::vector<FUNC_ANY> funcList;
	funcList.emplace_back(std::in_place_type<SIG_S(hello_world)>, hello_world, "whoo hoo");
	funcList.emplace_back(std::in_place_type<SIG_S(&A::Out)>,  &A::Out, a, 5, 7.5);
	funcList.emplace_back(std::in_place_type<SIG_S(&A::Out2)>, &A::Out2, &a, 92);
	funcList.emplace_back(std::in_place_type<SIG_F(&A::Moo)>,  &A::Moo);
	//funcList.emplace_back(std::in_place_type<RUNTIME_OBJ>, &A::Out);

	funcList.emplace_back(std::in_place_type<SIG_PB(&A::Partial, 2u)>, &A::Partial, a, 5, 6.0); //partial 
	funcList.emplace_back(std::in_place_type<SIG_F(Add)>,  Add);
	funcList.emplace_back(std::in_place_type<SIG_F(Add2)>, Add2);
	funcList.emplace_back(std::in_place_type<SIG_F(MakeCopy)>, MakeCopy);
	funcList.emplace_back(std::in_place_type<SIG_F(ReturnRef)>, ReturnRef);
	funcList.emplace_back(std::in_place_type<SIG_F(functor)>, functor);

	// Catch and process the return value
	auto rt_visitor = [](const auto& ret)
	{
		using RT    = std::decay_t<decltype(ret)>;
		static_assert(FUNC_ANY_2::RTS_UNIQUE::contains<RT>, "Error: Invalid return type");

		using RT_IS = IS_TYPE<RT>;
		if constexpr      (RT_IS::template value<NO_CALL>)
		{
			//std::cout << "Func was not called" << std::endl;
		}
		else if constexpr (RT_IS::template value<VOID>)
		{
			//std::cout << "Func returned with type: void" << std::endl;
		}
		else if constexpr (RT_IS::template value<int>)
		{
			std::cout << "Func returned " << ret << " with type: int" << "\n";
		}
		else if constexpr (RT_IS::template value<float>)
		{
			std::cout << "Func returned " << ret << " with type: float" << "\n";
		}
		else if constexpr (RT_IS::template value<std::string>)
		{
			std::cout << "Func returned " << ret << " with type: string" << "\n";
		}
		else if constexpr (RT_IS::template value<A>)
		{
			std::cout << "Func returned {" << ret.m_i << ", " << ret.m_f << "} with type: A" << "\n";
		}
		else if constexpr (RT_IS::template value<A*>)         // Reference converted to pointer type
		{
			std::cout << "Func returned " << ret << ", *A = {" << ret->m_i << ", " << ret->m_f << "} with type: A*" << "\n";
		}
		else
		{
			std::cout << "Return Type not handled" << "\n";
		}
	};

	std::cout << "---Try_Invoke all functions in funcList---" << "\n";
	// Try to invoke each function in funcList with a set of parameters
	for (auto& it : funcList)
	{
		it.Invoke(rt_visitor);
		it.Invoke(rt_visitor, 0);
		it.Invoke(rt_visitor, std::ref(a), 5, 6);
		it.Invoke(rt_visitor, 1.f, 2, 3.5);
		it.Invoke(rt_visitor, -3432.12, 2.643);
		it.Invoke(rt_visitor, std::ref(a)); //calls both ref and non-ref versions
		it.Invoke(rt_visitor, a); //only calls non-ref version
		//it.Invoke(rt_visitor, false, true, 2, 3, 4, 5); // Error because this argument list is not convertable to any in FUNC_ANY
	}

	std::cout << std::endl << "---Moved all functions from funcList1 to funcList2---" << "\n\n";
	std::vector<FUNC_ANY_2> funcList2;
	for (auto& it : funcList)
	{
		funcList2.push_back(std::move(it));
	}
	funcList.clear();
	
	std::cout << "---Try_Invoke all functions in funcList2---" << "\n";
	// Try to invoke each function in funcList with a set of parameters
	for (auto& it : funcList2)
	{
		it.Invoke(rt_visitor);
		it.Invoke(rt_visitor, 0);
		it.Invoke(rt_visitor, std::ref(a), 5.43, -6.9);
		it.Invoke(rt_visitor, std::ref(a));
		it.Invoke(rt_visitor, a);
	}
	
	funcList.clear();
	
	std::cout << "\n" <<  "Changing value of a..." << "\n";
	a.m_i = -2;
	a.m_f = 98.321;
	std::cout << "---Try_Invoke all functions in funcList2---" << "\n";
	// Try to invoke each function in funcList with a set of parameters
	for (auto& it : funcList2)
	{
		it.Invoke(rt_visitor);
		it.Invoke(rt_visitor, 0);
		it.Invoke(rt_visitor, std::ref(a), 0, -934);
		it.Invoke(rt_visitor, std::ref(a));
		it.Invoke(rt_visitor, a);
	}
	
	std::string s;
	std::getline(std::cin, s);

	return 0;
}