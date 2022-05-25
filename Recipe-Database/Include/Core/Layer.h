#ifndef LAYER_H
#define LAYER_H

#include "Core/Core.h"
#include "Events/Event.h"

namespace Recipe_Database {

	class Layer {

	public:
		Layer() = default;
		virtual ~Layer() = 0;

		virtual bool OnAttach() = 0;
		virtual bool OnDetach() = 0;
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}
		virtual void OnUpdate() = 0;
	};
}

#endif // !LAYER_H
