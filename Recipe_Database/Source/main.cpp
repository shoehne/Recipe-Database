//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "RecipeApp.h"

#ifdef PLATFORM_WINDOWS
#pragma comment(linker, "/ENTRY:mainCRTStartup")
#endif // PLATFORM_WINDOWS


int main(int argc, char** argv) {

	Recipe_Database::RecipeApp* recipe_app = new Recipe_Database::RecipeApp();

	recipe_app->Init();

	recipe_app->Run();
}