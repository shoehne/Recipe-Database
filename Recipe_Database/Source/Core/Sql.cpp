//------Precompiled header------
#include "RecipeDatabasePch.h"

#include "Core/Sql.h"

Recipe_Database::SqlDatabase::SqlDatabase(std::string name) {

	query = SqlQuery::Create(SqlQueryType::None);
}