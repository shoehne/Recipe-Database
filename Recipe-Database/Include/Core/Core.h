#pragma once
#ifndef CORE_H
#define CORE_H

#include <memory>
#ifdef PLATFORM_WINDOWS
#include <rpc.h>
#elif PLATFORM_LINUX
#include <uuid/uuid.h>
#endif // PLATFORM_WINDOWS

// SQLiteCpp
#include <SQLiteCpp/SQLiteCpp.h>

#include "Core/Log.h"

#define BIT(x) (1 << x)

#define BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Recipe_Database {

	// This function should probably run in a separate thread because
	// the look-up times might be pretty long with larger databases.
	// There's no reason to let the user edit the recipe and check once
	// the user wants to save the recipe if a uuid has been generated or not.
	bool CreateUuid(std::string uuid_str) {

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

		// Check whether the created ID is for some reason already 
		// stored in the database.
		try {

			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

			SQLite::Statement query(db, "SELECT recipe_id FROM recipes WHERE recipe_id = ?;");
			query.bind(1, uuid_str);

			if (query.executeStep()) {

				RECIPE_DATABASE_WARN("WARNING: The generated ID already exists!");
				uuid_str.clear();

				return false;
			}
		}
		catch (std::exception& e) {

			RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", e.what());

			return false;
		}

		return true;
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

