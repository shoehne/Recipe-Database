#pragma once
#ifndef SQL_H
#define SQL_H

#include "Core/Core.h"
#include "Core/Recipe.h"
#include "Events/SqlEvent.h"

// SQLiteCpp
#include <SQLiteCpp/SQLiteCpp.h>

namespace Recipe_Database {

	struct SelectFilters {

		std::string name;
		// Add a "nationality" to the filter with the option to exclude a 
		// specific "nationality" from the results
		std::string nationality;
		bool is_nationality = true;
		// Add ingredients selected by type to the filter with the option to exclude 
		// specific ingredients. Up to 3 types can be added.
		std::vector<std::string> type;
		std::vector<bool> is_type;
		// Add specific ingredients by name to the filter giving the option 
		// to exclude specific ingredients.
		// Up to 6 different ingredients can be added.
		std::vector<std::string> ingredient;
		std::vector<bool> is_ingredient;
	};

	class SqlDatabase {

		using EventCallbackFn = std::function<void(Event&)>;

	public:
		SqlDatabase(std::string name);
		~SqlDatabase();

		void SetEventCallback(const EventCallbackFn& callback);

		// SQLite queries
		bool DeleteQuery(std::string id);
		bool InsertQuery(Recipe recipe);
		bool SelectQuery(SelectFilters filter);
		
	private:
		SQLite::Database database;
		EventCallbackFn event_callback;
		uint8_t temp_table_count = 0;
	};
}
#endif // !SQLDATABASE_H

