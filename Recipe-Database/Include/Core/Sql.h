#ifndef SQL_H
#define SQL_H

#include "Core.h"
#include "Events/SqlEvent.h"

#include <SQLiteCpp/SQLiteCpp.h>

namespace Recipe_Database {

	class SqlQuery;

	class SqlQueryStack {

	public:
		SqlQueryStack() = default;
		~SqlQueryStack();

		void ExecuteQuery();
		void OnEvent(Event& e);
		void OnUpdate();
		void PushQuery(SqlQuery* query);

		std::vector<Scope<SqlQuery*>>::iterator begin() { return queries.begin(); }
		std::vector<Scope<SqlQuery*>>::const_iterator begin() const { return queries.begin(); }
		std::vector<Scope<SqlQuery*>>::iterator end() { return queries.end(); }
		std::vector<Scope<SqlQuery*>>::const_iterator end() const { return queries.end(); }
		std::vector<Scope<SqlQuery*>>::reverse_iterator rbegin() { return queries.rbegin(); }
		std::vector<Scope<SqlQuery*>>::const_reverse_iterator rbegin() const { return queries.rbegin(); }
		std::vector<Scope<SqlQuery*>>::reverse_iterator rend() { return queries.rend(); }
		std::vector<Scope<SqlQuery*>>::const_reverse_iterator rend() const { return queries.rend(); }
		
	private:
		std::vector<Scope<SqlQuery*>> queries;
		bool query_running = false;
	};

	class SqlQuery {

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		SqlQuery() = default;
		~SqlQuery();

		virtual bool Run() = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	};

	class SqlDeleteQuery : public SqlQuery {

	public:
		SqlDeleteQuery(std::string id,
			std::string name);
		~SqlDeleteQuery();

		virtual bool Run() override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		EventCallbackFn event_callback;
		std::string recipe_id, recipe_name;
	};

	class SqlInsertQuery : public SqlQuery {

	public:
		SqlInsertQuery(Recipe r);
		~SqlInsertQuery();

		virtual bool Run() override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		EventCallbackFn event_callback;
		Recipe recipe;
	};

	class SqlSelectQuery : public SqlQuery {

	public:
		SqlSelectQuery(Ref<std::vector<Recipe_Database::Recipe>> r,
			SqlSelectFilter sf);
		~SqlSelectQuery();

		virtual bool Run() override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		EventCallbackFn event_callback;
		SqlSelectFilter filter;
		Ref<std::vector<Recipe_Database::Recipe>> recipes;
	};

	class SqlUpdateQuery : public SqlQuery {

	public:
		SqlUpdateQuery(Recipe ro,
			Recipe rn);
		~SqlUpdateQuery();

		virtual bool Run() override;
		virtual void SetEventCallback(const EventCallbackFn& callback) override;

	private:
		EventCallbackFn event_callback;
		Recipe recipe_new, recipe_old;
	};
}

#endif // !SQLITE_H
