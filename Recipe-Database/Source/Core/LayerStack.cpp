#include "RecipeDatabasePch.h"

#include "Core/LayerStack.h"

namespace Recipe_Database {

	LayerStack::~LayerStack() {

		for (auto it = end();
			it != begin();
			--it) {

			(*(*it))->OnDetach();
			layers.erase(it);
		}
	}

	void LayerStack::PopLayer(Scope<Layer*> layer) {

		auto it = std::find(layers.begin(),
			layers.begin() + layer_insert_index,
			layer);
		if (it != layers.begin() + layer_insert_index) {

			if (!(*(*it))->OnDetach()) {

				RECIPE_DATABASE_ERROR("Failed to detach layer!");

				return;
			}
			layers.erase(it);
			layer_insert_index--;
		}
	}

	void LayerStack::PopOverlay(Scope<Layer*> overlay) {

		auto it = std::find(layers.begin() + layer_insert_index,
			layers.end(),
			overlay);
		if (it != layers.end()) {

			if (!(*(*it))->OnDetach()) {

				RECIPE_DATABASE_ERROR("Failed to detach overlay!");

				return;
			}
			layers.erase(it);
		}
	}

	void LayerStack::PushLayer(Layer* layer) {

		if (!layer->OnAttach()) {

			RECIPE_DATABASE_ERROR("Failed to attach layer!");

			return;
		}
		layers.emplace(layers.begin() + layer_insert_index,
			CreateScope<Recipe_Database::Layer*>(layer));
		layer_insert_index++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {

		if (!overlay->OnAttach()) {

			RECIPE_DATABASE_ERROR("Failed to attach overlay!");

			return;
		}
		layers.emplace_back(CreateScope<Recipe_Database::Layer*>(overlay));
	}
}