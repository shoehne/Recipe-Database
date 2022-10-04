#include "RecipeDatabasePch.h"

#include "Core/Sql.h"

namespace Recipe_Database {

	class SqlQuery;

	SqlQueryStack::~SqlQueryStack() {

		for (auto it = end();
			it != begin();
			--it) {

			queries.erase(it);
		}
	}

	void SqlQueryStack::ExecuteQuery() {

		// Execute the Run function of the first query in the stack.
		// To DO: Make the query execute in a separate thread to allow the user to
		// stack queries ie. select multiple filters without having to deal with a frozen UI.
		if (!queries.empty()) {
		
			RECIPE_DATABASE_TRACE("Querystack is empty!");

			return;
		}
		
		auto it = queries.begin();
		(*(*it))->Run();
	}

	void SqlQueryStack::OnEvent(Event& e) {

		// Remove the first element of the stack and set query_running to "false".
		//query_running = false;
		auto it = queries.begin();
		queries.erase(it);

		ExecuteQuery();
	}

	void SqlQueryStack::OnUpdate() {


	}

	void SqlQueryStack::PushQuery(SqlQuery* query) {

		// Emplace the new query at back of the stack. If there is currently no query executing
		// execute the first query on the stack i.e. when there are no queries currently being 
		// executed execute the first query on the stack.
		query->SetEventCallback(BIND_EVENT(SqlQueryStack::OnEvent));
		queries.emplace_back(CreateScope<Recipe_Database::SqlQuery*>(query));
		ExecuteQuery();
	}

	SqlDeleteQuery::SqlDeleteQuery(std::string id,
		std::string name) {

		recipe_id = id;
		recipe_name = name;
	}

	SqlDeleteQuery::~SqlDeleteQuery() {


	}

	bool SqlDeleteQuery::Run() {

		try {

			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

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

		#ifdef DEBUG
		RECIPE_DATABASE_INFO("INFO: Deleted recipe {0}",
			recipe_name);
		#endif // DEBUG

		// Create and call a SqlQueryFinishedEvent to tell the SqlQueryStack that the operation has finished and
		// it can continue with the next query in the queue.
		SqlQueryFinishedEvent event;
		event_callback(event);

		return true;
	}

	void SqlDeleteQuery::SetEventCallback(const EventCallbackFn& callback) {

		event_callback = callback;
	}

	SqlInsertQuery::SqlInsertQuery(Recipe r) {

		recipe = r;
	}

	SqlInsertQuery::~SqlInsertQuery() {


	}

	bool SqlInsertQuery::Run() {

		// If DEBUG is defined run a timer to see if the query completes in an 
		// acceptable time frame with larger databases.

		// Create a unique ID for the recipe.
		std::string id;
		do {
		
			Recipe_Database::CreateUuid(id);
		} while (SqlInsertQuery::DuplicateId(id));

		try {
			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

			std::string query_str;
			query_str.clear();
			query_str.append("INSERT INTO recipes(recipe_id, recipe_name, course, servings, nationality, ");
			query_str.append("instructions, picture)");
			query_str.append(" VALUES(?, ?, ?, ?, ?, ?, ?);");
			SQLite::Statement recipe_query(db,
				query_str);
			recipe_query.bind(1, id);
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

		// Create and call a SqlQueryFinishedEvent to tell the SqlQueryStack that the operation has finished and
		// it can continue with the next query in the queue.
		SqlQueryFinishedEvent event;
		event_callback(event);

		return true;
	}

	void SqlInsertQuery::SetEventCallback(const EventCallbackFn& callback) {

		event_callback = callback;
	}

	bool SqlInsertQuery::DuplicateId(std::string id) {

		// Check whether the created ID is for some reason already 
		// stored in the database.
		try {

			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

			SQLite::Statement query(db, "SELECT recipe_id FROM recipes WHERE recipe_id = ?;");
			query.bind(1, id);

			if (query.executeStep()) {

				RECIPE_DATABASE_WARN("WARNING: The generated ID already exists!");
				id.clear();

				return true;
			}
		}
		catch (std::exception& e) {

			RECIPE_DATABASE_ERROR("SQL-ERROR: {0}", e.what());

			return true;
		}
	}

	SqlSelectQuery::SqlSelectQuery(Ref<std::vector<Recipe_Database::Recipe>> r, 
		SqlSelectFilter sf) {

		filter = sf;
		recipes = r;
	}

	SqlSelectQuery::~SqlSelectQuery() {


	}

	// !!!ATTENTION: KEEP recipes THREAD-SAFE!!!
	bool SqlSelectQuery::Run() {

		try {

			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

			bool and_needed = false;
			int bind_count = 1;
			std::string query_str;

			// Only include items to be "included" - where the boolean in filter is set to true - in a SELECT
			// query. The false values will be sorted out in separate queries.
			query_str.clear();
			query_str.append("SELECT recipes->recipe_id, recipes->recipe_name, recipes->course, ");
			query_str.append("recipes->servings, recipes->nationality, recipes->instructions, recipes->picture ");
			query_str.append("FROM recipes ");
			query_str.append("INNER JOIN dish_types ON dish_types.recipe_id=recipes->recipe_id ");
			query_str.append("INNER JOIN ingredients ON ingredients.recipe_id=recipes->recipe_id ");
			query_str.append("WHERE ");
			if (!filter.recipe_name.empty()) {

				query_str.append("recipes->recipe_name=? ");
				and_needed = true;
			}
			if (!filter.nationality.empty()) {

				if (and_needed) {

					query_str.append("AND ");
					and_needed = false;
				}
				if (filter.is_nationality) {

					query_str.append("recipes->nationality=? ");
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
			query_str.append("ORDER BY recipes->recipe_id;");
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

				if (recipes->empty()) {

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

					recipes->push_back(recipe);
				}
				if (recipes->back().id != query_1.getColumn(0).getString()) {

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

					recipes->push_back(recipe);
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

						for (auto it = recipes->begin(); it != recipes->end();) {

							if (it->id == query_2.getColumn(0).getString()) {

								it = recipes->erase(it);
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

						for (auto it = recipes->begin(); it != recipes->end();) {

							if (it->id == query_3.getColumn(0).getString()) {

								it = recipes->erase(it);
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

						for (auto it = recipes->begin(); it != recipes->end();) {

							if (it->id == query_4.getColumn(0).getString()) {

								it = recipes->erase(it);
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
			for (auto it = recipes->begin(); it != recipes->end(); ++it) {

				// Add the different dish types to the recipes->
				SQLite::Statement query_5(db,
					"SELECT dish_type FROM dish_types WHERE recipe_id=?;");
				query_5.bind(1,
					it->id);

				while (query_5.executeStep()) {

					std::string type = query_5.getColumn(0).getString();
					it->dish_type.push_back(type);
				}

				// Add the different ingredients to the recipes->
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

		// Create and call a SqlQueryFinishedEvent to tell the SqlQueryStack that the operation has finished and
		// it can continue with the next query in the queue.
		SqlQueryFinishedEvent event;
		event_callback(event);

		return true;
	}

	void SqlSelectQuery::SetEventCallback(const EventCallbackFn& callback) {

		event_callback = callback;
	}

	SqlUpdateQuery::SqlUpdateQuery(Recipe ro,
		Recipe rn) {

		recipe_new = rn;
		recipe_old = ro;
	}

	SqlUpdateQuery::~SqlUpdateQuery() {


	}

	bool SqlUpdateQuery::Run() {

		try {

			SQLite::Database db("./data/recipe_database.db3",
				SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

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

		// Create and call a SqlQueryFinishedEvent to tell the SqlQueryStack that the operation has finished and
		// it can continue with the next query in the queue.
		SqlQueryFinishedEvent event;
		event_callback(event);

		return true;
	}

	void SqlUpdateQuery::SetEventCallback(const EventCallbackFn& callback) {

		event_callback = callback;
	}
}