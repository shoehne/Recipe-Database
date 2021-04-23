//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Core/Log.h"

//------spdlog------
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


namespace Recipe_Database {

	std::shared_ptr<spdlog::logger> Log::logger;

	void Log::Init() {

		spdlog::set_pattern("%^[%Y/%m/%d %T] %n: %v%$");
		logger = spdlog::stdout_color_mt("RECIPE_DATABASE");
		logger->set_level(spdlog::level::trace);
	}
}