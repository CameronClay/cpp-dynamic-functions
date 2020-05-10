#ifndef TYPE_LIST_HELPERS_H
#define TYPE_LIST_HELPERS_H

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

		template <bool...> struct bool_pack;
		template <bool... v>
		using          all_true   = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

		// all_true_v - used like std::conjunction except works on bools instead of predicates
		template <bool... v>
		constexpr bool all_true_v = all_true<v...>::value;

		// one_true_v - used like std::disjunction except works on bools instead of predicates
		template <bool... v>
		constexpr bool one_true_v = (v || ...);

		//// checks if type is template of another type
		template < template<class...> class TemplateOf, class Type>
		struct is_template_of
		{
			static constexpr bool value = false;
		};

		// is_template_of_type - Checks to see if First, Rest... are all types of the same template
		template <template<class...> class TemplateOf, class... Ts>
		struct is_template_of<TemplateOf, TemplateOf<Ts...>>
		{
			static constexpr bool value = true;
		};

		template <template<class...> class TemplateOf, class Type>
		constexpr bool is_template_of_v = is_template_of<TemplateOf, Type>::value;

		// is_template_of_type - Checks to see if First, Rest... are all types of the same template
		template <template<class...> class TemplateOf, class... Ts>
		struct all_templates_of
		{
			static constexpr bool value = false;
		};
		template <template<class...> class TemplateOf>
		struct all_templates_of<TemplateOf>
		{
			static constexpr bool value = true;
		};
		template <template<class...> class TemplateOf, template<class...> class First, class... Rest, class... FirstTs>
		struct all_templates_of<TemplateOf, First<FirstTs...>, Rest...>
		{
			static constexpr bool value = std::is_same_v<
				TemplateOf<FirstTs...>, First<FirstTs...>> &&
				all_templates_of<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			template<class...> class First, template<class...> class Second,
			class... Rest,
			class... FirstTs, class... SecondTs>
		struct all_templates_of<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				all_templates_of<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			template<class...> class First, template<class...> class Second, template<class...> class Third,
			class... Rest,
			class... FirstTs, class... SecondTs, class... ThirdTs>
		struct all_templates_of<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				all_templates_of<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			template<class...> class First, template<class...> class Second, template<class...> class Third, template<class...> class Fourth,
			class... Rest,
			class... FirstTs, class... SecondTs, class... ThirdTs, class... FourthTs>
		struct all_templates_of<TemplateOf, First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Fourth<FourthTs...>, Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				std::is_same_v<TemplateOf<FourthTs...>, Fourth<FourthTs...>> &&
				all_templates_of<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			template<class...> class First, template<class...> class Second, template<class...> class Third, template<class...> class Fourth, template<class...> class Fifth,
			class... Rest,
			class... FirstTs, class... SecondTs, class... ThirdTs, class... FourthTs, class... FifthTs>
		struct all_templates_of<TemplateOf,
			First<FirstTs...>, Second<SecondTs...>, Third<ThirdTs...>, Fourth<FourthTs...>, Fifth<FifthTs...>,
			Rest...>
		{
			static constexpr bool value =
				std::is_same_v<TemplateOf<FirstTs...>,  First<FirstTs...>>   &&
				std::is_same_v<TemplateOf<SecondTs...>, Second<SecondTs...>> &&
				std::is_same_v<TemplateOf<ThirdTs...>,  Third<ThirdTs...>>   &&
				std::is_same_v<TemplateOf<FourthTs...>, Fourth<FourthTs...>> &&
				std::is_same_v<TemplateOf<FifthTs...>,  Fifth<FifthTs...>>   &&
				all_templates_of<TemplateOf, Rest...>::value;
		};
		template <template<class...> class TemplateOf,
			template<class...> class First, template<class...> class Second,  template<class...> class Third,  template<class...> class Fourth, template<class...> class Fifth,
			template<class...> class Sixth, template<class...> class Seventh, template<class...> class Eighth, template<class...> class Nineth, template<class...> class Tenth,
			class... Rest,
			class... FirstTs, class... SecondTs,  class... ThirdTs,  class... FourthTs, class... FifthTs,
			class... SixthTs, class... SeventhTs, class... EighthTs, class... NinethTs, class... TenthTs>
		struct all_templates_of<TemplateOf,
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
				all_templates_of<TemplateOf, Rest...>::value;
		};

		template < template<class...> class TemplateOf, class... Ts>
		constexpr bool all_templates_of_v = all_templates_of<TemplateOf, Ts...>::value;

		// is_template_of_type - Checks to see if First, Rest... are all types of the same template
		template <class TypeTemplateOf, class... Ts>
		struct all_templates_of_type;
		template <template<class...> class TemplateOf, class... TemplateOfTs, class... Ts>
		struct all_templates_of_type<TemplateOf<TemplateOfTs...>, Ts...> : all_templates_of<TemplateOf, Ts...> {};
		template <class TypeTemplateOf, class... Ts>
		constexpr bool all_templates_of_type_v = all_templates_of_type<TypeTemplateOf, Ts...>::value;

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

		//template <typename TList1, typename TList2>
		//struct is_convertible_list;
		//template <typename TList1, template<typename...> class TList2, typename... Ts>
		//struct is_convertible_list<TList1, TList2<Ts...>>
		//{
		//	static_assert(all_templates_of_v<type_list, TList1>, "Error: is_not_type_list_overload expected TList1 to be of type t_list::type_list<Ts...>");
		//	static constexpr bool value = TList1::template is_convertible<Ts...>();
		//};
		//template <typename TList1, typename TList2>
		//constexpr bool is_convertible_list_v = is_convertible_list<TList1, TList2>::value;

		template <typename TTo, typename TFrom>
		struct is_convertible_list;
		template <template<typename...> class TTo, typename TFrom, typename... Ts>
		struct is_convertible_list<TTo<Ts...>, TFrom>
		{
			static_assert(all_templates_of_v<type_list, TFrom>, "Error: is_not_type_list_overload expected TList1 to be of type t_list::type_list<Ts...>");
			static constexpr bool value = TFrom::template is_convertible<Ts...>();
		};
		template <typename TTo, typename TFrom>
		constexpr bool is_convertible_list_v = is_convertible_list<TTo, TFrom>::value;

		template <typename ArgListTo, typename ArgsFrom>
		struct contains_convertible_list;
		template <template<typename...> class ArgListTo, typename ArgsFrom>
		struct contains_convertible_list<ArgListTo<>, ArgsFrom>
		{
			static constexpr bool value = false;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom, typename First>
		struct contains_convertible_list<ArgListTo<First>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First>, "Error: contains_convertible_list expected ArgsFrom and First to be of type t_list::type_list<Ts...>");
			static constexpr bool value = First::template is_convertible_list<ArgsFrom>();
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom, typename First, typename... Ts>
		struct contains_convertible_list<ArgListTo<First, Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First>, "Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");
			static constexpr bool value = conditional_bool_binary_v<First::template is_convertible_list<ArgsFrom>(), true, contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom,
			typename First, typename Second,
			typename... Ts>
		struct contains_convertible_list<ArgListTo<First, Second, Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First, Second>, "Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					First::template is_convertible_list<ArgsFrom>  () ||
					Second::template is_convertible_list<ArgsFrom> ()
				), true,
				contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom,
			typename First, typename Second, typename Third,
			typename... Ts>
		struct contains_convertible_list<ArgListTo<First, Second, Third, Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First, Second, Third>, "Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					First::template is_convertible_list<ArgsFrom>  () ||
					Second::template is_convertible_list<ArgsFrom> () ||
					Third::template is_convertible_list<ArgsFrom>  ()
				), true,
				contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom,
			typename First, typename Second, typename Third, typename Fourth,
			typename... Ts>
		struct contains_convertible_list<ArgListTo<First, Second, Third, Fourth, Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First, Second, Third, Fourth>, "Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					First::template is_convertible_list<ArgsFrom>  () ||
					Second::template is_convertible_list<ArgsFrom> () ||
					Third::template is_convertible_list<ArgsFrom>  () ||
					Fourth::template is_convertible_list<ArgsFrom> ()
				), true,
				contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom,
			typename First, typename Second, typename Third, typename Fourth, typename Fifth,
			typename... Ts>
		struct contains_convertible_list<ArgListTo<First, Second, Third, Fourth, Fifth, Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom, First, Second, Third, Fourth, Fifth>, "Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					First::template is_convertible_list<ArgsFrom>  () ||
					Second::template is_convertible_list<ArgsFrom> () ||
					Third::template is_convertible_list<ArgsFrom>  () ||
					Fourth::template is_convertible_list<ArgsFrom> () ||
					Fifth::template is_convertible_list<ArgsFrom>  ()
				), true,
				contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <template<typename...> class ArgListTo, typename ArgsFrom,
			typename First, typename Second,  typename Third,  typename Fourth, typename Fifth,
			typename Sixth, typename Seventh, typename Eighth, typename Nineth, typename Tenth,
			typename... Ts>
		struct contains_convertible_list<ArgListTo<
			First, Second, Third, Fourth, Fifth,
			Sixth, Seventh, Eighth, Nineth, Tenth,
			Ts...>, ArgsFrom>
		{
			static_assert(all_templates_of_v<type_list, ArgsFrom,
				First, Second, Third, Fourth, Fifth,
				Sixth, Seventh, Eighth, Nineth, Tenth>,
				"Error: contains_convertible_list expected ArgsFrom and First... to be of type t_list::type_list<Ts...>");

			static constexpr bool value = conditional_bool_binary_v<
				(
					First::template is_convertible_list<ArgsFrom>   () ||
					Second::template is_convertible_list<ArgsFrom>  () ||
					Third::template is_convertible_list<ArgsFrom>   () ||
					Fourth::template is_convertible_list<ArgsFrom>  () ||
					Fifth::template is_convertible_list<ArgsFrom>   () ||
					Sixth::template is_convertible_list<ArgsFrom>   () ||
					Seventh::template is_convertible_list<ArgsFrom> () ||
					Eighth::template is_convertible_list<ArgsFrom>  () ||
					Nineth::template is_convertible_list<ArgsFrom>  () ||
					Tenth::template is_convertible_list<ArgsFrom>   ()
				), true,
				contains_convertible_list<ArgListTo<Ts...>, ArgsFrom>::value>;
		};
		template <typename ArgListTo, typename ArgsFrom>
		constexpr bool contains_convertible_list_v = contains_convertible_list<ArgListTo, ArgsFrom>::value;

		template <typename TList1, typename TList2>
		struct is_not_type_list_overload;
		template <typename TList1, template<typename...> class TList2, typename... Ts>
		class is_not_type_list_overload<TList1, TList2<Ts...>>
		{
			static_assert(all_templates_of_v<type_list, TList1>, "Error: is_not_type_list_overload expected TList1 to be of type t_list::type_list<Ts...>");
			static constexpr bool n_types = sizeof...(Ts);
		public:
			static constexpr bool value = conditional_bool_binary_v<(n_types != 1), true, !detail::all_templates_of<type_list, typename TList1::front>>;
		};
		template <typename TList1, typename TList2>
		constexpr bool is_not_type_list_overload_v = is_not_type_list_overload<TList1, TList2>::value;

		template <typename TList1, typename TList2>
		struct is_equivalent
		{
			static_assert(all_templates_of_v<type_list, TList1, TList2>, "Error: is_equivalent expected TList1 and TList2 to be of type t_list::type_list<Ts...>");
			static constexpr bool value = conditional_bool_v<(TList1::n_types == TList2::n_types), TList1::template is_subset<TList2>>;
		};
		template <typename TList1, typename TList2>
		constexpr bool is_equivalent_v = is_equivalent<TList1, TList2>::value;
	}
}

#endif