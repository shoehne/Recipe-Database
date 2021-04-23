#pragma once
#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "Events/Event.h"

namespace Recipe_Database {

	class ApplicationUpdateEvent : public Event {

	public:
		ApplicationUpdateEvent() = default;

		EVENT_CLASS_TYPE(APPLICATION_UPDATE);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
	};

	class WindowCloseEvent : public Event {

	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WINDOW_CLOSE);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
	};

	class WindowResizeEvent : public Event {

	public:
		WindowResizeEvent(unsigned int w,
			unsigned int h)
			: width(w), height(h) {}

		unsigned int GetWidth() const { return width; }
		unsigned int GetHeight() const { return height; }

		// For debugging purposes
		#ifdef DEBUG
		std::string ToString() const override {

			std::stringstream stream;
			stream << "WindowResizeEvent: " << width << "," << height;
			return stream.str();
		}
		#endif // DEBUG

		EVENT_CLASS_TYPE(WINDOW_RESIZED);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);

	private:
		unsigned int width;
		unsigned int height;
	};
}

#endif // !WINDOWEVENT_H

