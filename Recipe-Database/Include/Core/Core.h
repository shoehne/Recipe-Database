#pragma once
#ifndef CORE_H
#define CORE_H

#include "Core/Log.h"

#include <memory>
#ifdef PLATFORM_WINDOWS
#include <rpc.h>
#elif PLATFORM_LINUX
#include <uuid/uuid.h>
#endif // PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#define BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Recipe_Database {

	inline void CreateUuid(std::string uuid_str) {

		uuid_str.clear();
		uuid_str.append("#REC");
		#ifdef PLATFORM_WINDOWS
		UUID uuid;
		UuidCreate(&uuid);

		unsigned char* buffer;
		UuidToStringA(&uuid,
			&buffer);
		uuid_str.append((char*)buffer);

		RpcStringFreeA(&buffer);
		#elif PLATFORM_LINUX
		uuid_t uuid;
		uuid_generate(uuid);

		char buffer[37];
		uuid_unparse(uuid,
			buffer);

		uuid_str.append(buffer);
		#else
		RECIPE_DATABASE_ERROR("ERROR: The chosen platform is not supported!");
		return false;
		#endif // PLATFORM_WINDOWS
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

	// Convert a std::string to std::wstring for some Win32 API requirements.
	#ifdef PLATFORM_WINDOWS
	inline std::wstring ToWstring(std::string string) {

		int str_len = (int)string.length() + 1;
		int len = MultiByteToWideChar(CP_ACP,
			0,
			string.c_str(),
			str_len,
			0,
			0);
		wchar_t* buffer = new wchar_t[len];
		MultiByteToWideChar(CP_ACP,
			0,
			string.c_str(),
			str_len,
			buffer,
			len);

		return buffer;
	}
	#endif // PLATFORM_WINDOWS


}

#endif // !CORE_H

