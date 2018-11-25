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

A MakeCopy(const A& a)
{
	return a;
}

A* ReturnPointer(A& a)
{
	return &a;
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

	std::vector<FunctionAny<sig_f_t<decltype(&Add)>, sig_f_t<decltype(&A::Moo)>, sig_s_t<decltype(&A::Out)>, sig_s_t<decltype(&A::Out2)>, sig_f_t<decltype(&MakeCopy)>, sig_f_t<decltype(&ReturnPointer)>>> funcList;
	funcList.emplace_back(std::in_place_type<sig_s_t<decltype(&A::Out)>>, &A::Out, a, 5, 7.5);
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&A::Moo)>>, &A::Moo);
	funcList.emplace_back(std::in_place_type<sig_s_t<decltype(&A::Out2)>>, &A::Out2, &a, 92);
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&Add)>>, &Add);
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&MakeCopy)>>, &MakeCopy);
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&ReturnPointer)>>, &ReturnPointer);
	funcList.emplace_back(std::in_place_type<void()>, hello_world, "boo hoo");

	auto rt_visitor = [](const auto& ret)
	{
		using RT = std::decay_t<decltype(ret)>;
		if constexpr (std::is_same_v<RT, VOID>)
		{
			//std::cout << "Func returned with type: void" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, NO_CALL>)
		{
			//std::cout << "Func was not called" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, int>)
		{
			std::cout << "Func returned " << ret << " with type: int" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, std::string>)
		{
			std::cout << "Func returned " << ret << " with type: string" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, A>)
		{
			std::cout << "Func returned {" << static_cast<A>(ret).m_i << ", " << static_cast<A>(ret).m_f << "} with type: A" << std::endl;
		}
		else if constexpr (std::is_same_v<RT, A*>)
		{
			std::cout << "Func returned " << ret << " {" << static_cast<A*>(ret)->m_i << ", " << static_cast<A*>(ret)->m_f << "} with type: A*" << std::endl;
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
		it.Invoke(rt_visitor, std::ref(a));
	}

	std::string s;
	std::getline(std::cin, s);

	return 0;
}