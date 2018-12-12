#pragma once
#include <algorithm>

namespace t_list
{
	template <typename... Ts> struct type_list;

	namespace detail
	{
		template <typename... Ts>
		using type_list = t_list::type_list<Ts...>;

		template <typename T1, typename T2> struct pair
		{
			using first = T1;
			using second = T2;
		};

		// type_nonexistant - Dummy type representing a nonexistant type
		struct type_nonexistant {};

		// Helper predicates
		template<typename T1>
		struct is_a
		{
			template<typename T2>
			struct helper
			{
				static constexpr bool value = std::is_same_v<T1, T2>;
			};
		};
		template<typename T1>
		using is_a_predicate = typename is_a<T1>::helper;

		template<typename T1>
		struct is_not_a
		{
			template<typename T2>
			struct helper
			{
				static constexpr bool value = !std::is_same_v<T1, T2>;
			};
		};
		template<typename T1>
		using is_not_a_predicate = typename is_not_a<T1>::helper;

		template<typename> struct reverse_base_case;
		template <template<typename...> class TList, typename... TArgs>
		struct reverse_base_case<TList<TArgs...>>
		{
			using type = TList<>;
		};

		template<typename TList, // Input
			typename = typename reverse_base_case<TList>::type> // Reversed output
			struct reverse;

		template<template <typename...> class TList, typename... Ts>
		struct reverse<typename reverse_base_case<TList<Ts...>>::type, TList<Ts...>>
		{
			using type = TList<Ts...>;
		};
		template<template<typename...> class T, typename x, typename... xs,
			typename... done>
			struct reverse<T<x, xs...>, T<done...>>
		{
			using type = typename reverse<T<xs...>, T<x, done...>>::type;
		};
		template<typename TList>
		using reverse_t = typename reverse<TList>::type;

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

		// back - Access last type in type_list
		template <typename TList> struct back;
		template <template <typename...> class TList>
		struct back<TList<>>
		{
			using type = type_nonexistant;
		};
		template <template <typename...> class TList, typename T>
		struct back<TList<T>>
		{
			using type = T;
		};
		template <template <typename...> class TList, typename T, typename... Ts>
		struct back<TList<T, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template<typename TList>
		using back_t = typename back<TList>::type;

		// append_conditional_v - add element to end of list if add == true
		template <typename TNew, typename TList, bool add> struct append_conditional_v;
		template <typename TNew, typename... Ts>
		struct append_conditional_v<TNew, type_list<Ts...>, true> { using type = type_list<Ts...>; };
		template <typename TNew, typename... Ts >
		struct append_conditional_v<TNew, type_list<Ts...>, false> { using type = type_list<Ts..., TNew>; };
		template <typename TNew, typename TList, bool add>
		using append_conditional_v_t = typename append_conditional_v<TNew, TList, add>::type;

		// append_conditional - Appends Ts... to TList if Predicate<T>::value is true
		template <template <typename> class Predicate, typename TList, typename... Ts> struct append_conditional;
		template <template <typename> class Predicate, template <typename...> class TList>
		struct append_conditional<Predicate, TList<>>
		{
			using type = TList<>;
		};
		template <template <typename> class Predicate, template <typename...> class TList, typename First, typename... Rest, typename... TLArgs>
		struct append_conditional<Predicate, TList<TLArgs...>, First, Rest...>
		{
			using type = std::conditional_t<Predicate<First>::value,
				typename append_conditional<Predicate, typename TList<TLArgs...>::template append<First>, Rest...>::type,
				typename append_conditional<Predicate, TList, Rest...>::type>;
		};
		template <template <typename> class Predicate, typename TList, typename... Ts>
		using append_conditional_t = typename append_conditional<Predicate, TList, Ts...>::type;

		// type_list_cat_conditional - Concatenate multiple type_list's each type is added if Predicate<T>::value == true
		template <template <typename> class Predicate, typename... TLists> struct type_list_cat_conditional;
		template <template <typename> class Predicate>
		struct type_list_cat_conditional<Predicate>
		{
			using type = type_list<>;
		};
		template <template <typename> class Predicate, template <typename...> class TList1, typename... TL1Args>
		struct type_list_cat_conditional<Predicate, TList1<TL1Args...>>
		{
			using type = type_list<TL1Args...>;
		};
		template <template <typename> class Predicate, template <typename...> class TList1, template <typename...> class TList2, typename... TL1Args, typename... TL2Args, class... TLists>
		struct type_list_cat_conditional<Predicate, TList1<TL1Args...>, TList2<TL2Args...>, TLists...>
		{
			using type = typename type_list_cat_conditional<Predicate, append_conditional_t<Predicate, type_list<TL1Args...>, TL2Args...>, TLists...>::type;
		};
		template <template <typename> class Predicate, typename... TLists>
		using type_list_cat_conditional_t = typename type_list_cat_conditional<Predicate, TLists...>::type;

		// append_conditional_front - Appends Ts... to front TList if Predicate<T>::value is true
		template <template <typename> class Predicate, typename TList, typename... Ts> struct append_conditional_front;
		template <template <typename> class Predicate, template <typename...> class TList>
		struct append_conditional_front<Predicate, TList<>>
		{
			using type = TList<>;
		};
		template <template <typename> class Predicate, template <typename...> class TList, typename First, typename... Rest, typename... TLArgs>
		struct append_conditional_front<Predicate, TList<TLArgs...>, First, Rest...>
		{
			using type = std::conditional_t<Predicate<First>::value,
				typename append_conditional_front<Predicate, typename TList<TLArgs...>::template append_front<First>, Rest...>::type,
				typename append_conditional_front<Predicate, TList, Rest...>::type>;
		};
		template <template <typename> class Predicate, typename TList, typename... Ts>
		using append_conditional_front_t = typename append_conditional_front<Predicate, TList, Ts...>::type;


		// append_conditional_binary_op - Appends TNew to TListAddTo if Predicate<TNew, TList>::value is true
		template <template <typename, typename> class Predicate, typename TNew, typename TListAddTo, typename TList> struct append_conditional_binary_op;
		template <template <typename, typename> class Predicate, typename TNew, template <typename...> class TListAddTo, template <typename...> class TList, typename... TListAddToTs, typename... TListTs>
		struct append_conditional_binary_op<Predicate, TNew, TListAddTo<TListAddToTs...>, TList<TListTs...>>
		{
			using type = std::conditional_t<Predicate<TNew, TList<TListTs...>>::value, TListAddTo<TNew, TListAddToTs...>, TListAddTo<TListAddToTs...>>;
		};
		template <template <typename, typename> class Predicate, typename TNew, typename TListAddTo, typename TList>
		using append_conditional_binary_op_t = typename append_conditional_binary_op<Predicate, TNew, TListAddTo, TList>::type;

		// binary_op - Adds all types in TList1 to a new type_list if Predicate<T, TList2>::value == true
		template <template <typename, typename> class Predicate, typename TList1, typename TList2> struct binary_op;
		template <template <typename, typename> class Predicate, template <typename...> class TList1, template <typename...> class TList2, typename First, typename... TL2Args>
		struct binary_op<Predicate, TList1<First>, TList2<TL2Args...>>
		{
			using type = append_conditional_binary_op_t<Predicate, First, type_list<>, TList2<TL2Args...>>;
		};
		template <template <typename, typename> class Predicate, template <typename...> class TList1, template <typename...> class TList2, typename First, typename... Rest, typename... TL2Args>
		struct binary_op<Predicate, TList1<First, Rest...>, TList2<TL2Args...>>
		{
			using type = append_conditional_binary_op_t<Predicate, First, typename binary_op<Predicate, TList1<Rest...>, TList2<TL2Args...>>::type, TList2<TL2Args...>>;
		};
		template <template <typename, typename> class Predicate, typename TList1, typename TList2>
		using binary_op_t = typename binary_op<Predicate, TList1, TList2>::type;

		template<typename T, typename TList> struct predicate_intersection;
		template<typename T, template <typename...> class TList, typename... Ts>
		struct predicate_intersection<T, TList<Ts...>>
		{
			static constexpr bool value = contains_v<T, Ts...>;
		};
		template<typename T, typename TList> struct predicate_difference;
		template<typename T, template <typename...> class TList, typename... Ts>
		struct predicate_difference<T, TList<Ts...>>
		{
			static constexpr bool value = !contains_v<T, Ts...>;
		};

		// intersection_t - type_list containing all types in both TList1 and TList2
		template<typename TList1, typename TList2>
		using intersection_t         = binary_op_t<predicate_intersection, TList1, TList2>;

		// predicate_difference_t - type_list containing all types in both TList1 that are not in TList2
		template<typename TList1, typename TList2>
		using difference_t           = binary_op_t<predicate_difference, TList1, TList2>;

		// symmetric_difference_t - type_list containing all types in both TList1 and TList2 but not in the intersection
		template<typename TList1, typename TList2, typename... TListRest>
		using symmetric_difference_t = typename difference_t<TList1, TList2>::template setop_union<difference_t<TList2, TList1>>;

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
			using type = append_conditional_v_t<THead, typename type_list_unique_helper<TTail...>::type, contains_v<THead, TTail...>>;
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
		template <std::size_t idx, typename... Ts>
		using extract_t = typename extract<idx, Ts...>::type;

		template <std::size_t idx, class TypeList> struct type_list_extract;
		template <std::size_t idx, template <class...> class TypeList, typename... Ts>
		struct type_list_extract<idx, TypeList<Ts...>>
		{
			using type = extract_t<idx, Ts...>;
		};
		template <class TypeList, std::size_t idx>
		using type_list_extract_t = typename type_list_extract<idx, TypeList>::type;

		// erase - Erase type at idx in Ts...
		template <std::size_t idx, typename... Ts>
		class erase
		{
			static_assert(idx < sizeof...(Ts), "idx out of bounds");

			template <std::size_t i, std::size_t n, typename... TTail>
			struct erase_impl;

			template <std::size_t i, std::size_t n, typename T, typename... TTail>
			struct erase_impl<i, n, T, TTail...>
			{
				using type = type_list_cat_t<type_list<T>, typename erase_impl<i + 1, n, TTail...>::type>;
			};

			template <std::size_t n, typename T, typename... TTail>
			struct erase_impl<n, n, T, TTail...>
			{
				using type = type_list<TTail...>;
			};

		public:
			using type = typename erase_impl<0, idx, Ts...>::type;
		};
		template <std::size_t idx, typename... Ts>
		using erase_t = typename erase<idx, Ts...>::type;

		template <std::size_t idx, class TypeList> struct type_list_erase;
		template <std::size_t idx, template <class...> class TypeList, typename... Ts>
		struct type_list_erase<idx, TypeList<Ts...>>
		{
			using type = erase_t<idx, Ts...>;
		};
		template <class TypeList, std::size_t idx>
		using type_list_erase_t = typename type_list_erase<idx, TypeList>::type;

		// cartesian_product - Cross Product of two type_lists
		//		type = pair<T, U>...;
		template <typename TList1, typename TList2> struct cartesian_product;
		template <template <typename...> class TList1, template <typename...> class TList2, typename... Us>
		struct cartesian_product<TList1<>, TList2<Us...>>
		{
			using type = type_list<>;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, typename T, typename... Ts, typename... Us>
		struct cartesian_product<TList1<T, Ts...>, TList2<Us...>>
		{
			using type = type_list_cat_t<type_list<pair<T, Us>...>,
				typename cartesian_product<type_list<Ts...>, type_list<Us...>>::type>;
		};
		template <typename TList1, typename TList2>
		using cartesian_product_t = typename cartesian_product<TList1, TList2>::type;

		// rebind - Rebinds template arguments from T1 to T2 where T1 and T2 are templates
		template <class TFrom, template<class...> class TTo> struct rebind;
		template <template<class...> class TFrom, class... Ts, template<typename...> class TTo>
		struct rebind<TFrom<Ts...>, TTo>
		{
			using type = TTo<Ts...>;
		};
		template <class T1, template<class...> class T2>
		using rebind_t = typename rebind<T1, T2>::type;

		// is_template_of_type - Checks to see if First, Rest... are all types of the same template
		template <template<class...> class TemplateOf, class First, class... Rest>
		struct is_template_of_type                                                    : std::integral_constant<bool, false> {};
		template <template<class...> class TemplateOf, template<class...> class First, class... FirstTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>>                     : std::integral_constant<bool, std::is_same_v<TemplateOf<FirstTs...>, First<FirstTs...>>> {};
		template <template<class...> class TemplateOf, template<class...> class First, template<class...> class... Rest, class... FirstTs, class... RestTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Rest<RestTs...>...> : std::integral_constant<bool, std::conjunction_v<std::is_same<TemplateOf<FirstTs...>, First<FirstTs...>>, std::is_same<TemplateOf<RestTs...>, Rest<RestTs...>>...>> {};
		template < template<class...> class TemplateOf, class First, class... Rest>
		constexpr bool is_template_of_type_v = is_template_of_type<TemplateOf, First, Rest...>::value;

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