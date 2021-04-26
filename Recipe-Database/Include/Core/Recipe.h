#pragma once
#ifndef RECIPE_H
#define RECIPE_H

#include "RecipeDatabasePch.h"

namespace Recipe_Database {

	enum class Currency : uint8_t {

		NA = 0,
		EURO,
		GBP,
		JPY,
		USD
	};

	enum class Unit : uint8_t {

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
		float price;

		// Convert a value from one currency to another. Maybe get current exchange rates
		// from the internet.
		float CurrencyConversion(float value,
			Currency currency_src,
			Currency currency_target) {

			float value_new = 0;

			return value_new;
		}

		float VolumeConversion(float value,
			Unit unit_src,
			Unit unit_target) {

			float value_new;

			switch (static_cast<int>(unit_src)) {

			case (static_cast<int>(Unit::LITRE)): {

				switch (static_cast<int>(unit_target)) {

					case static_cast<int>(Unit::LITRE) : {

						return value;
					}
					case static_cast<int>(Unit::DECILITRE) : {

						value_new = value * 10;

						return value_new;
					}
					case static_cast<int>(Unit::CENTILITRE) : {

						value_new = value * 100;

						return value_new;
					}
					case static_cast<int>(Unit::MILLILITRE) : {

						value_new = value * 1000;

						return value_new;
					}
					default: {

						RECIPE_DATABASE_ERROR("Invalid conversion types!");

						return -1;
					}
				}
			}
				case static_cast<int>(Unit::DECILITRE) : {

					switch (static_cast<int>(unit_target)) {

						case static_cast<int>(Unit::LITRE) : {

							value_new = value / 10;

							return value_new;
						}
						case static_cast<int>(Unit::DECILITRE) : {

							return value;
						}
						case static_cast<int>(Unit::CENTILITRE) : {

							value_new = value * 10;

							return value_new;
						}
						case static_cast<int>(Unit::MILLILITRE) : {

							value_new = value * 100;

							return value_new;
						}
						default: {

							RECIPE_DATABASE_ERROR("Invalid conversion types!");

							return -1;
						}
					}
				}
				case static_cast<int>(Unit::CENTILITRE) : {

					switch (static_cast<int>(unit_target)) {

						case static_cast<int>(Unit::LITRE) : {

							value_new = value / 100;

							return value_new;
						}
						case static_cast<int>(Unit::DECILITRE) : {

							value_new = value / 10;

							return value_new;
						}
						case static_cast<int>(Unit::CENTILITRE) : {

							return value;
						}
						case static_cast<int>(Unit::MILLILITRE) : {

							value_new = value * 10;

							return value_new;
						}
						default: {

							RECIPE_DATABASE_ERROR("Invalid conversion types!");

							return -1;
						}
					}
				}
				case static_cast<int>(Unit::MILLILITRE) : {

					switch (static_cast<int>(unit_target)) {

						case static_cast<int>(Unit::LITRE) : {

							value_new = value / 1000;

							return value_new;
						}
						case static_cast<int>(Unit::DECILITRE) : {

							value_new = value / 100;

							return value_new;
						}
						case static_cast<int>(Unit::CENTILITRE) : {

							value_new = value / 10;

							return value_new;
						}
						case static_cast<int>(Unit::MILLILITRE) : {

							return value;
						}
						default: {

							RECIPE_DATABASE_ERROR("Invalid conversion types!");

							return -1;
						}
					}
				}
				default: {

					RECIPE_DATABASE_ERROR("Invalid conversion types!");

					return -1;
				}
			}
		}

		float WeightConversion(float value,
			Unit unit_src,
			Unit unit_target) {

			float value_new;

			switch (static_cast<int>(unit_src)) {

				case static_cast<int>(Unit::KILOGRAM) : {

					switch (static_cast<int>(unit_target)) {

						case static_cast<int>(Unit::KILOGRAM) : {

							return value;
						}
						case static_cast<int>(Unit::GRAM) : {

							value_new = value * 1000;

							return value_new;
						}
						default: {

							RECIPE_DATABASE_ERROR("Invalid conversion types!");

							return value;
						}
					}
				}
				case static_cast<int>(Unit::GRAM) : {

					switch (static_cast<int>(unit_target)) {

						case static_cast<int>(Unit::KILOGRAM) : {

							value_new = value / 1000;

							return value_new;
						}
						case static_cast<int>(Unit::GRAM) : {

							return value;
						}
						default: {

							RECIPE_DATABASE_ERROR("Invalid conversion types!");

							return value;
						}
					}
				}
				default: {

					RECIPE_DATABASE_ERROR("Invalid conversion types!");

					return value;
				}
			}
		}
	};

	struct Recipe {

		std::string id;
		std::string name;
		std::string course;
		std::vector<std::string> dish_type;
		uint8_t servings = 10;
		std::string nationality;
		std::vector<Ingredient> ingredients;
		std::string instructions;
		// File path to a picture
		std::string picture;

		// Calculate the amount needed depending on the new
		// number of servings required taking into account the 
		// base serving amount.
		// E.g. calculate the amount needed for 150 servings with a 
		// base amount of 10 servings.
		void servings_calculation(int servings_new) {

			// Just set the amount of each ingredient as calculated.
			// Recipe and Ingredient objects only serve as temporary
			// instances of the data stored in the database. Unless
			// the recipe gets saved none of the values will be permaenent.
			for (int i = 0; i < ingredients.size(); i++) {

				ingredients[i].amount = (ingredients[i].amount / servings) * servings_new;
			}
		}
	};
}

#endif // !RECIPE_H

