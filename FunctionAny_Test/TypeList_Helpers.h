#pragma once
#include <type_traits>

namespace t_list
{
	template <typename... Ts> struct type_list;

	namespace detail
	{
		// type_nonexistant - Dummy type representing a nonexistant type
		struct type_nonexistant {};

		// Helper predicates
		template<typename T1>
		struct binary_to_unary_predicate
		{
			template<typename T2>
			struct is_a
			{
				static constexpr bool value = std::is_same_v<T1, T2>;
			};
			template<typename T2>
			struct is_not_a
			{
				static constexpr bool value = !std::is_same_v<T1, T2>;
			};
		};
		template<typename T1>
		using is_a_predicate     = typename binary_to_unary_predicate<T1>::is_a;
		template<typename T1>
		using is_not_a_predicate = typename binary_to_unary_predicate<T1>::is_not_a;

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
		constexpr T    conditional_v             = conditional_val<Test, T, v1, v2>::value;
		template <bool Test, bool v1>
		constexpr bool conditional_bool_v        = conditional_val<Test, bool, v1, false>::value;
		template <bool Test, bool v1, bool v2>
		constexpr bool conditional_bool_binary_v = conditional_val<Test, bool, v1, v2>::value;

		// bool pack - used like std::conjunction except works on bools instead of predicates
		template <bool...> struct bool_pack;
		template <bool... v>
		using          all_true   = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;
		template <bool... v>
		constexpr bool all_true_v = all_true<v...>::value;

		// bool pack - used like std::conjunction except works on bools instead of predicates
		template <bool... v>
		constexpr bool one_true_v = (v || ...);

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
			template<class...> class First, template<class...> class Second,  template<class...> class Third,  template<class...> class Fourth, template<class...> class Fifth,
			template<class...> class Sixth, template<class...> class Seventh, template<class...> class Eighth, template<class...> class Nineth, template<class...> class Tenth,
			class... Rest,
			class... FirstTs, class... SecondTs,  class... ThirdTs,  class... FourthTs, class... FifthTs,
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
		template <template<typename...> class TList1, typename TList2>
		struct contains_convertible_list<TList1<>, TList2>
		{
			static constexpr bool value = false;
		};
		template <template<typename...> class TList1, typename TList2, typename First>
		struct contains_convertible_list<TList1<First>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First>, "Error: contains_convertible_list expected TList2 and First to be of type t_list::type_list<Ts...>");
			static constexpr bool value = TList2::template is_convertible_list<First>();
		};
		template <template<typename...> class TList1, typename TList2, typename First, typename... Ts>
		struct contains_convertible_list<TList1<First, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First>, "Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");
			static constexpr bool value = conditional_bool_binary_v<TList2::template is_convertible_list<First>(), true, contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <template<typename...> class TList1, typename TList2,
			typename First, typename Second,
			typename... Ts>
		struct contains_convertible_list<TList1<First, Second, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First, Second>, "Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					TList2::template is_convertible_list<First>  () ||
					TList2::template is_convertible_list<Second> ()
				), true,
				typename contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <template<typename...> class TList1, typename TList2,
			typename First, typename Second, typename Third,
			typename... Ts>
		struct contains_convertible_list<TList1<First, Second, Third, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First, Second, Third>, "Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					TList2::template is_convertible_list<First>  () ||
					TList2::template is_convertible_list<Second> () ||
					TList2::template is_convertible_list<Third>  ()
				), true,
				contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <template<typename...> class TList1, typename TList2,
			typename First, typename Second, typename Third, typename Fourth,
			typename... Ts>
		struct contains_convertible_list<TList1<First, Second, Third, Fourth, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First, Second, Third, Fourth>, "Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					TList2::template is_convertible_list<First>  () ||
					TList2::template is_convertible_list<Second> () ||
					TList2::template is_convertible_list<Third>  () ||
					TList2::template is_convertible_list<Fourth> ()
				), true,
				contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <template<typename...> class TList1, typename TList2,
			typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			typename... Ts>
		struct contains_convertible_list<TList1<First, Second, Third, Fourth, Fifth, Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2, First, Second, Third, Fourth, Fifth>, "Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					TList2::template is_convertible_list<First>  () ||
					TList2::template is_convertible_list<Second> () ||
					TList2::template is_convertible_list<Third>  () ||
					TList2::template is_convertible_list<Fourth> () ||
					TList2::template is_convertible_list<Fifth>  ()
				), true,
				contains_convertible_list<TList1<Ts...>, TList2>::value>;
		};
		template <template<typename...> class TList1, typename TList2,
			typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
			typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			typename... Ts>
		struct contains_convertible_list<TList1<
			First, Second, Third, Fourth, Fifth,
			Sixth, Seventh, Eighth, Nineth, Tenth,
			Ts...>, TList2>
		{
			static_assert(is_template_of_type_v<type_list, TList2,
				First, Second, Third, Fourth, Fifth,
				Sixth, Seventh, Eighth, Nineth, Tenth>,
				"Error: contains_convertible_list expected TList2 and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					TList2::template is_convertible_list<First>   () ||
					TList2::template is_convertible_list<Second>  () ||
					TList2::template is_convertible_list<Third>   () ||
					TList2::template is_convertible_list<Fourth>  () ||
					TList2::template is_convertible_list<Fifth>   () ||
					TList2::template is_convertible_list<Sixth>   () ||
					TList2::template is_convertible_list<Seventh> () ||
					TList2::template is_convertible_list<Eighth>  () ||
					TList2::template is_convertible_list<Nineth>  () ||
					TList2::template is_convertible_list<Tenth>   ()
				), true,
				contains_convertible_list<TList1<Ts...>, TList2>::value>;
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
			static constexpr bool value = conditional_bool_binary_v<(n_types != 1), true, !detail::is_template_of_type_v<type_list, typename TList1::front>>;
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