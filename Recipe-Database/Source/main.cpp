//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "RecipeApp.h"

#ifdef PLATFORM_WINDOWS
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif // PLATFORM_WINDOWS


int main(int argc, char** argv) {

	#ifdef DEBUG && PLATFORM_WINDOWS
	AllocConsole();
	freopen("CONOUT$",
		"w+",
		stdout);
	freopen("CONOUT$",
		"w+",
		stderr);
	#elif DEBUG && PLATFORM_LINUX
	
	#endif // DEBUG && PLATFORM_WINDOWS

	Recipe_Database::RecipeApp* recipe_app = new Recipe_Database::RecipeApp();

	recipe_app->Init();

	recipe_app->Run();
}