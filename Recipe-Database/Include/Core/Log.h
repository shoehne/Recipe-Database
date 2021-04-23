#pragma once
#ifndef LOG_H
#define LOG_H

//------spdlog------
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Recipe_Database {

	class Log {

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; };

	private:
		static std::shared_ptr<spdlog::logger> logger;
	};

	// Log macros
#define RECIPE_DATABASE_TRACE(...)			::Recipe_Database::Log::GetLogger()->trace(__VA_ARGS__)
#define RECIPE_DATABASE_INFO(...)			::Recipe_Database::Log::GetLogger()->info(__VA_ARGS__)
#define RECIPE_DATABASE_WARN(...)			::Recipe_Database::Log::GetLogger()->warn(__VA_ARGS__)
#define RECIPE_DATABASE_ERROR(...)			::Recipe_Database::Log::GetLogger()->error(__VA_ARGS__)
#define RECIPE_DATABASE_FATAL(...)			::Recipe_Database::Log::GetLogger()->fatal(__VA_ARGS__)
}

#endif // !LOG_H

