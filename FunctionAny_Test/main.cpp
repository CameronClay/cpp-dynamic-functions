#include "FunctionAny.h"
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

int __stdcall SCallFunc(int, float)
{
	return 0;
}

using namespace ftraits;

int main()
{
	std::string local = "the world";
	auto hello_world = [&local](const char* str)
	{
		std::cout << local << " says " << str << std::endl;
	};

	A a{ 5, 2.34f };

	std::vector<FunctionAny<SIG_S_T(hello_world), SIG_S_T(&A::Out), SIG_S_T(&A::Out2), SIG_F_T(&A::Moo), SIG_F_T(Add), SIG_F_T(Add2), SIG_F_T(MakeCopy), SIG_F_T(ReturnRef)>> funcList;
	funcList.emplace_back(std::in_place_type<SIG_S_T(hello_world)>, hello_world, "boo hoo");
	funcList.emplace_back(std::in_place_type<SIG_S_T(&A::Out)>, &A::Out, a, 5, 7.5);
	funcList.emplace_back(std::in_place_type<SIG_S_T(&A::Out2)>, &A::Out2, &a, 92);
	funcList.emplace_back(std::in_place_type<SIG_F_T(&A::Moo)>, &A::Moo);
	funcList.emplace_back(std::in_place_type<SIG_F_T(Add)>, Add);
	funcList.emplace_back(std::in_place_type<SIG_F_T(Add2)>, Add2);
	funcList.emplace_back(std::in_place_type<SIG_F_T(MakeCopy)>, MakeCopy);
	funcList.emplace_back(std::in_place_type<SIG_F_T(ReturnRef)>, ReturnRef);

	auto rt_visitor = [](const auto& ret)
	{
		using RT = std::decay_t<decltype(ret)>;
		if constexpr (std::is_same_v<RT, NO_CALL>)
		{
			//std::cout << "Func was not called" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, VOID>)
		{
			//std::cout << "Func returned with type: void" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, int>)
		{
			std::cout << "Func returned " << ret << " with type: int" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, float>)
		{
			std::cout << "Func returned " << ret << " with type: float" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, std::string>)
		{
			std::cout << "Func returned " << ret << " with type: string" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, A>)
		{
			std::cout << "Func returned {" << ret.m_i << ", " << ret.m_f << "} with type: A" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, A*>)
		{
			std::cout << "Func returned " << ret << " {" << ret->m_i << ", " << ret->m_f << "} with type: A*" << std::endl;
		}
		else
		{
			std::cout << "Invalid Return type" << std::endl;
		}
	};

	for (auto& it : funcList)
	{
		it.Invoke(rt_visitor);
		it.Invoke(rt_visitor, a, 5.0, 6);
		it.Invoke(rt_visitor, 1.f, 2, 3.5);
		it.Invoke(rt_visitor, std::ref(a));
	}

	std::string s;
	std::getline(std::cin, s);

	return 0;
}