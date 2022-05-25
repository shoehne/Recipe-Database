#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Core/Core.h"
#include "Core/Layer.h"

#include <vector>

namespace Recipe_Database {

	class LayerStack {

	public: 
		LayerStack() = default;
		~LayerStack();

		void PopLayer(Scope<Layer*> layer);
		void PopOverlay(Scope<Layer*> overlay);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		std::vector<Scope<Layer*>>::iterator begin() { return layers.begin(); }
		std::vector<Scope<Layer*>>::const_iterator begin() const { return layers.begin(); }
		std::vector<Scope<Layer*>>::iterator end() { return layers.end(); }
		std::vector<Scope<Layer*>>::const_iterator end() const { return layers.end(); }
		std::vector<Scope<Layer*>>::reverse_iterator rbegin() { return layers.rbegin(); }
		std::vector<Scope<Layer*>>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
		std::vector<Scope<Layer*>>::reverse_iterator rend() { return layers.rend(); }
		std::vector<Scope<Layer*>>::const_reverse_iterator rend() const { return layers.rend(); }

	private:
		std::vector<Scope<Layer*>> layers;
		unsigned int layer_insert_index = 0;
	};
}

#endif // !LAYERSTACK_H
