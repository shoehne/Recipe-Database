#pragma once
#ifndef RECIPEAPP_H
#define RECIPEAPP_H

namespace Recipe_Database {

	class RecipeApp {

	public:
		RecipeApp();
		~RecipeApp();

		void Init();
		void OnEvent(Event& event);
		void Run();

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		bool is_running = true;
		Ref<Window> main_window;
	};
}

#endif // !RECIPE_APP_H


