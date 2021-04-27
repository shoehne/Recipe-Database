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

	//main_window = Window::Create(WindowProps(name));
	//main_window->SetEventCallback(BIND_EVENT(RecipeApp::OnEvent));

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

void Recipe_Database::RecipeApp::Run() {

	while (is_running) {

		//main_window->OnUpdate();
	}
}

// Events
// Execute in a separate thread to not block the main application.
bool Recipe_Database::RecipeApp::OnSqlDelete(SqlDeleteEvent& e) {

	return true;
}

// Execute in a separate thread to not block the main application.
bool Recipe_Database::RecipeApp::OnSqlInsert(SqlInsertEvent& e) {

	//std::thread thread();

	return true;
}

bool Recipe_Database::RecipeApp::OnSqlSelect(SqlSelectEvent& e) {

	return true;
}

// Execute in a separate thread to not block the main application.
bool Recipe_Database::RecipeApp::OnSqlUpdate(SqlUpdateEvent& e) {

	return true;
}

bool Recipe_Database::RecipeApp::OnWindowClose(WindowCloseEvent& event) {

	is_running = false;
	return true;
}

bool Recipe_Database::RecipeApp::SqlDeleteQuery(std::string recipe_id,
	std::string recipe_name) {

	// Check if the database is currently connected on another thread
	// to avoid any consistency issues due to the missing implementation
	// of mutexes in the SQLiteCpp library.
	while (sql_database_connected == true) {

		// Have the thread wait with its execution for 1 second;
		// then check again.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
	const std::lock_guard<std::mutex> lock(recipes_mutex);

	try {

		SQLite::Database db("./data/recipe_database.db3", 
			SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

		// Set sql_database_connected to true so other SQL queries cannot be executed while
		// this thread is active.
		sql_database_connected = true;

		SQLite::Statement recipe_query(db, 
			"DELETE FROM recipes WHERE recipe_id=?;");
		recipe_query.bind(1, recipe_id);
		recipe_query.exec();

		SQLite::Statement type_query(db, 
			"DELETE FROM dish_types WHERE recipe_id=?;");
		type_query.bind(1, recipe_id);
		type_query.exec();

		SQLite::Statement ingredients_query(db, 
			"DELETE FROM ingredients WHERE recipe_id=?;");
		ingredients_query.bind(1, recipe_id);
		ingredients_query.exec();
	}
	catch (std::exception& e) {

		RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", 
			e.what());

		return false;
	}

	RECIPE_DATABASE_INFO("INFO: Deleted recipe {0}",
		recipe_name);

	// Set sql_database_connected to false so that other threads that may be waiting to 
	// execute can connect to the database.
	sql_database_connected = false;

	return true;
}

bool Recipe_Database::RecipeApp::SqlInsertQuery(Recipe recipe) {

	// Check if the database is currently connected on another thread
	// to avoid any consistency issues due to the missing implementation
	// of mutexes in the SQLiteCpp library.
	while (sql_database_connected == true) {

		// Have the thread wait with its execution for 1 second;
		// then check again.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	const std::lock_guard<std::mutex> lock(recipes_mutex);

	// If DEBUG is defined run a timer to see if the query completes in an 
	// acceptable time frame with larger databases.

	try {

		SQLite::Database db("./data/recipe_database.db3",
			SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

		// Set sql_database_connected to true so other SQL queries cannot be executed while
		// this thread is active.
		sql_database_connected = true;

		std::string query_str;
		query_str.clear();
		query_str.append("INSERT INTO recipes(recipe_id, recipe_name, course, servings, nationality, ");
		query_str.append("instructions, picture)");
		query_str.append(" VALUES(?, ?, ?, ?, ?, ?, ?);");
		SQLite::Statement recipe_query(db, 
			query_str);
		recipe_query.bind(1, recipe.id);
		recipe_query.bind(2, recipe.name);
		recipe_query.bind(3, recipe.course);
		recipe_query.bind(4, std::to_string(recipe.servings));
		recipe_query.bind(5, recipe.nationality);
		recipe_query.bind(6, recipe.instructions);
		// Concatenate the filepaths of all pictures for the recipe into one string.
		std::string picture_str;
		for (int i = 0; i < recipe.pictures.size(); i++) {

			if (i < recipe.pictures.size() - 1) {

				picture_str.append(recipe.pictures[i]);
				picture_str.append(",");
			}
			if (i == recipe.pictures.size() - 1) {

				picture_str.append(recipe.pictures[i]);
			}
		}
		recipe_query.bind(7, picture_str);
		recipe_query.exec();

		query_str.clear();
		query_str.append("INSERT INTO dish_types(recipe_id, dish_type)");
		query_str.append(" VALUES(?, ?);");
		for (int i = 0; i < recipe.dish_type.size(); i++) {

			SQLite::Statement type_query(db, 
				query_str);
			type_query.bind(1, recipe.id);
			type_query.bind(2, recipe.dish_type[i]);
			type_query.exec();
		}

		query_str.clear();
		query_str.append("INSERT INTO ingredients(recipe_id, ingredient_name, ingredient_type");
		query_str.append(", unit, amount, currency, price) ");
		query_str.append("VALUES (?, ?, ?, ?, ?, ?, ?);");
		for (int i = 0; i < recipe.ingredients.size(); i++) {

			SQLite::Statement ingredient_query(db, 
				query_str);
			ingredient_query.bind(1, recipe.id);
			ingredient_query.bind(2, recipe.ingredients[i].name);
			ingredient_query.bind(3, recipe.ingredients[i].type);
			ingredient_query.bind(4, std::to_string(static_cast<int>(recipe.ingredients[i].unit)));
			ingredient_query.bind(5, std::to_string(recipe.ingredients[i].amount));
			ingredient_query.bind(6, std::to_string(static_cast<int>(recipe.ingredients[i].currency)));
			ingredient_query.bind(7, std::to_string(recipe.ingredients[i].price));
			ingredient_query.exec();
		}
	}
	catch (std::exception& e) {

		RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", 
			e.what());

		return false;
	}

	// Set sql_database_connected to false so that other threads that may be waiting to 
	// execute can connect to the database.
	sql_database_connected = false;

	return true;
}

bool Recipe_Database::RecipeApp::SqlSelectQuery(SqlSelectFilter filter) {

	// Check if the database is currently connected on another thread
	// to avoid any consistency issues due to the missing implementation
	// of mutexes in the SQLiteCpp library.
	while (sql_database_connected == true) {

		// Have the thread wait with its execution for 1 second;
		// then check again.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	const std::lock_guard<std::mutex> lock(recipes_mutex);

	try {

		SQLite::Database db("./data/recipe_database.db3",
			SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
		
		// Set sql_database_connected to true so other SQL queries cannot be executed while
		// this thread is active.
		sql_database_connected = true;

		bool and_needed = false;
		int bind_count = 1;
		std::string query_str;
		
		// Only include items to be "included" - where the boolean in filter is set to true - in a SELECT
		// query. The false values will be sorted out in separate queries.
		query_str.clear();
		query_str.append("SELECT recipes.recipe_id, recipes.recipe_name, recipes.course, ");
		query_str.append("recipes.servings, recipes.nationality, recipes.instructions, recipes.picture ");
		query_str.append("FROM recipes ");
		query_str.append("INNER JOIN dish_types ON dish_types.recipe_id=recipes.recipe_id ");
		query_str.append("INNER JOIN ingredients ON ingredients.recipe_id=recipes.recipe_id ");
		query_str.append("WHERE ");
		if (!filter.recipe_name.empty()) {

			query_str.append("recipes.recipe_name=? ");
			and_needed = true;
		}
		if (!filter.nationality.empty()) {

			if (and_needed) {

				query_str.append("AND ");
				and_needed = false;
			}
			if (filter.is_nationality) {

				query_str.append("recipes.nationality=? ");
				and_needed = true;
			}
		}
		if (!filter.dish_types.empty()) {

			for (auto it = filter.dish_types.begin(); it != filter.dish_types.end(); ++it) {
			
				if (and_needed) {

					query_str.append("AND ");
					and_needed = false;
				}
				if (it->second == true) {

					query_str.append("dish_types.dish_type=? ");
					and_needed = true;
				}
			}
		}
		if (!filter.ingredient_types.empty()) {

			for (auto it = filter.ingredient_types.begin(); it != filter.ingredient_types.end(); ++it) {

				if (and_needed) {

					query_str.append("AND ");
					and_needed = false;
				}
				if (it->second) {

					query_str.append("ingredients.ingredient_type=? ");
					and_needed = true;
				}
			}
		}
		if (!filter.ingredients.empty()) {

			for (auto it = filter.ingredients.begin(); it != filter.ingredients.end(); ++it) {

				if (and_needed) {

					query_str.append("AND ");
					and_needed = false;
				}
				if (it->second == true) {

					query_str.append("ingredients.ingredient_name=? ");
					and_needed = true;
				}
			}
		}
		query_str.append("ORDER BY recipes.recipe_id;");
		#ifdef DEBUG
		// Log the whole string.
		RECIPE_DATABASE_TRACE("SELECT-Query: {0}", query_str);
		#endif // DEBUG

		// Create a SQLite query and bind the necessary parameter to the query.
		SQLite::Statement query_1(db,
			query_str);
		if (!filter.nationality.empty()) {

			query_1.bind(bind_count,
				filter.recipe_name);
			bind_count++;
		}
		if (!filter.nationality.empty() && filter.is_nationality) {

			query_1.bind(bind_count,
				filter.nationality);
			bind_count++;
		}
		if (!filter.dish_types.empty()) {

			for (auto it = filter.dish_types.begin(); it != filter.dish_types.end(); ++it) {

				if (it->second == true) {

					query_1.bind(bind_count,
						it->first);
					bind_count++;
				}
			}
		}
		if (!filter.ingredient_types.empty()) {

			for (auto it = filter.ingredient_types.begin(); it != filter.ingredient_types.end(); ++it) {

				if (it->second) {

					query_1.bind(bind_count,
						it->first);
					bind_count++;
				}
			}
		}
		if (!filter.ingredients.empty()) {

			for (auto it = filter.ingredients.begin(); it != filter.ingredients.end(); ++it) {


				if (it->second == true) {

					query_1.bind(bind_count,
						it->first);
					bind_count++;
				}
			}
		}

		// Iterate through the executed query and store the Recipe information in the recipes vector.
		while (query_1.executeStep()) {

			if (recipes.empty()) {

				Recipe recipe;
				recipe.id = query_1.getColumn(0).getString();
				recipe.name = query_1.getColumn(1).getString();
				recipe.course = query_1.getColumn(2).getString();
				recipe.servings = std::stoul(query_1.getColumn(3).getString());
				recipe.nationality = query_1.getColumn(4).getString();
				recipe.instructions = query_1.getColumn(5).getString();
				// Store the filepath(s) of the pictures divided by a comma in the
				// pictures vector of the Recipe object.
				int start = 0;
				for (int i = 0; i < query_1.getColumn(6).getString().size(); i++) {

					if (query_1.getColumn(6).getString()[i] == (char)",") {

						int count = i - start;

						std::string file_path = query_1.getColumn(6).getString().substr(start, count);

						recipe.pictures.push_back(file_path);

						start = i + 1;
					}
					if (i == query_1.getColumn(6).getString().size() - 1 ) {

						int count = i - (start - 1);

						std::string file_path = query_1.getColumn(6).getString().substr(start, count);

						recipe.pictures.push_back(file_path);
					}
				}

				recipes.push_back(recipe);
			}
			if (recipes.back().id != query_1.getColumn(0).getString()) {

				Recipe recipe;
				recipe.id = query_1.getColumn(0).getString();
				recipe.name = query_1.getColumn(1).getString();
				recipe.course = query_1.getColumn(2).getString();
				recipe.servings = std::stoul(query_1.getColumn(3).getString());
				recipe.nationality = query_1.getColumn(4).getString();
				recipe.instructions = query_1.getColumn(5).getString();
				// Store the filepath(s) of the pictures divided by a comma in the
				// pictures vector of the Recipe object.
				int start = 0;
				for (int i = 0; i < query_1.getColumn(6).getString().size(); i++) {

					if (query_1.getColumn(6).getString()[i] == (char)",") {

						int count = i - start;

						std::string file_path = query_1.getColumn(6).getString().substr(start, count);

						recipe.pictures.push_back(file_path);

						start = i + 1;
					}
					if (i == query_1.getColumn(6).getString().size() - 1) {

						int count = i - (start - 1);

						std::string file_path = query_1.getColumn(6).getString().substr(start, count);

						recipe.pictures.push_back(file_path);
					}
				}

				recipes.push_back(recipe);
			}
		}

		// Iterate through filter.dish_types and throw out all the recipes where
		// the bool was set to false; so those recipes that should be excluded from results
		// because they are of an undesirable dish type.
		// Do the same for filter.ingredients and filter.ingredient_types
		// and filter out recipes with undesirable ingredients or overarching types of ingredients.
		for (auto it = filter.dish_types.begin(); it != filter.dish_types.end(); it++) {

			if (!it->second) {

				SQLite::Statement query_2(db,
					"SELECT recipe_id FROM dish_types WHERE dish_type=?;");
				query_2.bind(1,
					it->first);
				
				while (query_2.executeStep()) {

					for (auto it = recipes.begin(); it != recipes.end();) {

						if (it->id == query_2.getColumn(0).getString()) {

							it = recipes.erase(it);
						}
						else {

							++it;
						}
					}
				}
			}
		}
		for (auto it = filter.ingredient_types.begin(); it != filter.ingredient_types.end(); ++it) {

			if (!it->second) {

				SQLite::Statement query_3(db,
					"SELECT recipe_id FROM ingredients WHERE ingredient_type=?;");
				query_3.bind(1,
					it->first);

				while (query_3.executeStep()) {

					for (auto it = recipes.begin(); it != recipes.end();) {

						if (it->id == query_3.getColumn(0).getString()) {

							it = recipes.erase(it);
						}
						else {

							++it;
						}
					}
				}
			}
		}
		for (auto it = filter.ingredients.begin(); it != filter.ingredients.end(); ++it) {

			if (!it->second) {

				SQLite::Statement query_4(db,
					"SELECT recipe_id FROM ingredients WHERE ingredient_name=?;");
				query_4.bind(1,
					it->first);

				while (query_4.executeStep()) {

					for (auto it = recipes.begin(); it != recipes.end();) {

						if (it->id == query_4.getColumn(0).getString()) {

							it = recipes.erase(it);
						}
						else {

							++it;
						}
					}
				}
			}
		}

		// Once the recipe list has been properly reduced complete the individual recipes 
		// by querying the dish_types and ingredients tables.
		for (auto it = recipes.begin(); it != recipes.end(); ++it) {

			// Add the different dish types to the recipes.
			SQLite::Statement query_5(db,
				"SELECT dish_type FROM dish_types WHERE recipe_id=?;");
			query_5.bind(1,
				it->id);

			while (query_5.executeStep()) {

				std::string type = query_5.getColumn(0).getString();
				it->dish_type.push_back(type);
			}

			// Add the different ingredients to the recipes.
			query_str.clear();
			query_str.append("SELECT ");
			query_str.append("ingredient_name, ingredient_type, unit, amount, currency, price ");
			query_str.append("FROM ingredients ");
			query_str.append("WHERE recipe_id=?;");
			SQLite::Statement query_6(db,
				query_str);
			query_6.bind(1,
				it->id);

			while (query_6.executeStep()) {

				Ingredient ingredient;
				ingredient.name = query_6.getColumn(0).getString();
				ingredient.type = query_6.getColumn(1).getString();
				ingredient.unit = static_cast<Unit>(std::stoul(query_6.getColumn(2).getString()));
				ingredient.amount = std::stof(query_6.getColumn(3).getString());
				ingredient.currency = static_cast<Currency>(std::stoul(query_6.getColumn(4).getString()));
				ingredient.price = std::stof(query_6.getColumn(5).getString());

				it->ingredients.push_back(ingredient);
			}
		}
	}
	catch (std::exception& e) {

		RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", 
			e.what());

		return false;
	}

	// Set sql_database_connected to false so that other threads that may be waiting to 
	// execute can connect to the database.
	sql_database_connected = false;

	return true;
}

bool Recipe_Database::RecipeApp::SqlUpdateQuery(Recipe recipe_old,
	Recipe recipe_new) {

	// Check if the database is currently connected on another thread
	// to avoid any consistency issues due to the missing implementation
	// of mutexes in the SQLiteCpp library.
	while (sql_database_connected == true) {

		// Have the thread wait with its execution for 1 second;
		// then check again.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	const std::lock_guard<std::mutex> lock(recipes_mutex);
	
	try {

		SQLite::Database db("./data/recipe_database.db3",
			SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

		// Set sql_database_connected to true so other SQL queries cannot be executed while
		// this thread is active.
		sql_database_connected = true;

		std::string query_str;
		query_str.clear();
		query_str.append("UPDATE recipes ");
		query_str.append("SET recipe_name=?, course=?, servings=?, nationality=?, ");
		query_str.append("instructions=?, picture=? ");
		query_str.append("WHERE recipe_id=?");
		SQLite::Statement recipe_query(db,
			query_str);
		recipe_query.bind(1, recipe_new.name);
		recipe_query.bind(2, recipe_new.course);
		recipe_query.bind(3, std::to_string(recipe_new.servings));
		recipe_query.bind(4, recipe_new.nationality);
		recipe_query.bind(5, recipe_new.instructions);
		// Concatenate the filepaths of all pictures for the recipe into one string.
		std::string picture_str;
		for (int i = 0; i < recipe_new.pictures.size(); i++) {

			if (i < recipe_new.pictures.size() - 1) {
				
				picture_str.append(recipe_new.pictures[i]);
				picture_str.append(",");
			}
			if (i == recipe_new.pictures.size() - 1) {

				picture_str.append(recipe_new.pictures[i]);
			}
		}
		recipe_query.bind(6, picture_str);
		recipe_query.bind(7, recipe_old.id);
		recipe_query.exec();

		// Update the ingredients in the recipe according to how many ingredients are 
		// listed in the new recipe.
		query_str.clear();
		query_str.append("UPDATE ingredients ");
		query_str.append("SET ingredient_name=?, ingredient_type=?, unit=?, amount=? ");
		query_str.append("currency=?, price=?");
		query_str.append("WHERE recipe_id=? AND ingredient_name=?;");
		if (recipe_old.ingredients.size() == recipe_new.ingredients.size()) {

			for (int i = 0; i < recipe_old.ingredients.size(); i++) {

				SQLite::Statement ingredient_query(db,
					query_str);
				ingredient_query.bind(1, recipe_new.ingredients[i].name);
				ingredient_query.bind(2, recipe_new.ingredients[i].type);
				ingredient_query.bind(3, std::to_string(static_cast<int>(recipe_new.ingredients[i].unit)));
				ingredient_query.bind(4, std::to_string(recipe_new.ingredients[i].amount));
				ingredient_query.bind(5, std::to_string(static_cast<int>(recipe_new.ingredients[i].currency)));
				ingredient_query.bind(6, std::to_string(recipe_new.ingredients[i].price));
				ingredient_query.bind(7, recipe_old.id);
				ingredient_query.bind(8, recipe_old.ingredients[i].name);
				ingredient_query.exec();
			}
		}
		else if (recipe_old.ingredients.size() > recipe_new.ingredients.size()) {

			int difference = recipe_old.ingredients.size() - recipe_new.ingredients.size();

			for (int i = 0; i < recipe_new.ingredients.size(); i++) {

				SQLite::Statement ingredient_query(db,
					query_str);
				ingredient_query.bind(1, recipe_new.ingredients[i].name);
				ingredient_query.bind(2, recipe_new.ingredients[i].type);
				ingredient_query.bind(3, std::to_string(static_cast<int>(recipe_new.ingredients[i].unit)));
				ingredient_query.bind(4, std::to_string(recipe_new.ingredients[i].amount));
				ingredient_query.bind(5, std::to_string(static_cast<int>(recipe_new.ingredients[i].currency)));
				ingredient_query.bind(6, std::to_string(recipe_new.ingredients[i].price));
				ingredient_query.bind(7, recipe_old.id);
				ingredient_query.bind(8, recipe_old.ingredients[i].name);
				ingredient_query.exec();
			}
			// DELETE any "leftovers".
			for (int i = difference; i < recipe_old.ingredients.size(); i++) {

				SQLite::Statement ingredient_query(db,
					"DELETE FROM ingredients WHERE recipe_id=? AND ingredient_name=?;");
				ingredient_query.bind(1, recipe_old.id);
				ingredient_query.bind(2, recipe_old.ingredients[i].name);
				ingredient_query.exec();
			}
		}
		else if (recipe_old.ingredients.size() < recipe_new.ingredients.size()) {

			int difference = recipe_new.ingredients.size() - recipe_old.ingredients.size();

			for (int i = 0; i < recipe_old.ingredients.size(); i++) {

				SQLite::Statement ingredient_query(db,
					query_str);
				ingredient_query.bind(1, recipe_new.ingredients[i].name);
				ingredient_query.bind(2, recipe_new.ingredients[i].type);
				ingredient_query.bind(3, std::to_string(static_cast<int>(recipe_new.ingredients[i].unit)));
				ingredient_query.bind(4, std::to_string(recipe_new.ingredients[i].amount));
				ingredient_query.bind(5, std::to_string(static_cast<int>(recipe_new.ingredients[i].currency)));
				ingredient_query.bind(6, std::to_string(recipe_new.ingredients[i].price));
				ingredient_query.bind(7, recipe_old.id);
				ingredient_query.bind(8, recipe_old.ingredients[i].name);
				ingredient_query.exec();

				// Finalise the ingredient_query.
				ingredient_query.~Statement();
			}
			for (int i = difference; i < recipe_new.ingredients.size(); i++) {

				query_str.clear();
				query_str.append("INSERT INTO ingredients(recipe_id, ingredient_name, ingredient_type");
				query_str.append(", unit, amount, currency, price) ");
				query_str.append("VALUES (?, ?, ?, ?, ?, ?, ?);");
				SQLite::Statement ingredient_query(db,
					query_str);
				ingredient_query.bind(1, recipe_old.id);
				ingredient_query.bind(1, recipe_new.ingredients[i].name);
				ingredient_query.bind(1, recipe_new.ingredients[i].type);
				ingredient_query.bind(1, std::to_string(static_cast<int>(recipe_new.ingredients[i].unit)));
				ingredient_query.bind(1, std::to_string(recipe_new.ingredients[i].amount));
				ingredient_query.bind(1, std::to_string(static_cast<int>(recipe_new.ingredients[i].currency)));
				ingredient_query.bind(1, std::to_string(recipe_new.ingredients[i].price));
				ingredient_query.exec();

				// Finalise the ingredient_query.
				ingredient_query.~Statement();
			}
		}

		// Update the dish_types in the recipe according to how man dish_types are
		// listed in the new recipe.
		query_str.clear();
		query_str.append("UPDATE dish_types ");
		query_str.append("SET dish_type=? ");
		query_str.append("WHERE recipe_id=? AND dish_type=?;");
		if (recipe_old.dish_type.size() == recipe_new.dish_type.size()) {

			for (int i = 0; i < recipe_old.dish_type.size(); i++) {

				SQLite::Statement type_query(db,
					query_str);
				type_query.bind(1, recipe_new.dish_type[i]);
				type_query.bind(2, recipe_old.id);
				type_query.bind(3, recipe_old.dish_type[i]);
				type_query.exec();
			}
		}
		else if (recipe_old.dish_type.size() > recipe_new.dish_type.size()) {

			int difference = recipe_old.dish_type.size() - recipe_new.dish_type.size();
			for (int i = 0; i < recipe_new.dish_type.size(); i++) {

				SQLite::Statement type_query(db,
					query_str);
				type_query.bind(1, recipe_new.dish_type[i]);
				type_query.bind(2, recipe_old.id);
				type_query.bind(3, recipe_old.dish_type[i]);
				type_query.exec();
			}
			for (int i = difference; i < recipe_old.dish_type.size(); i++) {

				SQLite::Statement type_query(db,
					"DELETE FROM dish_types WHERE recipe_id=? AND dish_type=?;");
				type_query.bind(1, recipe_old.id);
				type_query.bind(2, recipe_old.dish_type[i]);
				type_query.exec();
			}
		}
		else if (recipe_old.dish_type.size() < recipe_new.dish_type.size()) {

			int difference = recipe_new.dish_type.size() - recipe_old.dish_type.size();

			for (int i = 0; i < recipe_old.dish_type.size(); i++) {

				SQLite::Statement type_query(db,
					query_str);
				type_query.bind(1, recipe_new.dish_type[i]);
				type_query.bind(2, recipe_old.id);
				type_query.bind(3, recipe_old.dish_type[i]);
				type_query.exec();
			}
			for (int i = difference; i < recipe_new.dish_type.size(); i++) {

				query_str.clear();
				query_str.append("INSERT INTO dish_types(recipe_id, dish_type) ");
				query_str.append("VALUES (?, ?);");
				SQLite::Statement type_query(db,
					query_str);
				type_query.bind(1, recipe_old.id);
				type_query.bind(2, recipe_new.dish_type[i]);
				type_query.exec();
			}
		}
	}
	catch (std::exception& e) {

		RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", 
			e.what());

		return false;
	}
	
	// Set sql_database_connected to false so that other threads that may be waiting to 
	// execute can connect to the database.
	sql_database_connected = false;

	return true;
}