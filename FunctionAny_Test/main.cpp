#include "FunctionAny.h"
#include <iostream>
#include <vector>
#include <string>

struct A
{
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
};

void Add(int v1, int v2)
{
	std::cout << v1 << " + " << v2 << " = " <<  v1 + v2 << std::endl;
	//return v1 + v2;
}

using namespace ftraits;

int main()
{
	std::string local = "the world";
	auto hello_world = [&local](const char* str)
	{
		std::cout << local << " says " << str << std::endl;
	};

	A a;

	//Function<void()> f{ &A::Out, &a, 5, 7.5 };
	//auto f = Function<void()>::MakeFunc(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//f();

	std::vector<FunctionAny<funcpt_to_sig_t<decltype(&Add)>, funcpt_to_sig_t<decltype(&A::Out)>, funcpt_to_sig_t<decltype(&A::Moo)>>> funcList;
	//std::vector<FunctionAny<decltype(&A::Out), decltype(&A::Moo), decltype(&Add)>, decltype(&hello_world)> funcList;
	funcList.emplace_back(std::in_place_type<void()>, &A::Out, a, 5, 7.5);
	funcList.back()();
	funcList.emplace_back(std::in_place_type<void()>, &A::Moo);
	funcList.back()();
	funcList.emplace_back(std::in_place_type<void()>, &A::Out2, &a, 92);
	funcList.back()();
	funcList.emplace_back(std::in_place_type<void(int, int)>, &Add);
	funcList.back()(5, 6);

	std::cout << std::endl;

	for (auto& it : funcList)
	{
		it(5, 6);
	}
	//funcList.emplace_back(std::in_place_type<void()>, &hello_world, "boo hoo");
	//funcList.back()();

	std::string s;
	std::getline(std::cin, s);

	return 0;
}