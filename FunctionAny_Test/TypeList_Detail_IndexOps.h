#ifndef TYPE_LIST_DETAIL_IO_H
#define TYPE_LIST_DETAIL_IO_H

#include <utility>
#include <type_traits>

namespace t_list
{
	namespace detail
	{
		template <typename... Ts>
		using type_list = t_list::type_list<Ts...>;

		template <std::size_t I, typename T>
		struct indexed 
		{
			using type = T;
		};

		template <typename Is, typename... Ts>
		struct indexer;

		template <std::size_t... Is, typename... Ts>
		struct indexer<std::index_sequence<Is...>, Ts...>
			: indexed<Is, Ts>...
		{ };

		template <std::size_t I, typename T>
		static indexed<I, T> select(indexed<I, T>);

		// extract_t extract type from Ts... given index I
		template <std::size_t I, typename... Ts>
		using extract_t = typename decltype(select<I>(
			indexer<std::index_sequence_for<Ts...>, Ts...>{}
		))::type;

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
				using type = typename erase_impl<i + 1, n, TTail...>::type::template append_front<T>;
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

		//inclusive bounds [Start, End]
		template <std::size_t Start, std::size_t End, typename... Ts>
		class slice
		{
			static constexpr std::size_t n_types = sizeof...(Ts);
			static_assert(Start <= End                  , "Start must be less than end");
			static_assert(Start >= 0u && Start < n_types, "Start out of bounds");
			static_assert(End >= 0u && End < n_types    , "End out of bounds");

			template <std::size_t i, std::size_t Start, std::size_t End, typename... TTail>
			struct slice_impl;

			template <std::size_t i, std::size_t Start, std::size_t End, typename T, typename... TTail>
			class slice_impl<i, Start, End, T, TTail...>
			{
				using type_h = typename slice_impl<i + 1, Start, End, TTail...>::type;
			public:
				using type = std::conditional_t<i >= Start, typename type_h::template append_front<T>, type_h>;
			};

			//slice of size 0 case
			template <std::size_t n, typename T, typename... TTail>
			struct slice_impl<n, n, n, T, TTail...>
			{
				using type = type_list<T>;
			};

			//slice of size 0 case
			template <std::size_t n, std::size_t Start, typename T, typename... TTail>
			struct slice_impl<n, Start, Start, T, TTail...>
			{
				using type = type_list<>;
			};

			//end case
			template <std::size_t n, std::size_t Start, typename T, typename... TTail>
			struct slice_impl<n, Start, n, T, TTail...>
			{
				using type = type_list<T>;
			};

		public:
			using type = std::conditional_t<Start == 0u && End == n_types - 1, t_list::type_list<Ts...>, typename slice_impl<0u, Start, End, Ts...>::type>;
		};
		template <std::size_t Start, std::size_t End, typename... Ts>
		using slice_t = typename slice<Start, End, Ts...>::type;

		// Create slice [0, n] (select first n types)
		template <std::size_t n, typename... Ts>
		using first_of_t = detail::slice_t<0u, n, Ts...>;

		template <std::size_t n, typename... Ts>
		class last_of {
			static constexpr std::size_t n_types = sizeof...(Ts);
		public:
			using type = detail::slice_t<n_types - n, n_types - 1, Ts...>;
		};

		template <typename... Ts>
		struct last_of<0u, Ts...> {
			using type = type_list<>;
		};

		// Create slice [n_types - n, n_types-1] (select last n types)
		template <std::size_t n, typename... Ts>
		using last_of_t = typename last_of<n, Ts...>::type;


		template <std::size_t idx, class TypeList> struct type_list_erase;
		template <std::size_t idx, template <class...> class TypeList, typename... Ts>
		struct type_list_erase<idx, TypeList<Ts...>>
		{
			using type = erase_t<idx, Ts...>;
		};
		template <class TypeList, std::size_t idx>
		using type_list_erase_t = typename type_list_erase<idx, TypeList>::type;
	}
}

#endif