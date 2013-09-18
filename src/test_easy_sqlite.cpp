#include "easy_sqlite.h"
#include <iostream>

using namespace std;
using namespace easy_sqlite;

int main()
{
	sqlite db("test.db");
	if(!db.is_table_exist("t_student"))
	{
		if(!db.exec("create table t_student(f_id int, f_name varchar(20), f_age int)"))
		{
			cout << "create table t_student failed" << endl;
		}
	}
	// insert
	char sql[1024];
	for(int i = 0; i < 10; i++)
	{
		sprintf(sql, "insert into t_student(f_id, f_name, f_age) values(%d,'%s',%d)", i+1, "john", i+20);;
		if(!db.exec(sql))
		{
			cout << "exec sql failed, sql=" << sql << endl;
		}
	}
	// query
	sprintf(sql, "select * from t_student");
	result_set_ptr rsp = db.get_resultset(sql);
	if(!rsp.get())
	{
		cout << "get result set failed, sql=" << sql << endl;
		return 1;
	}
	while(rsp->move_next())
	{
		cout << rsp->get_column_value_int("f_id") << " "
			<< rsp->get_column_value_text("f_name") << " "
			<< rsp->get_column_value_int("f_age") << endl;
	}
	return 0;
}