# easy_sqlite #
## c++封装的操作sqlite的库，方便操作sqlite。 ##
## 特点： ##
1. 跨平台，支持windows和linux；
2. 不依赖于第三方库，sqlite库除外；
3. 使用简单，源码编译。共两个文件，直接加到自己的项目里面即可；
4. 遵循开源协议，可无限制修改、引用。

##接口介绍##

    // 数据库类
	class sqlite
	{
	public:
		/* 构造函数，传入数据库文件名；或者定义空对象，之后set_filename来设置数据库名。 */
		sqlite(const std::string& file_name);
		sqlite();
		~sqlite(void);
		/* 设置数据库文件名，如果构造函数已经传入了数据库名，该方法不需要调用。 */
		void set_filename(const std::string& file_name);

		/* 打开、关闭 */
		bool open();
		bool is_open();
		void close();

		/* 数据库操作：查询、执行、事务 */
		result_set_ptr get_resultset(const std::string& sql);
		bool exec(const std::string& sql);
		bool begin_trans();
		bool commit();

		/* 数据绑定操作*/
		sqlite3_stmt* prepare_stmt(const std::string& sql);
		bool exec(sqlite3_stmt *stmt); 

		/* 判断表是否存在 */
		bool is_table_exist(const std::string& table_name);
	};

    // 记录集，调用sqlite::get_resultset时返回该类对象的指针，
	class result_set
	{
	public:
		result_set();
		~result_set();
		/* 移动游标到下一条记录，遍历结果集时调用 */
		bool move_next();
		/* 返回stmt指针，进行数据绑定操作的时候会用到 */
		sqlite3_stmt*& get_stmt();
		/* 按下标获取当前行某一列的字符串值 */ 
		std::string get_column_value_text(int col_index);
		/* 按列名获取当前行某一列的字符串值 */
		std::string get_column_value_text(const std::string& col_name);
		/* 按下标获取当前行某一列的整型值 */
		int get_column_value_int(int col_index);
		/* 按列名获取当前行某一列的整型值 */
		int get_column_value_int(const std::string& col_name);
		/* 按下标获取当前行某一列的块值，通常是二进制值 */
		const void * get_column_value_blob(int col_index);
		/* 按列名获取当前行某一列的块值，通常是二进制值 */
		const void * get_column_value_blob(const std::string& col_name);
		/* 按下标获取当前行某一列的大小（字节数） */
		int get_column_bytes(int col_index);
		/* 按列名获取当前行某一列的大小（字节数） */
		int get_column_bytes(const std::string& col_name);
	};

## 代码示例： ##

    #include "easy_sqlite.h"
    #include <iostream>
    
    using namespace std;
    using namespace easy_sqlite;
    
    int main()
    {
		/* 打开数据库文件test.db, 不存在会自动创建 */ 
    	sqlite db("test.db");
		/* 判断表t_student是否存在 */
    	if(!db.is_table_exist("t_student"))
    	{
			/* 创建数据表 */
    		if(!db.exec("create table t_student(f_id int, f_name varchar(20), f_age int)"))
    		{
    			cout << "create table t_student failed" << endl;
    		}
    	}
    	/* 插入数据 */
    	char sql[1024];
    	for(int i = 0; i < 10; i++)
    	{
    		sprintf(sql, "insert into t_student(f_id, f_name, f_age) values(%d,'%s',%d)", i+1, "john", i+20);;
    		if(!db.exec(sql))
    		{
    			cout << "exec sql failed, sql=" << sql << endl;
    		}
    	}
    	/* 查询 */
    	sprintf(sql, "select * from t_student");
    	result_set_ptr rsp = db.get_resultset(sql);
    	if(!rsp.get())
    	{
    		cout << "get result set failed, sql=" << sql << endl;
    		return 1;
    	}
		/* 遍历结果集 */
    	while(rsp->move_next())
    	{
    		cout << rsp->get_column_value_int("f_id") << " "
    			<< rsp->get_column_value_text("f_name") << " "
    			<< rsp->get_column_value_int("f_age") << endl;
    	}
		//
    	return 0;
    }