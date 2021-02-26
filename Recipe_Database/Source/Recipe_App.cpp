//------Precompiled header------
#include "Recipe_Database_Pch.h"

#include "Recipe_App.h"
#include "Core/Log.h"

Recipe_Database::RecipeApp::RecipeApp() {

	Init();
}

Recipe_Database::RecipeApp::~RecipeApp() {


}

void Recipe_Database::RecipeApp::Init() {

	Recipe_Database::Log::Init();
	RECIPE_DATABASE_TRACE("Initialised Log.");
}

void Recipe_Database::RecipeApp::Run() {

	while (true);
}