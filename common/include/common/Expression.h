#pragma once

#include "spvgentwo/stdreplacement.h"

namespace spvgentwo
{	
	template <class Func>
	class Expression
	{
		Func m_func;
		bool ran = false;
	public:
		Expression(const Func& _func) : m_func(_func) {}

		//template <class ...Args>
		//Expression(Args&& ... _args) : m_func{ stdrep::forward<Args>(_args)... } {}

		Func& get() { return m_func; }
		const Func& get() const { return m_func; }

		bool evaluated() const { return ran; }

		template <class ...Args>
		auto operator()(Args&& ... _args)
		{ 
			ran = true;
			return m_func(stdrep::forward<Args>(_args)...);
		}
	};

	template <class ...Args>
	auto make_expr(Args&& ... _args) { return Expression(stdrep::forward<Args>(_args)...); }

	template <class ...Args>
	using expr_type = decltype(make_expr(stdrep::declval<Args>()...));
} // !spvgentwo