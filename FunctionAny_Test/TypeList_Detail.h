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
		constexpr bool contains_v      = contains<T, Ts...>::value;
		template <typename T, typename... Ts>
		constexpr bool contains_not_v = !contains_v<T, Ts...>;

		// type_list_filter - Filter all elements of of type_list where Predicate::value is false
		template <template <typename> class Predicate, typename... Ts> struct filter;
		template <template <typename> class Predicate>
		struct filter<Predicate>
		{
			using type = type_list<>;
		};
		template <template <typename> class Predicate, typename T>
		struct filter<Predicate, T>
		{
			using type = std::conditional_t<Predicate<T>::value, type_list<T>, type_list<>>;
		};
		template <template <typename> class Predicate, typename T, typename... Ts>
		class filter<Predicate, T, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = std::conditional_t<Predicate<T>::value, typename helper_t::template append_front<T>, helper_t>;
		};
		template <template <typename> class Predicate, typename... Ts>
		using filter_t = typename filter<Predicate, Ts...>::type;

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

		// pop_back - Remove last type in type_list
		template <typename TList, typename = type_list<>> struct pop_back;
		template <template <typename...> class TList>
		struct pop_back<TList<>>
		{
			using type = type_list<>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename T, typename... TListAddToTs>
		struct pop_back<TList<T>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs...>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename T, typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<T, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., T>>::type;
		};

		template<typename TList>
		using pop_back_t = typename pop_back<TList>::type;

		// type_list_unique - Make unique type_list given TList
		template <typename TList, typename = type_list<>> struct type_list_unique;
		template <template <typename...> class TList>
		struct type_list_unique<TList<>>
		{
			using type = type_list<>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename T, typename... TListAddToTs>
		struct type_list_unique<TList<T>, TListAddTo<TListAddToTs...>>
		{
			using type = std::conditional_t<contains_not_v<T, TListAddToTs...>,
				type_list<TListAddToTs..., T>,
				type_list<TListAddToTs...>>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename T, typename... TLTs, typename... TListAddToTs>
		struct type_list_unique<TList<T, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename type_list_unique<TList<TLTs...>,
				std::conditional_t<contains_not_v<T, TListAddToTs...>,
				TListAddTo<TListAddToTs..., T>,
				TListAddTo<TListAddToTs...>>>::type;
		};

		template <typename TList>
		using type_list_unique_t = typename type_list_unique<TList>::type;

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
		template <template<class...> class TemplateOf, class... Ts>
		struct is_template_of_type
		{
			static constexpr bool value = false;
		};
		template <template<class...> class TemplateOf>
		struct is_template_of_type<TemplateOf>
		{
			static constexpr bool value = true;
		};
		template <template<class...> class TemplateOf, template<class...> class First, class... FirstTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>>
		{
			static constexpr bool value = std::is_same_v<TemplateOf<FirstTs...>, First<FirstTs...>>;
		};
		template <template<class...> class TemplateOf, template<class...> class First, class... Rest, class... FirstTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Rest...>
		{
			static constexpr bool value = std::is_same_v<TemplateOf<FirstTs...>, First<FirstTs...>> && is_template_of_type<TemplateOf, Rest...>::value;
		};
		//template <template<class...> class TemplateOf, typename First, typename... Rest>
		//struct is_template_of_type<TemplateOf, First, Rest...>                        : std::integral_constant<bool, false> {};
		template < template<class...> class TemplateOf, class... Ts>
		constexpr bool is_template_of_type_v = is_template_of_type<TemplateOf, Ts...>::value;

		// is_template_of_type - Checks to see if First, Rest... are all types of the same template
		template <class TypeTemplateOf, class... Ts>
		struct is_template_of_type_type;
		template <template<class...> class TemplateOf, class... TemplateOfTs, class... Ts>
		struct is_template_of_type_type<TemplateOf<TemplateOfTs...>, Ts...> : is_template_of_type<TemplateOf, Ts...> {};
		template <class TypeTemplateOf, class... Ts>
		constexpr bool is_template_of_type_type_v = is_template_of_type_type<TypeTemplateOf, Ts...>::value;

		// apply_unary - applies each template argument from outer to inner (takes 1 type_list as parameterse)
		// TTo_First/TTo_Rest applied from left to right 
		template <class TList, template<class...> class TTo_First, template<class...> class... TTo_Rest> struct apply_unary;
		template <template<class...> class TList, template<class...> class TTo_First, class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First>
		{
			using type = TList<TTo_First<Ts>...>;
		};
		template <template<class...> class TList, template<class...> class TTo_First, template<class...> class... TTo_Rest, class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First, TTo_Rest...>
		{
			using type = typename apply_unary<TList<TTo_First<Ts>...>, TTo_Rest...>::type;
		};
		template <class TList, template<class...> class TTo_First, template<class...> class... TTo_Rest>
		using apply_unary_t = typename apply_unary<TList, TTo_First, TTo_Rest...>::type;

		// apply_binary - applies each template argument from outer to inner (takes 2 type_list's as parameterse)
		// TTo_First/TTo_Rest applied from left to right
		template <class TList1, class TList2, template<class...> class TTo_First, template<class...> class... TTo_Rest> struct apply_binary;
		template <template<class...> class TList1, template<class...> class TList2, template<class...> class TTo_First, class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First>
		{
			static_assert(sizeof...(TL1Ts) == sizeof...(TL2Ts), "Error: apply_binary expected matched list sizes");
			using type = type_list<TTo_First<TL1Ts, TL2Ts>...>;
		};
		template <template<class...> class TList1, template<class...> class TList2, template<class...> class TTo_First, template<class...> class... TTo_Rest, class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First, TTo_Rest...>
		{
			using type = typename apply_binary<type_list<TTo_First<TL1Ts, TL2Ts>...>, TTo_Rest...>::type;
		};
		template <class TList1, class TList2, template<class...> class TTo_First, template<class...> class... TTo_Rest>
		using apply_binary_t = typename apply_binary<TList1, TList2, TTo_First, TTo_Rest...>::type;

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

		// Helper structs for use in type_list
		template <template<typename, typename> class Predicate, typename TList1, typename TList2>
		struct all_match_predicate_list;
		template <template<typename, typename> class Predicate, typename TList1, template<typename...> class TList2, typename... Ts>
		struct all_match_predicate_list<Predicate, TList1, TList2<Ts...>>
		{
			static constexpr bool value = TList1::template all_match_predicate<Ts...>();
		};
		template <template<typename, typename> class Predicate, typename TList1, typename TList2>
		constexpr bool all_match_predicate_list_v = all_match_predicate_list<Predicate, TList1, TList2>::value;

		template <typename TList1, typename TList2>
		struct is_convertible_list;
		template <typename TList1, template<typename...> class TList2, typename... Ts>
		struct is_convertible_list<TList1, TList2<Ts...>>
		{
			static_assert(is_template_of_type_v<type_list, TList1>, "Error: is_not_type_list_overload expected TList1 to be of type t_list::type_list<Ts...>");
			static constexpr bool value = TList1::template is_convertible<Ts...>();
		};
		template <typename TList1, typename TList2>
		constexpr bool is_convertible_list_v = is_convertible_list<TList1, TList2>::value;

		template <typename TList1, typename TList2>
		struct contains_convertible_list;
		template <template<typename...> class TList1, typename TList2, typename T>
		struct contains_convertible_list<TList1<T>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, T>, "Error: contains_convertible_list expected TList2 and T to be of type t_list::type_list<Ts...>");
			static constexpr bool value = TList2::template is_convertible_list<T>();
		};
		template <template<typename...> class TList1, typename TList2, typename T, typename... Ts>
		struct contains_convertible_list<TList1<T, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, T>, "Error: contains_convertible_list expected TList2 and T to be of type t_list::type_list<Ts...>");
			static constexpr bool value = conditional_v<TList2::template is_convertible_list<T>(), bool, true, contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <typename TList1, typename TList2>
		constexpr bool contains_convertible_list_v = contains_convertible_list<TList1, TList2>::value;

		template <typename TList1, typename TList2>
		struct is_not_type_list_overload;
		template <typename TList1, template<typename...> class TList2, typename... Ts>
		class is_not_type_list_overload<TList1, TList2<Ts...>>
		{
			static_assert(is_template_of_type_v<type_list, TList1>, "Error: is_not_type_list_overload expected TList1 to be of type t_list::type_list<Ts...>");
			static constexpr bool n_types = sizeof...(Ts);
		public:
			static constexpr bool value = conditional_v<(n_types != 1), bool, true, !detail::is_template_of_type_v<type_list, typename TList1::front>>;
		};
		template <typename TList1, typename TList2>
		constexpr bool is_not_type_list_overload_v = is_not_type_list_overload<TList1, TList2>::value;

		template <typename TList1, typename TList2>
		struct is_equivalent
		{
			static_assert(is_template_of_type_v<type_list, TList1, TList2>, "Error: is_equivalent expected TList1 and TList2 to be of type t_list::type_list<Ts...>");
			static constexpr bool value = conditional_bool_v<(TList1::n_types == TList2::n_types), TList1::template is_subset<TList2>>;
		};
		template <typename TList1, typename TList2>
		constexpr bool is_equivalent_v = is_equivalent<TList1, TList2>::value;
	}
}