#pragma once
#include "TypeList_Detail.h"

namespace t_list
{
	// type_list - Represents a compile time list of types
	// All type_list operations that work on the type_list give a new type_list and do not modify the current
	template <typename... Ts> 
	struct type_list
	{
		// Rebind Ts... to another template in the form of TTo<Ts...>
		template <template<class...> class TTo>
		using rebind              = t_list::detail::rebind_t<type_list<Ts...>, TTo>;
		// Apply Ts... to one or more templates in the form of TTo_First<TTo_Rest<Ts>>...>
		template <template<class...> class TTo_First, template<class...> class... TTo_Rest>
		using apply               = t_list::detail::apply_t<type_list<Ts...>, TTo_First, TTo_Rest...>;

		// Remove duplicates from list
		using unique              = t_list::detail::type_list_unique<Ts...>;

		// Acess first type in list
		using front               = t_list::detail::front_t<type_list<Ts...>>;
		template <typename... Args>
		// Add Args to front of list
		using append_front = type_list<Args..., Ts...>;
		// Add Args to front of list and remove all duplicates
		template <typename... Args>
		using append_front_unique = t_list::detail::type_list_unique<Args..., Ts...>;
		// Remove first element in list
		using pop_front           = t_list::detail::pop_front_t<type_list<Ts...>>;

		// Append Args to end of list
		template <typename... Args>
		using append              = type_list<Ts..., Args...>;
		// Append Args to end of list and remove all duplicates
		template <typename... Args>
		using append_unique       = t_list::detail::type_list_unique<Ts..., Args...>;
		// Append all TLists to current list of types
		template <class... TLists>
		using append_lists        = t_list::detail::type_list_cat_t<type_list<Ts...>, TLists...>;
		// Append all TLists to current list of types and remove all duplicates
		template <class... TLists>
		using append_lists_unique = t_list::detail::type_list_unique<type_list<Ts...>, TLists...>;
		//Remove all elements from list
		using clear               = type_list<>;

		// Smallest type in list
		using smallest_type       = t_list::detail::smallest_type_t<Ts...>;
		// Largest type in list
		using largest_type        = t_list::detail::largest_type_t<Ts...>;
		
		// Remove all elements where Predicate::value is false
		template <template <typename> class Predicate>
		using filter              = t_list::detail::type_list_filter_t<Predicate, Ts...>;
		// Computes cross product with another type_list
		template <class TypeList>
		using cartesian_product   = t_list::detail::cartesian_product_t<type_list<Ts...>, TypeList>;
		// Extract type at idx
		template <std::size_t idx>
		using extract             = t_list::detail::type_list_extract_t<type_list<Ts...>, idx>;

		// Number of types in list
		static constexpr std::size_t n_types      = sizeof... (Ts);
		// True if list is empty
		static constexpr bool        empty        = n_types == 0;
		// True if there are no duplicate types in list
		static constexpr bool        is_unique    = t_list::detail::is_unique_v<Ts...>;
		// True if all Ts in list are storable types
		static constexpr bool        all_storable = std::conjunction_v<t_list::detail::is_storable<Ts>...>;

		// True if Ts contains T
		template<typename T>
		static constexpr bool        contains     = t_list::detail::contains_v<T, Ts...>;

		// True if all Ts are same as all Args
		template<typename... Args>
		static constexpr bool        same         = std::is_same_v<type_list<Args...>, type_list<Ts...>>;

		// Returns true if all Ts are convertible to Args
		template <typename... Args>
		static constexpr bool        convertible()
		{
			if constexpr (sizeof... (Args) == n_types)
				return std::conjunction_v<std::is_convertible<std::decay_t<Ts>, std::decay_t<Args>>...>;

			return false;
		}
	};
}