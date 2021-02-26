//------Precompiled header------
#include "Recipe_Database_Pch.h"

#include "Recipe_App.h"

int main(int argc, char** argv) {

	Recipe_Database::RecipeApp* recipe_app = new Recipe_Database::RecipeApp();

	recipe_app->Run();
}