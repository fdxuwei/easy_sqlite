#ifndef __EASY_SQLITE_H__
#define __EASY_SQLITE_H__

#include <string>
#include <memory>
#include <map>
#include "sqlite3.h"

namespace easy_sqlite
{
	class result_set;
	// 记录集指针
	typedef std::auto_ptr<result_set> result_set_ptr;

	const int c_left_index = 0;
	// 记录集
	class result_set
	{
	public:
		result_set();
		~result_set();
		bool move_next();
		sqlite3_stmt*& get_stmt();
		std::string get_column_value_text(int col_index);
		std::string get_column_value_text(const std::string& col_name);
		int get_column_value_int(int col_index);
		int get_column_value_int(const std::string& col_name);
		const void * get_column_value_blob(int col_index);
		const void * get_column_value_blob(const std::string& col_name);
		int get_column_bytes(int col_index);
		int get_column_bytes(const std::string& col_name);
		void set_column_info();
	private:
		sqlite3_stmt *m_stmt;
		std::map<std::string, int> m_col_name_to_index;
	};

	// 数据库类
	class sqlite
	{
	public:
		sqlite(const std::string& file_name);
		sqlite();
		~sqlite(void);
		// initialize
		void set_filename(const std::string& file_name);

		// open close
		bool open();
		bool is_open();
		void close();

		// operate
		result_set_ptr get_resultset(const std::string& sql);
		bool exec(const std::string& sql);
		bool begin_trans();
		bool commit();

		// used for data binding
		sqlite3_stmt* prepare_stmt(const std::string& sql);
		bool exec(sqlite3_stmt *stmt); 

		// utils
		bool is_table_exist(const std::string& table_name);

	private:
		std::string m_file_name;
		sqlite3 *m_db;
		bool m_is_open;
	};
}


#endif