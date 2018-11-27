#pragma once

namespace t_list
{
	template <typename... Args> struct type_list {};

	template <typename T, typename List> struct type_list_has;
	template <typename A, typename... Ts>
	struct type_list_has<A, type_list<Ts...>> : std::disjunction<std::is_same<A, Ts>...> {};
	template <typename T, typename List>
	constexpr bool type_list_has_v = type_list_has<T, List>::value;

	template <typename TNew, typename Ts, bool is_duplicate = type_list_has_v<TNew, Ts>>
	struct add_unique;
	template <typename TNew, typename... Ts>
	struct add_unique<TNew, type_list<Ts...>, true>  { using type = type_list<Ts...>; };
	template <typename TNew, typename... Ts >
	struct add_unique<TNew, type_list<Ts...>, false> { using type = type_list<TNew, Ts...>; };

	template <typename... Ts> struct type_list_unique_helper;
	template <> struct type_list_unique_helper<>
	{
		using type = type_list<>;
	};
	template <typename... TypeListArgs, typename... Args>
	struct type_list_unique_helper<type_list<TypeListArgs...>, Args...>
	{ 
		using type = typename type_list_unique_helper<TypeListArgs..., Args...>::type; 
	};
	template <typename THead, typename... TTail>
	struct type_list_unique_helper<THead, TTail...>
	{ 
		using type = typename add_unique<THead, typename type_list_unique_helper<TTail...>::type>::type; 
	};

	//make unique type list given list of types (also concatenates multiple type_lists)
	template <typename... Ts>
	using type_list_unique = typename type_list_unique_helper<Ts...>::type;


	// rebind - Rebinds template arguments from T1 to T2 where T1 and T2 are templates
	template<class T1, template<typename...> class T2> struct rebind;
	template<template<typename...> class T1, typename... Ts, template<typename...> class T2> 
	struct rebind<T1<Ts...>, T2>
	{ 
		using type = T2<Ts...>; 
	};

	template<class T1, template<typename...> class T2>
	using rebind_t = typename rebind<T1, T2>::type;

	// type_list_apply - applies each template argument from outer to inner
	template<class Outer, template<typename...> class Inner> struct type_list_apply;
	template<template<typename...> class Outer, template<typename...> class Inner, typename... Ts>
	struct type_list_apply<Outer<Ts...>, Inner>
	{
		using type = Outer<Inner<Ts>...>;
	};

	template<class Outer, template<typename...> class Inner>
	using type_list_apply_t = typename type_list_apply<Outer, Inner>::type;
}