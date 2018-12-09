#pragma once

namespace t_list
{
	template <typename... Ts> struct type_list;

	template <typename T, typename... Ts>
	struct contains : std::disjunction<std::is_same<T, Ts>...> {};
	template <typename T, typename... Ts>
	constexpr bool contains_v = contains<T, Ts...>::value;

	template <typename List, typename T> struct type_list_contains;
	template <typename T, typename... Ts>
	struct type_list_contains<type_list<Ts...>, T> : std::disjunction<std::is_same<T, Ts>...> {};
	template <typename List, typename T>
	constexpr bool type_list_contains_v = type_list_contains<T, List>::value;

	template <typename... Ts> struct is_unique;

	template <> struct is_unique<>
	{
		static constexpr bool value = true;
	};
	template <typename THead, typename... TTail>
	struct is_unique<THead, TTail...>
	{
		static constexpr bool value = !contains_v<THead, TTail...> && is_unique<TTail...>::value;
	};

	template <typename... Ts>
	constexpr bool is_unique_v = is_unique<Ts...>::value;

	template <typename TNew, typename Ts, bool is_duplicate = type_list_contains_v<TNew, Ts>>
	struct add_unique;
	template <typename TNew, typename... Ts>
	struct add_unique<TNew, type_list<Ts...>, true> { using type = type_list<Ts...>; };
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

	// type_list_unique -  Make unique type_list given list of types (also concatenates multiple type_lists)
	template <typename... Ts>
	using type_list_unique = typename type_list_unique_helper<Ts...>::type;

	// extract -  Extract type at idx in Ts...
	template <std::size_t idx, typename... Ts>
	class extract
	{
		static_assert(idx < sizeof...(Ts), "idx out of bounds");

		template <std::size_t i, std::size_t n, typename... TTail>
		struct extract_impl;

		template <std::size_t i, std::size_t n, typename T, typename... TTail>
		struct extract_impl<i, n, T, TTail...>
		{
			using type = typename extract_impl<i + 1, n, TTail...>::type;
		};

		template <std::size_t n, typename T, typename... TTail>
		struct extract_impl<n, n, T, TTail...>
		{
			using type = T;
		};

	public:
		using type = typename extract_impl<0, idx, Ts...>::type;
	};

	template <std::size_t idx, class TypeList>
	struct type_list_extract;

	template <std::size_t idx, template <class...> class TypeList, typename... Ts>
	struct type_list_extract<idx, TypeList<Ts...>>
	{
		using type = typename extract<idx, Ts...>::type;
	};

	template <std::size_t idx, class TypeList>
	using type_list_extract_t = typename type_list_extract<idx, TypeList>::type;

	// cartesian_product - Cross Product of two type_lists
	template<typename T1, typename T2> struct pair {};
	template <typename T, typename U> struct cartesian_product;
	template <template<typename...> class TypeList1, template<typename...> class TypeList2, typename... Us>
	struct cartesian_product<TypeList1<>, TypeList2<Us...>>
	{
		using type = type_list<>;
	};
	template <template<typename...> class TypeList1, template<typename...> class TypeList2, typename T, typename... Ts, typename... Us>
	struct cartesian_product<TypeList1<T, Ts...>, TypeList2<Us...>>
	{
		using type = type_list<type_list<pair<T, Us>...>,
			typename cartesian_product<type_list<Ts...>, type_list<Us...>>::type>;
	};

	template <typename T, typename U>
	using cartesian_product_t = typename cartesian_product<T, U>::type;

	// rebind - Rebinds template arguments from T1 to T2 where T1 and T2 are templates
	template<class TFrom, template<typename...> class TTo> struct rebind;
	template<template<typename...> class TFrom, typename... Ts, template<typename...> class TTo>
	struct rebind<TFrom<Ts...>, TTo>
	{ 
		using type = TTo<Ts...>; 
	};

	template<class T1, template<typename...> class T2>
	using rebind_t = typename rebind<T1, T2>::type;

	// type_list_apply - applies each template argument from outer to inner
	template<class Outer, template<typename...> class Inner> struct apply_inner;
	template<template<typename...> class Outer, template<typename...> class Inner, typename... Ts>
	struct apply_inner<Outer<Ts...>, Inner>
	{
		using type = Outer<Inner<Ts>...>;
	};

	template<class Outer, template<typename...> class Inner>
	using apply_inner_t = typename apply_inner<Outer, Inner>::type;

	template <bool...> struct bool_pack;
	template <bool... v> using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;
	template <bool... v> constexpr bool all_true_v = all_true<v...>::value;

	template <typename... Ts> class type_list
	{
	public:
		using unique = type_list_unique<Ts...>;
		template <typename... Args>
		using append            = type_list<Ts..., Args...>;
		template <typename... Args>
		using append_unique     = type_list_unique<Ts..., Args...>;

		template <template<typename...> class TypeList>
		using cartesian_product = cartesian_product_t<type_list<Ts...>, TypeList>;
		template<template<typename...> class TTo>
		using rebind            = rebind_t<type_list<Ts...>, TTo>;
		template<template<typename...> class Inner>
		using apply             = apply_inner_t<type_list<Ts...>, Inner>;
		template <std::size_t idx>
		using extract           = type_list_extract_t<idx, type_list<Ts...>>;

		static constexpr int  n_types   = sizeof... (Ts);
		static constexpr bool empty     = n_types == 0;
		static constexpr bool is_unique = is_unique_v<Ts...>;

		template<typename T>
		static constexpr bool contains  = contains_v<T, Ts...>;

		template<typename... Args>
		static constexpr bool same      = std::is_same_v<type_list<Args...>, type_list<Ts...>>;

		template<typename... Args>
		static constexpr bool convertible()
		{
			if constexpr (sizeof... (Args) == n_types)
				return all_true_v<std::is_convertible_v<std::decay_t<Ts>, std::decay_t<Args>>...>;

			return false;
		}
	};
}