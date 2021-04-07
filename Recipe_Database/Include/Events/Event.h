#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "RecipeDatabasePch.h"

namespace Recipe_Database {

	enum class EventType {

		None = 0,
		APPLICATION_TICK,
		APPLICATION_UPDATE,
		KEY_PRESSED,
		KEY_RELEASED,
		MOUSE_BUTTON_MOVED,
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_BUTTON_SCROLLED,
		WINDOW_CLOSE,
		WINDOW_FOCUS,
		WINDOW_LOST_FOCUS,
		WINDOW_MOVED,
		WINDOW_RESIZED
	};

	enum EventCategory {

		None = 0,
		EVENT_CATEGORY_APPLICATION,
		EVENT_CATEGORY_INPUT,
		EVENT_CATEGORY_KEYBOARD,
		EVENT_CATEGORY_MOUSE,
		EVENT_CATEGORY_MOUSE_BUTTON
	};

// Macro to simplify implementation of the virtual event functions that need to be implemented
// for each child class of Recipe_Database::Event.
#ifdef DEBUG
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
								virtual EventType GetEventType() const override {return GetStaticType();}\
								virtual const char* GetName() const override {return #type;}
#else
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
								virtual EventType GetEventType() const override {return GetStaticType();}
#endif // DEBUG

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class Event {

		friend class EventDispatcher;
	public:
		virtual int GetCategoryFlags() const = 0;
		virtual EventType GetEventType() const = 0;

		inline bool IsInCategory(EventCategory category) {

			return GetCategoryFlags() & category;
		}

		// Debug functions
		#ifdef DEBUG
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		#endif // DEBUG

	protected:
		bool handled = false;
	};

	class EventDispatcher {

		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& e) : event(e) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {

			if (event.GetEventType() == T::GetStaticType()) {

				event.handled = func(*(T*)&event);
				return true;
			}
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {

		return os << e.ToString();
	}
}

#endif // !EVENT_H

