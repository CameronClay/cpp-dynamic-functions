#pragma once
#include <utility>

namespace t_list
{
	template <typename... Ts> struct type_list;

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