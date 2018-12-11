#pragma once
#include <algorithm>

namespace t_list
{
	template <typename... Ts> struct type_list;

	namespace detail
	{
		template <typename... Ts>
		using type_list = t_list::type_list<Ts...>;

		// type_nonexistant - Dummy type representing a nonexistant type
		struct type_nonexistant {};

		// front - Access first type in type_list
		template <typename TList> struct front;
		template <template <typename...> class TList>
		struct front<TList<>>
		{
			using type = type_nonexistant;
		};
		template <template <typename...> class TList, typename T, typename... Ts>
		struct front<TList<T, Ts...>>
		{
			using type = T;
		};
		template<typename TList>
		using front_t = typename front<TList>::type;

		// pop_front - Remove first type in type_list
		template <typename TList> struct pop_front;
		template <template <typename...> class TList>
		struct pop_front<TList<>>
		{
			using type = TList<>;
		};
		template <template <typename...> class TList, typename T, typename... Ts>
		struct pop_front<TList<T, Ts...>>
		{
			using type = TList<Ts...>;
		};
		template <typename TList>
		using pop_front_t = typename pop_front<TList>::type;

		// type_list_cat - Concatenate multiple type_list's
		template <class... TLists> struct type_list_cat;
		template <> struct type_list_cat<>
		{
			using type = type_list<>;
		};
		template <template <typename...> class TList1, typename... TL1Args>
		struct type_list_cat<TList1<TL1Args...>>
		{
			using type = type_list<TL1Args...>;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, typename... TL1Args, typename... TL2Args, class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TLists...>
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

		// type_list_filter - Filter all elements of of type_list where Predicate::value is false
		template <template <typename> class Predicate, typename... Ts> struct type_list_filter;
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

		// append_conditional_front - add element to front list if add == true
		template <typename TNew, typename TList, bool add> struct append_conditional_front;
		template <typename TNew, typename... Ts>
		struct append_conditional_front<TNew, type_list<Ts...>, true>  { using type = type_list<Ts...>; };
		template <typename TNew, typename... Ts >
		struct append_conditional_front<TNew, type_list<Ts...>, false> { using type = type_list<TNew, Ts...>; };
		template <typename TNew, typename TList, bool add>
		using append_conditional_front_t = typename append_conditional_front<TNew, TList, add>::type;

		// append_conditional - add element to end list if add == true
		template <typename TNew, typename TList, bool add> struct append_conditional;
		template <typename TNew, typename... Ts>
		struct append_conditional<TNew, type_list<Ts...>, true>  { using type = type_list<Ts...>; };
		template <typename TNew, typename... Ts >
		struct append_conditional<TNew, type_list<Ts...>, false> { using type = type_list<Ts..., TNew>; };
		template <typename TNew, typename TList, bool add>
		using append_conditional_t = typename append_conditional<TNew, TList, add>::type;

		template <typename... Ts> struct type_list_unique_helper;
		template <> struct type_list_unique_helper<>
		{
			using type = type_list<>;
		};
		template <typename... TLArgs, typename... Args>
		struct type_list_unique_helper<type_list<TLArgs...>, Args...>
		{
			using type = typename type_list_unique_helper<TLArgs..., Args...>::type;
		};
		template <typename THead, typename... TTail>
		struct type_list_unique_helper<THead, TTail...>
		{
			using type = append_conditional_t<THead, typename type_list_unique_helper<TTail...>::type, contains_v<THead, TTail...>>;
		};
		// type_list_unique - Make unique type_list given list of types (also concatenates multiple type_lists)
		template <typename... Ts>
		using type_list_unique = typename type_list_unique_helper<Ts...>::type;

		// extract - Extract type at idx in Ts...
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
		template <template <typename...> class TList1, template <typename...> class TList2, typename... Us>
		struct cartesian_product<TList1<>, TList2<Us...>>
		{
			using type = type_list<>;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, typename T, typename... Ts, typename... Us>
		struct cartesian_product<TList1<T, Ts...>, TList2<Us...>>
		{
			using type = type_list_cat<type_list<pair<T, Us>...>,
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
		// TTo_First/TTo_Rest applied from left to right
		template <class TList, template<class...> class TTo_First, template<class...> class... TTo_Rest> struct apply;
		template <template<class...> class TList, template<class...> class TTo_First, class... Ts>
		struct apply<TList<Ts...>, TTo_First>
		{
			using type = TList<TTo_First<Ts>...>;
		};
		template <template<class...> class TList, template<class...> class TTo_First, template<class...> class... TTo_Rest, class... Ts>
		struct apply<TList<Ts...>, TTo_First, TTo_Rest...>
		{
			using type = typename apply<TList<TTo_First<Ts>...>, TTo_Rest...>::type;
		};
		template <class Outer, template<class...> class TTo_First, template<class...> class... TTo_Rest>
		using apply_t = typename apply<Outer, TTo_First, TTo_Rest...>::type;

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
		constexpr T    conditional_v      = conditional_val<Test, T, v1, v2>::value;
		template <bool Test, bool v1>
		constexpr bool conditional_bool_v = conditional_val<Test, bool, v1, false>::value;

		template <bool...> struct bool_pack;
		template <bool... v>
		using          all_true   = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;
		template <bool... v>
		constexpr bool all_true_v = all_true<v...>::value;

		// largest_type - type = largest type in Ts...
		template <typename... Ts> struct largest_type;
		template <>
		struct largest_type<>
		{
			using type = void;
		};
		template <typename T>
		struct largest_type<T>
		{
			using type = T;
		};
		template <typename T, typename U, typename... Ts>
		struct largest_type<T, U, Ts...>
		{
			using type = typename largest_type<typename std::conditional<
				(sizeof(U) <= sizeof(T)), T, U>::type, Ts...>::type;
		};
		template <typename... Ts>
		using largest_type_t = typename largest_type<Ts...>::type;

		// smallest_type - type = smallest type in Ts...
		template <typename... Ts> struct smallest_type;
		template <>
		struct smallest_type<>
		{
			using type = void;
		};
		template <typename T>
		struct smallest_type<T>
		{
			using type = T;
		};
		template <typename T, typename U, typename... Ts>
		struct smallest_type<T, U, Ts...>
		{
			using type = typename smallest_type<typename std::conditional<
				(sizeof(U) > sizeof(T)), T, U>::type, Ts...>::type;
		};
		template <typename... Ts>
		using smallest_type_t = typename smallest_type<Ts...>::type;
	}
}
