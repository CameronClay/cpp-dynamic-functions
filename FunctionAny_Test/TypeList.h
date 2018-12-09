#pragma once
#include <algorithm>

namespace t_list
{
	template <typename... Ts> struct type_list;

	// type_list_cat - concatenate multiple type_list's
	template <class... TLists> struct type_list_cat;
	template <> struct type_list_cat<>
	{
		using type = type_list<>;
	};
	template <typename... TL1Args>
	struct type_list_cat<type_list<TL1Args...>>
	{
		using type = type_list<TL1Args...>;
	};
	template <typename... TL1Args, typename... TL2Args, class... TLists>
	struct type_list_cat<type_list<TL1Args...>, type_list<TL2Args...>, TLists...>
	{
		using type = typename type_list_cat<type_list<TL1Args..., TL2Args...>, TLists...>::type;
	};
	template <class... TLists>
	using type_list_cat_t = typename type_list_cat<TLists...>::type;

	// type_list_contains_v - true if Ts contains type T
	template <typename T, typename... Ts>
	struct contains : std::disjunction<std::is_same<T, Ts>...> {};
	template <typename T, typename... Ts>
	constexpr bool contains_v = contains<T, Ts...>::value;

	// type_list_contains_v - true if type_list contains type T
	template <typename List, typename T> struct type_list_contains;
	template <typename T, typename... Ts>
	struct type_list_contains<type_list<Ts...>, T> : std::disjunction<std::is_same<T, Ts>...> {};
	template <typename List, typename T>
	constexpr bool type_list_contains_v = type_list_contains<T, List>::value;

	// is_unique - true if type_list contains no duplicates
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

	// type_list_filter - Filter all elements of of type_list where Predicate returns false
	template <template <typename> class Predicate, typename... Ts>
	struct type_list_filter;
	template <template <typename> class Predicate>
	struct type_list_filter<Predicate>
	{
		using type = type_list<>;
	};
	template <template <typename> class Predicate, typename T>
	struct type_list_filter<Predicate, T>
	{
		using type = std::conditional_t<Predicate<T>::value, type_list<T>, type_list<>>;
	};
	template <template <typename> class Predicate, typename T, typename... Ts>
	struct type_list_filter<Predicate, T, Ts...>
	{
		using type = typename std::conditional_t<Predicate<T>::value, type_list<T>, type_list<>>::template append<
			         typename type_list_filter<Predicate, Ts...>::type>;
	};

	template <template <typename> class Predicate, typename... Ts>
	using type_list_filter_t = typename type_list_filter<Predicate, Ts...>::type;

	template <typename TNew, typename Ts, bool is_duplicate = type_list_contains_v<TNew, Ts>>
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

	template <std::size_t idx, class TypeList> struct type_list_extract;
	template <std::size_t idx, template <class...> class TypeList, typename... Ts>
	struct type_list_extract<idx, TypeList<Ts...>>
	{
		using type = typename extract<idx, Ts...>::type;
	};
	template <class TypeList, std::size_t idx>
	using type_list_extract_t = typename type_list_extract<idx, TypeList>::type;

	// cartesian_product - Cross Product of two type_lists
	//		type = pair<T, U>...;
	template <typename T1, typename T2> struct pair {};
	template <typename T, typename U> struct cartesian_product;
	template <typename... Us>
	struct cartesian_product<type_list<>, type_list<Us...>>
	{
		using type = type_list<>;
	};
	template <typename T, typename... Ts, typename... Us>
	struct cartesian_product<type_list<T, Ts...>, type_list<Us...>>
	{
		using type = type_list_unique<type_list<pair<T, Us>...>,
			typename cartesian_product<type_list<Ts...>, type_list<Us...>>::type>;
	};

	template <typename T, typename U>
	using cartesian_product_t = typename cartesian_product<T, U>::type;

	// rebind - Rebinds template arguments from T1 to T2 where T1 and T2 are templates
	template <class TFrom, template<class...> class TTo> struct rebind;
	template <template<class...> class TFrom, class... Ts, template<typename...> class TTo>
	struct rebind<TFrom<Ts...>, TTo>
	{ 
		using type = TTo<Ts...>; 
	};

	template <class T1, template<class...> class T2>
	using rebind_t = typename rebind<T1, T2>::type;

	// apply_inner - applies each template argument from outer to inner
	template <class Outer, template<class...> class Inner> struct apply_inner;
	template <template<class...> class Outer, template<class...> class Inner, class... Ts>
	struct apply_inner<Outer<Ts...>, Inner>
	{
		using type = Outer<Inner<Ts>...>;
	};

	template <class Outer, template<typename...> class Inner>
	using apply_inner_t = typename apply_inner<Outer, Inner>::type;

	template <typename T>
	using                 is_storable   = std::disjunction<std::is_arithmetic<T>, std::conjunction<std::is_compound<T>, std::negation<std::is_function<T>>>>;
	template <typename T>
	constexpr bool        is_storable_v = is_storable<T>::value;

	// conditional_val - v1 if Test returns true and v2 otherwise
	template <bool Test, typename T, T v1, T v2>
	struct conditional_val
	{
		static constexpr T value = v2;
	};
	template <typename T, T v1, T v2>
	struct conditional_val<true, T, v1, v2>
	{
		static constexpr T value = v1;
	};

	template <bool Test, typename T, T v1, T v2>
	static constexpr T    conditional_v      = conditional_val<Test, T, v1, v2>::value;
	template <bool Test, bool v1>
	static constexpr bool conditional_bool_v = conditional_val<Test, bool, v1, false>::value;

	template <typename... Ts> struct type_list
	{
		template <template<class...> class TTo>
		using rebind              = rebind_t<type_list<Ts...>, TTo>;
		template <template<class...> class Inner>
		using apply               = apply_inner_t<type_list<Ts...>, Inner>;

		using unique              = type_list_unique<Ts...>;
		template <typename... Args>
		using append              = type_list<Ts..., Args...>;
		template <typename... Args>
		using append_unique       = type_list_unique<Ts..., Args...>;
		template <class... TLists>
		using append_lists        = type_list_cat_t<type_list<Ts...>, TLists...>;
		template <class... TLists>
		using append_lists_unique = type_list_unique<type_list<Ts...>, TLists...>;

		template <template <typename> class Predicate>
		using filter              = type_list_filter_t<Predicate, Ts...>;
		template <class TypeList>
		using cartesian_product   = cartesian_product_t<type_list<Ts...>, TypeList>;

		template <std::size_t idx>
		using extract             = type_list_extract_t<type_list<Ts...>, idx>;

		static constexpr std::size_t n_types      = sizeof... (Ts);
		static constexpr bool        empty        = n_types == 0;
		static constexpr bool        is_unique    = is_unique_v<Ts...>;
		static constexpr bool        all_storable = std::conjunction_v<is_storable<Ts>...>;

		template<typename T>
		static constexpr bool        contains     = contains_v<T, Ts...>;

		template<typename... Args>
		static constexpr bool        same         = std::is_same_v<type_list<Args...>, type_list<Ts...>>;

		template <typename... Args>
		static constexpr bool        convertible()
		{
			if constexpr (sizeof... (Args) == n_types)
				return std::conjunction_v<std::is_convertible<std::decay_t<Ts>, std::decay_t<Args>>...>;

			return false;
		}
	};
}