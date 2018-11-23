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

	static void Moo()
	{
		std::cout << "MOOO" << std::endl;
	}
};

void Add(int v1, int v2)
{
	std::cout << "v1 + v2 = " << v1 + v2 << std::endl;
}

int main()
{
	auto hello_world = [](const char* str)
	{
		std::cout << "the world" << " says " << str << std::endl;
	};

	A a;

	//Function<void()> f{ &A::Out, &a, 5, 7.5 };
	//auto f = Function<void()>::MakeFunc(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//auto f = Function<void()>::Function(&A::Out, a, 5, 7.5);
	//f();

	std::vector<FunctionAny<void(int, int), void(int), void(), void(const char*)>> funcList;
	//std::vector<FunctionAny<decltype(&A::Out), decltype(&A::Moo), decltype(&Add)>, decltype(&hello_world)> funcList;
	funcList.push_back(Function<void()>{&A::Out, &a, 5, 7.5});
	funcList.back()();
	//funcList.emplace_back(&A::Moo);
	//funcList.back()();
	//funcList.emplace_back(&Add);
	//funcList.back()(5, 6);
	//funcList.emplace_back(&hello_world, "boo hoo");
	//funcList.back()();

	std::string s;
	std::getline(std::cin, s);

	return 0;
}