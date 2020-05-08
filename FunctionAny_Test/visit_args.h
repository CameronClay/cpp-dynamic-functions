#pragma once
#include <cstddef>
#include <array>
#include <variant>
#include <memory>
#include "FunctionTraits.h"
#include "Function.h"

template<typename... Ts>
class visit_helper
{
	using list = t_list::type_list<Ts...>;
	using Func = f_traits::PFunc<void, void>;

	template<typename T>
	struct lookup
	{
		static decltype(auto) call(void* p) { otherFunction(static_cast<T*>(p)); }
	};

	template <typename Visitor>
	constexpr visit_helper(Visitor&& vis)
	{
		for (std::size_t i{ 0 }; i < list::n_types; ++i)
		{
			lookup_table[i] = [vis = std::forward<Visitor>(vis)]()
			{
				return lookup<typename list::template extract<i>>::call(vis);
			}
		}
	}

	using Func = f_traits::PFunc<void, void>;
	template<typename T>


	std::array<Func, sizeof...(Ts)> lookup_table;
};
