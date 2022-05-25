#pragma once
#ifndef SQLEVENT_H
#define SQLEVENT_H

#include "Core/Recipe.h"
#include "Events/Event.h"

namespace Recipe_Database {

	struct SqlSelectFilter {

		// Add a name to the filter. Searching via name will start 
		// a full text search so that recipes containing the given
		// keyword will be loaded.
		std::string recipe_name;
		// Add a "nationality" to the filter with the option to exclude a 
		// specific "nationality" from the results
		std::string nationality;
		bool is_nationality = true;
		// Add dish types to the filter with the option to exclude specific
		// dish types e.g. exclude all soups or salads.
		// Up to 3 types can be added.
		std::map<std::string, bool> dish_types;
		// Add ingredients selected by type to the filter with the option to exclude 
		// specific ingredients. Up to 3 types can be added.
		std::map<std::string, bool> ingredient_types;
		// Add specific ingredients by name to the filter giving the option 
		// to exclude specific ingredients.
		// Up to 6 different ingredients can be added.
		std::map<std::string, bool> ingredients;
	};

	class SqlEvent : public Event {

	public:
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_SQL);
	};

	class SqlDeleteEvent : public SqlEvent {

	public:
		SqlDeleteEvent(std::string id,
			std::string name)
			: recipe_id(id), recipe_name(name) {}

		std::string GetID() { return recipe_id; }
		std::string GetName() { return recipe_name; }

		EVENT_CLASS_TYPE(SQL_DELETE_QUERY);

	private:
		std::string recipe_id;
		std::string recipe_name;
	};
	
	class SqlInsertEvent : public SqlEvent {

	public:
		SqlInsertEvent(Recipe r)
			: recipe(r) {}

		Recipe GetRecipe() { return recipe; }

		EVENT_CLASS_TYPE(SQL_INSERT_QUERY);

	private:
		Recipe recipe;
	};

	class SqlQueryFinishedEvent : public SqlEvent {

	public:
		SqlQueryFinishedEvent() = default;

		EVENT_CLASS_TYPE(SQL_QUERY_FINISHED);
	};

	class SqlSelectEvent : public SqlEvent {

	public:
		SqlSelectEvent(SqlSelectFilter sf)
			: filter(sf) {}

		SqlSelectFilter GetFilter() { return filter; }

		EVENT_CLASS_TYPE(SQL_SELECT_QUERY);

	private:
		SqlSelectFilter filter;
	};

	class SqlUpdateEvent : public SqlEvent {

	public:
		SqlUpdateEvent(Recipe ro,
			Recipe rn)
			: recipe_old(ro), recipe_new(rn) {}

		Recipe GetRecipeOld() { return recipe_old; }
		Recipe GetRecipeNew() { return recipe_new; }

		EVENT_CLASS_TYPE(SQL_UPDATE_QUERY);

	private:
		Recipe recipe_new;
		Recipe recipe_old;
	};
}
#endif // !SQLEVENT_H

