#pragma once
#ifndef RECIPE_H
#define RECIPE_H

#include "RecipeDatabasePch.h"

namespace Recipe_Database {

	enum class Unit {

		NA = 0,
		// Weight
		KILOGRAM,
		GRAM,
		// Volume
		LITRE,
		DECILITRE,
		CENTILITRE,
		MILLILITRE
	};

	struct Ingredient {

		std::string name;
		std::string type;
		Unit unit = Unit::NA;
		float amount;
		Currency currency = Currency::NA;
		float price_per_unit;
	};

	struct Recipe {

		std::string recipe_id;
		std::string name;
		std::string course;
		std::vector<std::string> dish_type;
		int servings = 10;
		std::string nationality;
		std::vector<Ingredient> ingredients;
		std::string instructions;
	};
}

#endif // !RECIPE_H

