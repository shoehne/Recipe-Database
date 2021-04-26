#pragma once
#ifndef RECIPEAPP_H
#define RECIPEAPP_H

#include "Events/SqlEvent.h"

#include <SQLiteCpp/SQLiteCpp.h>

namespace Recipe_Database {

	class RecipeApp {

	public:
		RecipeApp();
		~RecipeApp();

		void Init();
		void OnEvent(Event& event);
		void Run();

	private:
		// Events
		bool OnSqlDelete(SqlDeleteEvent& e);
		bool OnSqlInsert(SqlInsertEvent& e);
		bool OnSqlSelect(SqlSelectEvent& e);
		bool OnSqlUpdate(SqlUpdateEvent& e);
		bool OnWindowClose(WindowCloseEvent& event);

		// Handling of required SQLite queries.
		bool SqlDeleteQuery(std::string id,
			std::string recipe_name);
		bool SqlInsertQuery(Recipe recipe);
		bool SqlSelectQuery(SqlSelectFilter filter);
		bool SqlUpdateQuery(Recipe recipe_old,
			Recipe recipe_new);

		bool is_running = true;
		Ref<Window> main_window;
		// SQLite related variables
		// std::map<std::thread, > live_search_threads;
		std::vector<Recipe_Database::Recipe> recipes;
		std::mutex recipes_mutex;
		bool sql_database_connected = false;
		uint8_t temp_table_count = 0;
	};
}

#endif // !RECIPE_APP_H


