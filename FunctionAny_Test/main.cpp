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

	static void Moo()
	{
		std::cout << "MOOO" << std::endl;
	}

	int m_i;
	float m_f;
};

void Add(A& a, int v1, int v2)
{
	const auto res = v1 + v2;
	std::cout << v1 << " + " << v2 << " = " <<  res << std::endl;
	//return res;
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

	//Function<void()> f{ &A::Out, &a, 5, 7.5 };
	//auto f = Function<void()>::MakeFunc(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//f();

	std::vector<FunctionAny<sig_f_t<decltype(&Add)>, sig_s_t<decltype(&A::Out)>>> funcList;
	//std::vector<FunctionAny<decltype(&A::Out), decltype(&A::Moo), decltype(&Add)>, decltype(&hello_world)> funcList;
	funcList.emplace_back(std::in_place_type<sig_s_t<decltype(&A::Out)>>, &A::Out, a, 5, 7.5);
	//funcList.back()();
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&A::Moo)>>, &A::Moo);
	//funcList.back()();
	funcList.emplace_back(std::in_place_type<sig_s_t<decltype(&A::Out2)>>, &A::Out2, &a, 92);
	//funcList.back()();
	funcList.emplace_back(std::in_place_type<sig_f_t<decltype(&Add)>>, &Add);
	//funcList.back()(a, 5, 6);
	funcList.emplace_back(std::in_place_type<void()>, hello_world, "boo hoo");
	//funcList.back()();

	std::cout << std::endl;

	for (auto& it : funcList)
	{
		it();
		it(a, 5, 6);
	}

	std::string s;
	std::getline(std::cin, s);

	return 0;
}