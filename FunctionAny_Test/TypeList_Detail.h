#pragma once
#include <algorithm>
#include <type_traits>
#include "TypeList_Helpers.h"

namespace t_list
{
	template <typename... Ts> struct type_list;

	namespace detail
	{
		template <typename... Ts>
		using type_list = t_list::type_list<Ts...>;

		template<typename TList, typename... Ts> struct append;
		template<template <typename...> class TList, typename... Ts, typename... TListTs>
		struct append<TList<TListTs...>, Ts...>
		{
			using type = type_list<TListTs..., Ts...>;
		};
		template<typename TList, typename... Ts>
		using append_t = typename append<TList, Ts...>::type;

		template<typename> struct reverse_base_case;
		template <template<typename...> class TList, typename... TArgs>
		struct reverse_base_case<TList<TArgs...>>
		{
			using type = TList<>;
		};

		template<typename TList, typename = type_list<>> 
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
			     typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
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
			using type = typename type_list_cat<type_list<TL1Args..., TL2Args..., TL3Args..., TL4Args..., TL5Args...  , 
														  TL6Args..., TL7Args..., TL8Args..., TL9Args..., TL10Args...>,
														  TLists...>::type;
		};
		template <class... TLists>
		using type_list_cat_t = typename type_list_cat<TLists...>::type;

		// contains_v - true if Ts contains type T
		template <typename T, typename... Ts>
		constexpr bool contains_v      = one_true_v<std::is_same_v<T, Ts>...>;
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
				template append_conditional_front<Predicate, Second> ::
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
				template append_conditional_front<Predicate, Third>  ::
				template append_conditional_front<Predicate, Second> ::
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
				template append_conditional_front<Predicate, Fourth> ::
				template append_conditional_front<Predicate, Third>  ::
				template append_conditional_front<Predicate, Second> ::
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
				template append_conditional_front<Predicate, Fifth>  ::				
				template append_conditional_front<Predicate, Fourth> ::
				template append_conditional_front<Predicate, Third>  ::
				template append_conditional_front<Predicate, Second> ::
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
				template append_conditional_front<Predicate, Tenth>   ::
				template append_conditional_front<Predicate, Nineth>  ::
				template append_conditional_front<Predicate, Eighth>  ::
				template append_conditional_front<Predicate, Seventh> ::
				template append_conditional_front<Predicate, Sixth>   ::
				template append_conditional_front<Predicate, Fifth>   ::
				template append_conditional_front<Predicate, Fourth>  ::
				template append_conditional_front<Predicate, Third>   ::
				template append_conditional_front<Predicate, Second>  ::
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
			      typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth>
		struct back<TList<First, Second, Third, Fourth, Fifth,
						  Sixth, Seventh, Eighth, Nineth, Tenth>>
		{
			using type = Tenth;
		};
		template <template <typename...> class TList,
			      typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
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
							  Sixth, Seventh, Eighth, Nineth, Tenth>, 
			            TListAddTo<TListAddToTs...>>
		{
			using type = type_list<TListAddToTs..., First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Nineth>;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
			      typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
			      typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			      typename... TLTs, typename... TListAddToTs>
		struct pop_back<TList<First, Second, Third, Fourth, Fifth,
			                  Sixth, Seventh, Eighth, Nineth, Tenth, TLTs...>, 
			            TListAddTo<TListAddToTs...>>
		{
			using type = typename pop_back<TList<TLTs...>, TListAddTo<TListAddToTs..., First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Nineth, Tenth>>::type;
		};
		template<typename TList>
		using pop_back_t = typename pop_back<TList>::type;

		// append_unique - Appends 0 or more types to type_list if it does not already exist in the list
		template<typename TList, typename... Ts> struct append_unique;
		template<template <typename...> class TList, typename... TLTs>
		struct append_unique<TList<TLTs...>>
		{
			using type = TList<TLTs...>;
		};
		template<template <typename...> class TList, typename First, typename... Ts, typename... TLTs>
		class append_unique<TList<TLTs...>, First, Ts...>
		{
			using type_helper = std::conditional_t<contains_v<First, TLTs...>, type_list<TLTs...>, type_list<TLTs..., First>>;
		public:
			using type = typename append_unique<type_helper, Ts...>::type;
		};
		template<template <typename...> class TList, typename First, typename Second, typename... Ts, typename... TLTs>
		class append_unique<TList<TLTs...>, First, Second, Ts...>
		{
			static constexpr bool contains_first  = contains_v<First, TLTs...>;
			static constexpr bool contains_second = contains_v<Second, First, TLTs...>;

			using type_helper = std::conditional_t<(contains_first && contains_second), type_list<TLTs...>,
				std::conditional_t<!(contains_first || contains_second), type_list<TLTs..., First, Second>,
				std::conditional_t<!contains_first, type_list<TLTs..., First>, type_list<TLTs..., Second>>>>;
		public:
			using type = typename append_unique<type_helper, Ts...>::type;
		};
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

			using type_helper = append_unique_t
				<
				One,
				Second, 
				Third
				>;
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

			using type_helper = append_unique_t
				<
				One,
				Second,
				Third,
				Fourth
			>;
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

			using type_helper = append_unique_t
				<
				One,
				Second,
				Third,
				Fourth,
				Fifth
				>;
		public:
			using type = typename type_list_unique<TList<TLTs...>, type_helper>::type;
		};
		template <template <typename...> class TList, template <typename...> class TListAddTo,
				  typename First, typename Second,  typename Third,  typename Fourth, typename Fifth, 
				  typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
				  typename... TLTs, typename... TListAddToTs>
		class type_list_unique<TList<First, Second, Third, Fourth, Fifth, 
										 Sixth, Seventh, Eighth, Nineth, Tenth, TLTs...>, 
										 TListAddTo<TListAddToTs...>>
		{
			using One = std::conditional_t<contains_not_v<First, TListAddToTs...>,
				type_list<TListAddToTs..., First>,
				type_list<TListAddToTs...>>;

			using type_helper = append_unique_t
				<
				One,
				Second,
				Third,
				Fourth,
				Fifth,
				Sixth,
				Seventh,
				Eighth,
				Nineth,
				Tenth
				>;
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
		using                 is_storable   = std::negation<std::disjunction<std::is_void<T>, std::is_function<T>>>;
		template <typename T>
		constexpr bool        is_storable_v = !(std::is_void_v<T> || std::is_function_v<T>);

		// smallest_type - smallest type in Ts...
		// largest_type  - largest  type in Ts...
		template <typename... Ts> struct type_helper;
		template <> struct type_helper<>
		{
			using smallest_type = void;
			using largest_type  = void;
		};
		template <typename T>
		struct type_helper<T>
		{
			using smallest_type = T;
			using largest_type  = T;
		};
		template <typename T, typename U, typename... Ts>
		struct type_helper<T, U, Ts...>
		{
			using smallest_type = typename type_helper<std::conditional_t<
				(sizeof(U) > sizeof(T)),  T, U>, Ts...>::smallest_type;
			using largest_type = typename type_helper<std::conditional_t<
				(sizeof(U) <= sizeof(T)), T, U>, Ts...>::largest_type;
		};
		template <typename... Ts>
		using smallest_type_t = typename type_helper<Ts...>::smallest_type;
		template <typename... Ts>
		using largest_type_t  = typename type_helper<Ts...>::largest_type;
	}
}