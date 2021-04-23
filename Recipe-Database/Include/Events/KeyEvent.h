#pragma once
#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Event.h"

namespace Recipe_Database {
	
	class KeyEvent : public Event {

	public:
		inline int GetKeyCode() const { return key_code; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);
	protected:
		KeyEvent(int key) : key_code(key) {}

		int key_code;
	};

	class KeyPressedEvent : public KeyEvent {

	public:
		KeyPressedEvent(int key,
			int repeat)
			: KeyEvent(key), repeat_count(repeat) {}
		
		inline int GetRepeatCount() const { return repeat_count; }
		
		// For debugging purposes.
		#ifdef DEBUG
		std::string ToString() const override {

			std::stringstream stream;
			stream << "KeyPressedEvent: " << key_code << " (" << repeat_count << " repeats)";
			return stream.str();
		}
		#endif // DEBUG

		EVENT_CLASS_TYPE(KEY_PRESSED);

	private:
		int repeat_count;
	};

	class KeyReleasedEvent : public KeyEvent {

	public:
		KeyReleasedEvent(int key) : KeyEvent(key) {}

		// For debugging purposes.
		#ifdef DEBUG
		std::string ToString() const override {

			std::stringstream stream;
			stream << "KeyReleasedEvent: " << key_code;
			return stream.str();
		}
		#endif // DEBUG

		EVENT_CLASS_TYPE(KEY_RELEASED);
	};
}
#endif // !KEYEVENT_H

