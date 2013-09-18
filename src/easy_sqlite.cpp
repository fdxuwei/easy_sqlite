#include "easy_sqlite.h"

using namespace std;

namespace easy_sqlite
{
	result_set::result_set()
		: m_stmt(NULL)
	{

	}

	result_set::~result_set()
	{
		if(m_stmt)
		{
			sqlite3_finalize(m_stmt);
		}
	}

	sqlite3_stmt*& result_set::get_stmt()
	{
		return m_stmt;
	}

	bool result_set::move_next()
	{
		return (sqlite3_step(m_stmt) == SQLITE_ROW);
	}

	int result_set::get_column_value_int(int col_index)
	{
		return sqlite3_column_int(m_stmt, col_index);
	}

	int result_set::get_column_value_int(const std::string& col_name)
	{
		return sqlite3_column_int(m_stmt, m_col_name_to_index[col_name]);
	}

	string result_set::get_column_value_text(int col_index)
	{
		return (const char*)sqlite3_column_text(m_stmt, col_index);
	}

	string result_set::get_column_value_text(const std::string& col_name)
	{
		return (const char*)sqlite3_column_text(m_stmt, m_col_name_to_index[col_name]);
	}

	const void *result_set::get_column_value_blob(int col_index)
	{
		return sqlite3_column_blob(m_stmt, col_index);
	}

	const void *result_set::get_column_value_blob(const std::string& col_name)
	{
		return sqlite3_column_blob(m_stmt, m_col_name_to_index[col_name]);
	}

	int result_set::get_column_bytes(int col_index)
	{
		return sqlite3_column_bytes(m_stmt, col_index);
	}

	int result_set::get_column_bytes(const std::string& col_name)
	{
		return sqlite3_column_bytes(m_stmt, m_col_name_to_index[col_name]);
	}

	void result_set::set_column_info()
	{
		int column_count = sqlite3_column_count(m_stmt);
		for(int i = 0; i < column_count; i++)
		{
			string col_name = sqlite3_column_name(m_stmt, i);
			m_col_name_to_index[col_name] = i;
		}
	}

	sqlite::sqlite(const string& file_name)
		: m_file_name(file_name)
		, m_is_open(false)
	{
	}

	sqlite::sqlite()
		: m_file_name("")
		, m_is_open(false)
	{
	}

	sqlite::~sqlite(void)
	{
		close();
	}

	bool sqlite::open()
	{
		if(sqlite3_open(m_file_name.c_str(), &m_db) == SQLITE_OK)
		{
			m_is_open = true;
			return true;
		}
		return false;
	}

	void sqlite::close()
	{
		sqlite3_close_v2(m_db);
		m_is_open = false;
	}

	result_set_ptr sqlite::get_resultset(const std::string &sql)
	{
		if(!is_open())
		{
			open();
		}

		result_set_ptr rsp(new result_set);
		sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &(rsp->get_stmt()), NULL);
		rsp->set_column_info();
		return rsp;
	}

	bool sqlite::exec(const std::string &sql)
	{
		if(!is_open())
		{
			open();
		}

		sqlite3_stmt *pstmt = NULL;
		sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &pstmt, NULL);
		int ret = sqlite3_step(pstmt);
		sqlite3_finalize(pstmt);
		return (SQLITE_DONE == ret);
	}

	sqlite3_stmt* sqlite::prepare_stmt(const std::string& sql)
	{
		if(!is_open())
		{
			open();
		}

		sqlite3_stmt *pstmt = NULL;
		sqlite3_prepare_v2(m_db, sql.c_str(), sql.length(), &pstmt, NULL);
		return pstmt;
	}

	bool sqlite::exec(sqlite3_stmt *stmt)
	{
		if(!is_open())
		{
			open();
		}

		int ret = sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return (SQLITE_DONE == ret);
	}


	bool sqlite::is_open()
	{
		return m_is_open;
	}

	bool sqlite::begin_trans()
	{
		return exec("begin");
	}

	bool sqlite::commit()
	{
		return exec("commit");
	}

	void sqlite::set_filename(const std::string& file_name)
	{
		m_file_name = file_name;
	}

	bool sqlite::is_table_exist(const std::string& table_name)
	{
		if(!is_open())
		{
			open();
		}

		string sql = "select 1 from sqlite_master where tbl_name='" + table_name + "' and type='table'";
		result_set_ptr rsp = get_resultset(sql);
		if((rsp.get() != NULL) && rsp->move_next())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



