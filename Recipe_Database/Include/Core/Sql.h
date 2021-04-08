#pragma once
#ifndef SQL_H
#define SQL_H

#include "Core/Core.h"
#include "Events/SqlEvent.h"

namespace Recipe_Database {

	enum class SqlQueryType : uint8_t {

		None = 0
	};

	struct QueryResult {


	};

	class SqlDatabase {

		using EventCallbackFn = std::function<void(Event&)>;

	public:
		SqlDatabase(std::string name);
		~SqlDatabase();

		void Query(SqlQueryType type);
		void SetEventCallback(const EventCallbackFn& callback);
		
	private:
		Scope<SqlQuery> query;
	};

	class SqlQuery {

	public:
		static Scope<SqlQuery> Create(SqlQueryType type);
		virtual QueryResult GetResult() = 0;
	};
}
#endif // !SQLDATABASE_H

