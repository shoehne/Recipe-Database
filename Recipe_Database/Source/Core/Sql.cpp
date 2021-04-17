//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Core/Sql.h"

Recipe_Database::SqlDatabase::SqlDatabase(std::string name) {

	database = SQLite::Database("./data/recipe_database.db3",
		SQLite::OPEN_CREATE);
}

Recipe_Database::SqlDatabase::~SqlDatabase() {

	database.~Database();
}

void Recipe_Database::SqlDatabase::SetEventCallback(const EventCallbackFn& callback) {

	event_callback = callback;
}

bool Recipe_Database::SqlDatabase::DeleteQuery(std::string id) {

	return true;
}

bool Recipe_Database::SqlDatabase::InsertQuery(Recipe recipe) {

	return true;
}

bool Recipe_Database::SqlDatabase::SelectQuery(SelectFilters filter) {

	return true;
}