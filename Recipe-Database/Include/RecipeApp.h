#pragma once
#ifndef RECIPEAPP_H
#define RECIPEAPP_H

#include "Core/Core.h"
#include "Core/Sql.h"
#include "Core/LayerStack.h"
#include "Events/SqlEvent.h"

namespace Recipe_Database {

	class RecipeApp {

	public:
		RecipeApp();
		~RecipeApp();

		void Init();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void Run();

	private:
		// Events
		bool OnSqlDelete(SqlDeleteEvent& e);
		bool OnSqlInsert(SqlInsertEvent& e);
		bool OnSqlSelect(SqlSelectEvent& e);
		bool OnSqlUpdate(SqlUpdateEvent& e);
		bool OnWindowClose(WindowCloseEvent& event);

		bool is_running = true;
		LayerStack layer_stack;
		Ref<Window> main_window;
		Ref<std::vector<Recipe_Database::Recipe>> recipes;
		SqlQueryStack sql_query_stack;
	};
}

#endif // !RECIPE_APP_H


