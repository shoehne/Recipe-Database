#pragma once
#ifndef SQLEVENT_H
#define SQLEVENT_H

#include "Events/Event.h"

namespace Recipe_Database {

	class SqlEvent : public Event {

	public:
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_SQL);
	};

	class IngredientsQueriedEvent : public SqlEvent {

	public:
		IngredientsQueriedEvent() {}
	};
}
#endif // !SQLEVENT_H

