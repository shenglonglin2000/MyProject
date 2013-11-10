#include <iostream>

#include <cstdio>
#include <cstdlib>
#include "sqlite3.h"




int testdb_nomalfun(void);
int testdb_callbackfun(void);
int testdb_memoryfun(void);



//callback回调函数 _callback_exec的编写，其中notused为sqlite3_exec中的第四个参数，
//第二个参数是栏的数目，第三个是栏的名字，第四个为查询得到的值
static int _callback_exec(void * notused,int argc, char ** argv, char ** aszColName)
{
	int i;
	for ( i=0; i<argc; i++ )
	{
		printf( "%s = %s/n", aszColName[i], argv[i] == 0 ? "NUL" : argv[i] );
	}
	return 0;
}


int main(int argc, char* argv[])
{
	//文件模式
	testdb_nomalfun();
	testdb_callbackfun();
	testdb_memoryfun();
	return 0;
}


int testdb_nomalfun(void)
{
	sqlite3 *db=NULL;
	char *zErrMsg = 0;
	int rc;
	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	rc = sqlite3_open("test1.db", &db);
	if(rc)
	{
		fprintf(stderr, "Can’t open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return (1);
	}
	else
	{
		printf("You have opened a sqlite3 database named test1.db successfully!\n");
	}



	//创建一个表,如果该表存在，则不创建，并给出提示信息，存储在zErrMsg 中
	char *sql = " CREATE TABLE test(ID INTEGER PRIMARY KEY,  AGE INTEGER,  LEVEL INTEGER,  NAME VARCHAR(12), SALARY REAL);" ;
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );

	//插入数据
	sql = "INSERT INTO test VALUES ( NULL, 1, 1, '201205151206', 117.9 );" ;
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
	int nrow = 0;
	int ncolumn = 0; //查询结果集的行数、列数
	char **azResult; //二维数组存放结果

	//查询数据
	sql = "SELECT * FROM test ";
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
	int i = 0 ;
	printf( "row:%d column=%d \n" , nrow , ncolumn );
	printf( "\nThe result of querying is : \n" );
	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
	{
		printf( "azResult[%d] = %s\n", i , azResult[i] );
	}

	//删除数据
	sql = "DELETE FROM test WHERE AGE = 1 ;" ;
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );

	//释放掉azResult 的内存空间
	sqlite3_free_table( azResult );
	sqlite3_close(db); //关闭数据库

	int c=getchar();

	return 0;
}

int testdb_callbackfun(void)
{
	const char * sSQL = "select * from t1;";
	char * pErrMsg = 0;
	int ret = 0;
	sqlite3 * db = 0;
	ret = sqlite3_open("./test2.db", &db);
	if ( ret != SQLITE_OK )
	{
		fprintf(stderr, "Could not open database: %s", sqlite3_errmsg(db));
		exit(1);
	}

	printf("Successfully connected to database/n");
	sqlite3_exec( db, sSQL, _callback_exec, 0, &pErrMsg );

	if ( ret != SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s/n", pErrMsg);
		sqlite3_free(pErrMsg);
	}

	sqlite3_close(db);
	db = 0;

	return 0;
}

int testdb_memoryfun(void)
{
	const char * sSQL = "select * from t1;";
	char * pErrMsg = 0;
	int ret = 0;
	sqlite3 * db = 0;
	ret = sqlite3_open(":memory:", &db);
	if ( ret != SQLITE_OK )
	{
		fprintf(stderr, "Could not open database: %s", sqlite3_errmsg(db));
		exit(1);
	}

	printf("Successfully connected to database/n");
	char *zErrMsg = 0;
	char *sql = " CREATE TABLE test(ID INTEGER PRIMARY KEY,AGE INTEGER,LEVEL INTEGER,NAME VARCHAR(12),SALARY REAL);" ;
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );

	//插入数据
	sql = "INSERT INTO test VALUES(NULL , 1 , 1 , '201205151206', 20.9 );" ;
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );
	int nrow = 0, ncolumn = 0; //查询结果集的行数、列数
	char **azResult; //二维数组存放结果

	//查询数据
	sql = "SELECT * FROM test ";
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &zErrMsg );
	int i = 0 ;
	printf( "row:%d column=%d \n" , nrow , ncolumn );
	printf( "\nThe result of querying is : \n" );

	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )
	{
		printf( "azResult[%d] = %s\n", i , azResult[i] );
	}

	//释放掉azResult 的内存空间
	sqlite3_free_table( azResult );
	sqlite3_close(db); //关闭数据库

	return 0;
}



