#pragma once
#ifndef CORE_H
#define CORE_H

#include <memory>

#define BIT(x) (1 << x)

#define BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Recipe_Database {

	enum class Currency {

		NA = 0,
		AUD,
		CAD,
		EURO,
		GBP,
		HKD,
		JPY,
		USD
	};

	Currency CurrencyConversion(Currency currency) {

		return currency;
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {

		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)	{

		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif // !CORE_H

