//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "RecipeApp.h"

Recipe_Database::RecipeApp::RecipeApp() {

}

Recipe_Database::RecipeApp::~RecipeApp() {

}

void Recipe_Database::RecipeApp::Init() {

	Recipe_Database::Log::Init();
	#ifdef DEBUG
	RECIPE_DATABASE_TRACE("TRACE: Initialised Log.");
	#endif // DEBUG

	// Create a window with the default size and position.
	// Use "Recipe Database" as the title for the window.
	// TO DO: Consider supporting different languages and using
	// language appropriate titles.
	std::string name = "Recipe Database";

	main_window = Window::Create(WindowProps(name));
	main_window->SetEventCallback(BIND_EVENT(RecipeApp::OnEvent));

	// Create the necessary tables in the database if they do 
	// not already exist.
	try {
	
		SQLite::Database db("./data/recipe_database.db3",
			SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

		std::string query_str;
		query_str.clear();
		query_str.append("CREATE VIRTUAL TABLE IF NOT EXISTS recipes USING FTS5");
		query_str.append("(recipe_id, recipe_name,  ");
		query_str.append("course, servings, nationality, instructions, picture);");
		SQLite::Statement recipe_query(db, 
			query_str);
		int i = recipe_query.exec();
		RECIPE_DATABASE_INFO("{0}", i);

		query_str.clear();
		query_str.append("CREATE VIRTUAL TABLE IF NOT EXISTS ingredients USING FTS5");
		query_str.append("(recipe_id, ingredient_name, ");
		query_str.append("ingredient_type, unit, amount, currency, price);");
		SQLite::Statement ingredients_query(db, 
			query_str);
		ingredients_query.exec();

		query_str.clear();
		query_str.append("CREATE VIRTUAL TABLE IF NOT EXISTS dish_types USING FTS5");
		query_str.append("(recipe_id, dish_type);");
		SQLite::Statement type_query(db, 
			query_str);
		type_query.exec();
		

		#ifdef DEBUG
		//SQLite::Statement query(db, "SELECT recipe_id FROM recipes;");
		SQLite::Statement query(db, "SELECT recipe_id FROM recipes WHERE recipe_id = ?");
		int id = 5;
		query.bind(1 , id);

		while (query.executeStep()) {

			int t = query.getColumnCount();
			int test = query.getColumn(0);
			bool b = query.hasRow();
			if (test == id) {

				RECIPE_DATABASE_TRACE("Found object. Object ID: {0}", test);
			}
			RECIPE_DATABASE_TRACE("Object ID: {0}\nColumn count: {1}\nAdditional rows: {2}", test, t, b);
			//query.reset();
		}
		
		// For debugging/testing purposes fill the tables with entries.
		query_str.clear();
		query_str.append("INSERT INTO recipes ");
		query_str.append("(recipe_id, recipe_name, course, servings, nationality, instructions, picture) ");
		query_str.append("VALUES ('1', 'name1', 'course1', '10', 'nationality1', 'instructions', 'picture');");
		SQLite::Statement query1(db, query_str);
		query1.exec();
		query_str.clear();
		query_str.append("INSERT INTO recipes ");
		query_str.append("(recipe_id, recipe_name, course, servings, nationality, instructions, picture) ");
		query_str.append("VALUES ('2', 'name2', 'course2', '10', 'nationality2', 'instructions', 'picture');");
		SQLite::Statement query2(db, query_str);
		query2.exec();
		query_str.clear();
		query_str.append("INSERT INTO recipes ");
		query_str.append("(recipe_id, recipe_name, course, servings, nationality, instructions, picture) ");
		query_str.append("VALUES ('3', 'name3', 'course2', '10', 'nationality2', 'instructions', 'picture');");
		SQLite::Statement query3(db, query_str);
		query3.exec();
		query_str.clear();
		query_str.append("INSERT INTO ingredients ");
		query_str.append("(recipe_id, ingredient_name, ingredient_type, unit, amount, currency, price) ");
		query_str.append("VALUES ('1', 'ingredient1', 'type1', 'unit1', '1', 'currency1', '10');");
		SQLite::Statement query4(db, query_str);
		query4.exec();
		query_str.clear();
		query_str.append("INSERT INTO ingredients ");
		query_str.append("(recipe_id, ingredient_name, ingredient_type, unit, amount, currency, price) ");
		query_str.append("VALUES ('1', 'ingredient2', 'type2', 'unit1', '1', 'currency1', '10');");
		SQLite::Statement query5(db, query_str);
		query5.exec();
		query_str.clear();
		query_str.append("INSERT INTO ingredients ");
		query_str.append("(recipe_id, ingredient_name, ingredient_type, unit, amount, currency, price) ");
		query_str.append("VALUES ('2', 'ingredient1', 'type1', 'unit1', '1', 'currency1', '10');");
		SQLite::Statement query6(db, query_str);
		query6.exec();
		query_str.clear();
		query_str.append("INSERT INTO ingredients ");
		query_str.append("(recipe_id, ingredient_name, ingredient_type, unit, amount, currency, price) ");
		query_str.append("VALUES ('2', 'ingredient3', 'type4', 'unit1', '1', 'currency1', '10');");
		SQLite::Statement query7(db, query_str);
		query7.exec();
		query_str.clear();
		query_str.append("INSERT INTO ingredients ");
		query_str.append("(recipe_id, ingredient_name, ingredient_type, unit, amount, currency, price) ");
		query_str.append("VALUES ('3', 'ingredient4', 'type3', 'unit1', '1', 'currency1', '10');");
		SQLite::Statement query8(db, query_str);
		query8.exec();
		query_str.clear();
		query_str.append("INSERT INTO dish_types ");
		query_str.append("(recipe_id, dish_type) ");
		query_str.append("VALUES ('1', 'dtype1');");
		SQLite::Statement query9(db, query_str);
		query9.exec();
		query_str.clear();
		query_str.append("INSERT INTO dish_types ");
		query_str.append("(recipe_id, dish_type) ");
		query_str.append("VALUES ('1', 'dtype2');");
		SQLite::Statement query10(db, query_str);
		query10.exec();
		query_str.clear();
		query_str.append("INSERT INTO dish_types ");
		query_str.append("(recipe_id, dish_type) ");
		query_str.append("VALUES ('2', 'dtype2');");
		SQLite::Statement query11(db, query_str);
		query11.exec();
		query_str.clear();
		query_str.append("INSERT INTO dish_types ");
		query_str.append("(recipe_id, dish_type) ");
		query_str.append("VALUES ('2', 'dtype3');");
		SQLite::Statement query12(db, query_str);
		query12.exec();
		SQLite::Statement query13(db, "SELECT * FROM recipes;");
		while (query13.executeStep()) {
			RECIPE_DATABASE_TRACE("{0}", query13.getColumn(0).getString());
		}
		#endif // DEBUG

		RECIPE_DATABASE_INFO("INFO: Initialised database!");
	}
	catch (std::exception& e) {

		RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", 
			e.what());
	}
}

void Recipe_Database::RecipeApp::OnEvent(Event& event) {

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(RecipeApp::OnWindowClose));
}

void Recipe_Database::RecipeApp::PushLayer(Layer* layer) {

	layer_stack.PushLayer(layer);
}

void Recipe_Database::RecipeApp::PushOverlay(Layer* overlay) {

	layer_stack.PushOverlay(overlay);
}

void Recipe_Database::RecipeApp::Run() {

	while (is_running) {

		main_window->OnUpdate();
	}
}

bool Recipe_Database::RecipeApp::OnSqlDelete(SqlDeleteEvent& e) {

	SqlDeleteQuery* query = new SqlDeleteQuery(e.GetID(),
		e.GetName());
	sql_query_stack.PushQuery(query);

	return true;
}

bool Recipe_Database::RecipeApp::OnSqlInsert(SqlInsertEvent& e) {

	SqlInsertQuery* query = new SqlInsertQuery(e.GetRecipe());
	sql_query_stack.PushQuery(query);

	return true;
}

bool Recipe_Database::RecipeApp::OnSqlSelect(SqlSelectEvent& e) {

	SqlSelectQuery* query = new SqlSelectQuery(recipes,
		e.GetFilter());
	sql_query_stack.PushQuery(query);

	return true;
}

bool Recipe_Database::RecipeApp::OnSqlUpdate(SqlUpdateEvent& e) {

	SqlUpdateQuery* query = new SqlUpdateQuery(e.GetRecipeOld(),
		e.GetRecipeNew());
	sql_query_stack.PushQuery(query);

	return true;
}

bool Recipe_Database::RecipeApp::OnWindowClose(WindowCloseEvent& event) {

	is_running = false;
	return true;
}