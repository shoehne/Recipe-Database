#pragma once
#ifndef SQLEVENT_H
#define SQLEVENT_H

#include "Events/Event.h"

namespace Recipe_Database {

	struct SqlQueryResult {


	};

	class SqlEvent : public Event {

	public:
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_SQL);
	};

	class SqlQueryEvent : public SqlEvent {

	public:
		SqlQueryEvent() {}

		EVENT_CLASS_TYPE(SQL_QUERY);
	};
}
#endif // !SQLEVENT_H

