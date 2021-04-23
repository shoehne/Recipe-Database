//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "RecipeApp.h"

Recipe_Database::RecipeApp::RecipeApp() {

}

Recipe_Database::RecipeApp::~RecipeApp() {

}

void Recipe_Database::RecipeApp::Init() {

	Recipe_Database::Log::Init();
	RECIPE_DATABASE_TRACE("Initialised Log.");

	// Create a window with the default size and position.
	// Use "Recipe Database" as the title for the window.
	// TO DO: Consider supporting different languages and using
	// language appropriate titles.
	std::string name = "Recipe Database";

	main_window = Window::Create(WindowProps(name));
	main_window->SetEventCallback(BIND_EVENT(RecipeApp::OnEvent));
}

void Recipe_Database::RecipeApp::OnEvent(Event& event) {

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(RecipeApp::OnWindowClose));
}

void Recipe_Database::RecipeApp::Run() {

	#ifdef PLATFORM_WINDOWS
	
	#endif // DEBUG

	while (is_running) {

		main_window->OnUpdate();
	}
}

bool Recipe_Database::RecipeApp::OnWindowClose(WindowCloseEvent& event) {

	is_running = false;
	return true;
}