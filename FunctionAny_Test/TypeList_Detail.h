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

		template<typename TList, typename... Ts> struct append;
		template<template <typename...> class TList, typename... Ts, typename... TListTs>
		struct append<TList<TListTs...>, Ts...>
		{
			using type = type_list<TListTs..., Ts...>;
		};
		template<typename TList, typename... Ts>
		using append_t = typename append<TList, Ts...>::type;


		template<typename TList, // Input
			typename = type_list<>> // Reversed output
			struct reverse;

		template<template <typename...> class TList, typename... Ts>
		struct reverse<typename reverse_base_case<TList<Ts...>>::type, TList<Ts...>>
		{
			using type = TList<Ts...>;
		};
		template<template<typename...> class TList, typename First, typename... xs, typename... done>
		struct reverse<TList<First, xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>, TList<First, done...>>::type;
		};
		template<template<typename...> class TList, 
			     typename First, typename Second,
			     typename... xs, typename... done>
		struct reverse<TList<First, Second, xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>, 
				TList<Second, First, done...>>::type;
		};
		template<template<typename...> class TList,
			     typename First, typename Second, typename Third,
			     typename... xs, typename... done>
		struct reverse<TList<First, Second, Third,
			xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>,
				TList<Third, Second, First, done...>>::type;
		};
		template<template<typename...> class TList,
			     typename First, typename Second, typename Third, typename Fourth,
			     typename... xs, typename... done>
		struct reverse<TList<First, Second, Third, Fourth,
			xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>,
				TList<Fourth, Third, Second, First, done...>>::type;
		};
		template<template<typename...> class TList,
			     typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			     typename... xs, typename... done>
		struct reverse<TList<First, Second, Third, Fourth, Fifth,
							     xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>,
				TList<Fifth, Fourth, Third, Second, First, done...>>::type;
		};
		template<template<typename...> class TList,
			     typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			     typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			     typename... xs, typename... done>
		struct reverse<TList<First, Second, Third, Fourth, Fifth,
								 Sixth, Seventh, Eighth, Nineth, Tenth, 
		                         xs...>, TList<done...>>
		{
			using type = typename reverse<TList<xs...>, 
				TList<Tenth, Nineth, Eighth, Seventh, Sixth, Fifth, Fourth, Third, Second, First, done...>>::type;
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
		template <template <typename...> class TList1, template <typename...> class TList2, 
				  typename... TL1Args, typename... TL2Args, 
			      class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TLists...>
		{
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args...>, TLists...>::type;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, template <typename...> class TList3,
				  typename... TL1Args, typename... TL2Args, typename... TL3Args,
			      class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TList3<TL3Args...>, TLists...>
		{
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args..., TL3Args...>, TLists...>::type;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, template <typename...> class TList3, template <typename...> class TList4,
			      typename... TL1Args, typename... TL2Args, typename... TL3Args, typename... TL4Args,
			      class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TList3<TL3Args...>, TList4<TL4Args...>, TLists...>
		{
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args..., TL3Args..., TL4Args...>, TLists...>::type;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, template <typename...> class TList3, template <typename...> class TList4, template <typename...> class TList5,
			      typename... TL1Args, typename... TL2Args, typename... TL3Args, typename... TL4Args, typename... TL5Args,
			      class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TList3<TL3Args...>, TList4<TL4Args...>, TList5<TL5Args...>, TLists...>
		{
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args..., TL3Args..., TL4Args..., TL5Args...>, TLists...>::type;
		};
		template <template <typename...> class TList1, template <typename...> class TList2, template <typename...> class TList3, template <typename...> class TList4, template <typename...> class TList5,
				  template <typename...> class TList6, template <typename...> class TList7, template <typename...> class TList8, template <typename...> class TList9, template <typename...> class TList10,
			      typename... TL1Args, typename... TL2Args, typename... TL3Args, typename... TL4Args, typename... TL5Args,
			      typename... TL6Args, typename... TL7Args, typename... TL8Args, typename... TL9Args, typename... TL10Args,
			      class... TLists>
		struct type_list_cat<TList1<TL1Args...>, TList2<TL2Args...>, TList3<TL3Args...>, TList4<TL4Args...>, TList5<TL5Args...>, 
							 TList6<TL6Args...>, TList7<TL7Args...>, TList8<TL8Args...>, TList9<TL9Args...>, TList10<TL10Args...>,
							 TLists...>
		{
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args..., TL3Args..., TL4Args..., TL5Args..., 
														  TL6Args..., TL7Args..., TL8Args..., TL9Args..., TL10Args...>,
														  TLists...>::type;
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
		template <template <typename> class Predicate, typename First>
		struct filter<Predicate, First>
		{
			using type = std::conditional_t<Predicate<First>::value, type_list<First>, type_list<>>;
		};
		template <template <typename> class Predicate, typename First, typename... Ts>
		class filter<Predicate, First, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, First>;
		};
		template <template <typename> class Predicate,
			     typename First, typename Second,
			     typename... Ts>
		class filter<Predicate, First, Second, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, Second>::
				template append_conditional_front<Predicate, First>;
		};
		template <template <typename> class Predicate,
			      typename First, typename Second, typename Third,
			      typename... Ts>
		class filter<Predicate, First, Second, Third, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, Third>::
				template append_conditional_front<Predicate, Second>::
				template append_conditional_front<Predicate, First>;
		};
		template <template <typename> class Predicate,
			      typename First, typename Second, typename Third, typename Fourth,
			      typename... Ts>
		class filter<Predicate, First, Second, Third, Fourth, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, Fourth>::
				template append_conditional_front<Predicate, Third>::
				template append_conditional_front<Predicate, Second>::
				template append_conditional_front<Predicate, First>;
		};
		template <template <typename> class Predicate,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename... Ts>
		class filter<Predicate, First, Second, Third, Fourth, Fifth, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, Fifth>::				
				template append_conditional_front<Predicate, Fourth>::
				template append_conditional_front<Predicate, Third>::
				template append_conditional_front<Predicate, Second>::
				template append_conditional_front<Predicate, First>;
		};
		template <template <typename> class Predicate,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			      typename... Ts>
		class filter<Predicate, First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Nineth, Tenth, Ts...>
		{
			using helper_t = typename filter<Predicate, Ts...>::type;
		public:
			using type = typename helper_t::
				template append_conditional_front<Predicate, Tenth>::
				template append_conditional_front<Predicate, Nineth>::
				template append_conditional_front<Predicate, Eighth>::
				template append_conditional_front<Predicate, Seventh>::
				template append_conditional_front<Predicate, Sixth>::
				template append_conditional_front<Predicate, Fifth>::
				template append_conditional_front<Predicate, Fourth>::
				template append_conditional_front<Predicate, Third>::
				template append_conditional_front<Predicate, Second>::
				template append_conditional_front<Predicate, First>;
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
		template <template <typename...> class TList, typename First>
		struct back<TList<First>>
		{
			using type = First;
		};
		template <template <typename...> class TList, typename First, typename... Ts>
		struct back<TList<First, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template <template <typename...> class TList, 
			typename First, typename Second>
		struct back<TList<First, Second>>
		{
			using type = Second;
		};
		template <template <typename...> class TList, 
			typename First, typename Second,
			typename... Ts>
		struct back<TList<First, Second, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third>
		struct back<TList<First, Second, Third>>
		{
			using type = Third;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third,
			      typename... Ts>
		struct back<TList<First, Second, Third, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth>
		struct back<TList<First, Second, Third, Fourth>>
		{
			using type = Fourth;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth,
			      typename... Ts>
		struct back<TList<First, Second, Third, Fourth, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth>
		struct back<TList<First, Second, Third, Fourth, Fifth>>
		{
			using type = Fifth;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename... Ts>
		struct back<TList<First, Second, Third, Fourth, Fifth, Ts...>>
		{
			using type = typename back<TList<Ts...>>::type;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth>
		struct back<TList<First, Second, Third, Fourth, Fifth,
						      Sixth, Seventh, Eighth, Nineth, Tenth>>
		{
			using type = Tenth;
		};
		template <template <typename...> class TList,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			      typename... Ts>
		struct back<TList<First, Second, Third, Fourth, Fifth,
							  Sixth, Seventh, Eighth, Nineth, Tenth,
							  Ts...>>
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
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename First, typename... TListAddToTs>
		struct pop_back<TList<First>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs...>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename First, typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			typename First, typename Second,
			typename... TListAddToTs>
			struct pop_back<TList<First, Second>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third,
			      typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First, Second>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second, Third>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth,
			      typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First, Second, Third>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second, Third, Fourth>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, Fifth>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First, Second, Third, Fourth>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, Fifth, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second, Third, Fourth, Fifth>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			      typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, Fifth,
								  Sixth, Seventh, Eighth, Nineth, Tenth>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Nineth>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, Fifth,
			Sixth, Seventh, Eighth, Nineth, Tenth, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Nineth, Tenth>>::type;
		};
		template<typename TList>
		using pop_back_t = typename pop_back<TList>::type;

		template<typename TList, typename... Ts>
		struct append_unique;
		template<template <typename...> class TList, typename... Ts>
		struct append_unique<TList<Ts...>>
		{
			using type = type_list<Ts...>;
		};
		template<template <typename...> class TList, typename T, typename... Ts>
		struct append_unique<TList<Ts...>, T>
		{
			using type = std::conditional_t<contains_v<T, Ts...>, type_list<Ts...>, type_list<Ts..., T>>;
		};
		// Appends 0 or 1 types to type_list if it does not already exist in the list
		template<typename TList, typename... Ts>
		using append_unique_t = typename append_unique<TList, Ts...>::type;

		// type_list_unique - Make unique type_list given TList
		template <typename TList, typename = type_list<>> struct type_list_unique;
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename... TListAddToTs>
		struct type_list_unique<TList<>, TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs...>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename First, typename... TListAddToTs>
		struct type_list_unique<TList<First>, TListAddTo<TListAddToTs...>>
		{
			using type = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, typename First, typename... TLTs, typename... TListAddToTs>
		struct type_list_unique<TList<First, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using type = typename type_list_unique<TList<TLTs...>,
				std::conditional_t<contains_not_v<First, TListAddToTs...>,
				TListAddTo<TListAddToTs..., First>,
				TListAddTo<TListAddToTs...>>>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo, 
			typename First, typename Second, 
			typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, TLTs...>, TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t<One, Second>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			typename First, typename Second, typename Third,
			typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, Third, TLTs...>,
			TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t<
				append_unique_t<One, Second>, Third>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			typename First, typename Second, typename Third, typename Fourth,
			typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, Third, Fourth, TLTs...>,
			TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t<append_unique_t<
				append_unique_t<One, Second>, Third>, Fourth>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			      typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, Third, Fourth, Fifth, TLTs...>,
						                 TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t<append_unique_t<append_unique_t<
				append_unique_t<One, Second>, Third>, Fourth>, Fifth>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
				  typename First, typename Second, typename Third, typename Fourth, typename Fifth, 
				  typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
				  typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, Third, Fourth, Fifth, 
										 Sixth, Seventh, Eighth, Nineth, Tenth, TLTs...>, 
										 TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t<append_unique_t<
				append_unique_t<append_unique_t<append_unique_t<
				append_unique_t<append_unique_t<append_unique_t<
				append_unique_t<One, Second>, Third>, Fourth>, Fifth>, 
				Sixth>, Seventh>, Eighth>, Nineth>, Tenth>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
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
		template <template<class...> class TemplateOf, template<class...> class First, class... Rest, class... FirstTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Rest...>
		{
			static constexpr bool value = std::is_same_v<
				TemplateOf<FirstTs...>, First<FirstTs...>> && 
				is_template_of_type<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf, 
			      template<class...> class First, template<class...> class Second, 
			      class... Rest, 
			      class... FirstTs, class... SecondTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Rest...>
		{
			static constexpr bool value = 
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   && 
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				is_template_of_type<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			      template<class...> class First, template<class...> class Second, template<class...> class Third,
			      class... Rest,
			      class... FirstTs, class... SecondTs, class... ThirdTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Rest...>
		{
			static constexpr bool value = 
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   && 
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				is_template_of_type<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			      template<class...> class First, template<class...> class Second, template<class...> class Third, template<class...> class Fourth,
			      class... Rest,
			      class... FirstTs, class... SecondTs, class... ThirdTs, class... FourthTs>
		struct is_template_of_type<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Fourth<FourthTs...>, Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				std::is_same_v<TemplateOf<FourthTs...>, Fourth<FourthTs...>> &&
				is_template_of_type<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			      template<class...> class First, template<class...> class Second, template<class...> class Third, template<class...> class Fourth, template<class...> class Fifth,
			      class... Rest,
			      class... FirstTs, class... SecondTs, class... ThirdTs, class... FourthTs, class... FifthTs>
		struct is_template_of_type<TemplateOf, 
			First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Fourth<FourthTs...>, Fifth<FifthTs...>, 
			Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				std::is_same_v<TemplateOf<FourthTs...>, Fourth<FourthTs...>> &&
				std::is_same_v<TemplateOf<FifthTs...>,  Fifth<FifthTs...>>   &&
				is_template_of_type<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			      template<class...> class First, template<class...> class Second, template<class...> class Third, template<class...> class Fourth, template<class...> class Fifth,
			      template<class...> class Sixth, template<class...> class Seventh, template<class...> class Eighth, template<class...> class Nineth, template<class...> class Tenth,
			      class... Rest,
			      class... FirstTs, class... SecondTs, class... ThirdTs, class... FourthTs, class... FifthTs,
			      class... SixthTs, class... SeventhTs, class... EighthTs, class... NinethTs, class... TenthTs>
		struct is_template_of_type<TemplateOf, 
			First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Fourth<FourthTs...>, Fifth<FifthTs...>, 
			Sixth<SixthTs...>, Seventh<SeventhTs...>, Eighth<EighthTs...>, Nineth<NinethTs...>, Tenth<TenthTs...>,
			Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,   First<FirstTs...>>     &&
				std::is_same_v<TemplateOf<SecondTs...>,  Second<SecondTs...>>   &&
				std::is_same_v<TemplateOf<ThirdTs...>,   Third<ThirdTs...>>     &&
				std::is_same_v<TemplateOf<FourthTs...>,  Fourth<FourthTs...>>   &&
				std::is_same_v<TemplateOf<FifthTs...>,   Fifth<FifthTs...>>     &&
				std::is_same_v<TemplateOf<SixthTs...>,   Sixth<SixthTs...>>     &&
				std::is_same_v<TemplateOf<SeventhTs...>, Seventh<SeventhTs...>> &&
				std::is_same_v<TemplateOf<EighthTs...>,  Eighth<EighthTs...>>   &&
				std::is_same_v<TemplateOf<NinethTs...>,  Nineth<NinethTs...>>   &&
				std::is_same_v<TemplateOf<TenthTs...>,   Tenth<TenthTs...>>     &&
				is_template_of_type<TemplateOf, Rest...>::value;
		};

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
		template <template<class...> class TList,
			template<class...> class TTo_First, template<class...> class TTo_Second,
			class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First, TTo_Second>
		{
			using type = TList<TTo_Second<TTo_First<Ts>>...>;
		};
		template <template<class...> class TList, 
			template<class...> class TTo_First, template<class...> class TTo_Second, 
			template<class...> class... TTo_Rest, class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First, TTo_Second, TTo_Rest...>
		{
			using type = typename apply_unary<TList<TTo_Second<TTo_First<Ts>...>>, TTo_Rest...>::type;
		};
		template <template<class...> class TList, 
			template<class...> class TTo_First, template<class...> class TTo_Second, template<class...> class TTo_Third,
			class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First, TTo_Second, TTo_Third>
		{
			using type = typename TList<TTo_Third<TTo_Second<TTo_First<Ts>>>...>;
		};
		template <template<class...> class TList,
			template<class...> class TTo_First, template<class...> class TTo_Second, template<class...> class TTo_Third,
			template<class...> class... TTo_Rest, class... Ts>
		struct apply_unary<TList<Ts...>, TTo_First, TTo_Second, TTo_Third, TTo_Rest...>
		{
			using type = typename apply_unary<TList<TTo_Third<TTo_Second<TTo_First<Ts>...>>>, TTo_Rest...>::type;
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
		template <template<class...> class TList1, template<class...> class TList2,
			template<class...> class TTo_First, template<class...> class TTo_Second,
			class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First, TTo_Second>
		{
			using type = type_list<TTo_Second<TTo_First<TL1Ts, TL2Ts>>...>;
		};
		template <template<class...> class TList1, template<class...> class TList2, 
			template<class...> class TTo_First, template<class...> class TTo_Second, 
			template<class...> class... TTo_Rest, class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First, TTo_Second, TTo_Rest...>
		{
			using type = typename apply_binary<type_list<TTo_Second<TTo_First<TL1Ts, TL2Ts>>...>, TTo_Rest...>::type;
		};
		template <template<class...> class TList1, template<class...> class TList2,
			template<class...> class TTo_First, template<class...> class TTo_Second, template<class...> class TTo_Third,
			class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First, TTo_Second, TTo_Third>
		{
			using type = type_list<TTo_Third<TTo_Second<TTo_First<TL1Ts, TL2Ts>>>...>;
		};
		template <template<class...> class TList1, template<class...> class TList2,
			template<class...> class TTo_First, template<class...> class TTo_Second, template<class...> class TTo_Third,
			template<class...> class... TTo_Rest, class... TL1Ts, class... TL2Ts>
		struct apply_binary<TList1<TL1Ts...>, TList2<TL2Ts...>, TTo_First, TTo_Second, TTo_Third, TTo_Rest...>
		{
			using type = typename apply_binary<type_list<TTo_Third<TTo_Second<TTo_First<TL1Ts, TL2Ts>>>...>, TTo_Rest...>::type;
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