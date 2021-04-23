//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "RecipeApp.h"

int main(int argc, char** argv) {

	Recipe_Database::RecipeApp* recipe_app = new Recipe_Database::RecipeApp();

	recipe_app->Run();
}