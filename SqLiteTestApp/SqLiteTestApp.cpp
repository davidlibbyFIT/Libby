#include <iostream>
#include "sqlite3.h"
#include "DebugTimer.h"

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	SetDebugTimer();
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void RunSQLAtomicTransaction(sqlite3 *db)
{
	SetDebugTimer();
	if (!db)
		return;

	const char *pSQL[6];
	char *zErrMsg = 0;

	// Create a new myTable in database
	pSQL[0] = "create table myTable (FirstName varchar(30), LastName varchar(30), Age smallint not null)";

	// Insert first data item into myTable
	pSQL[1] = "insert into myTable (FirstName, LastName, Age) values ('Woody', 'Alan', 45)";

	// Insert second data item into myTable
	pSQL[2] = "insert into myTable (FirstName, LastName, Age) values ('Micheal', 'Bay', 38)";

	// Select all data in myTable
	pSQL[3] = "select * from myTable";

	// start sqlite transaction block
	sqlite3_exec(db, "BEGIN", 0, 0, 0);

	// execute all the sql statements
	for(int i = 0; i < 4; i++)
	{
		int rc = sqlite3_exec(db, pSQL[i], callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);

			// rollback all update/insert to sqlite 
			sqlite3_exec(db, "ROLLBACK", 0, 0, 0);

			break; // break the loop if error occur
		}
	}

	// commit all to sqlite
	sqlite3_exec(db, "COMMIT", 0, 0, 0);



	
	//HUGE Atomic Commit.
	// start sqlite transaction block
	sqlite3_exec(db, "BEGIN", 0, 0, 0);
	
	char mySQL[600];
	mySQL[0]='\0';


	//DebugTimer MyTime("Inside SqlLoop");

	// execute all the sql statements
	for(int i = 0; i < 1000000; i++)
	{
		Utils::DebugTimer myTimer("In TightLoop");
		
		sprintf_s(mySQL,sizeof(mySQL),"insert into myTable (FirstName, LastName, Age) values ('David %d', 'Libby %d', %d)",i,i,i);

		if(i % 100000 == 0)
		{
			fprintf(stdout, "\tCount = %d\n",i);
			
		}


		int rc = sqlite3_exec(db, mySQL, callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);

			// rollback all update/insert to sqlite 
			sqlite3_exec(db, "ROLLBACK", 0, 0, 0);

			break; // break the loop if error occur
		}
	}
	// commit all to sqlite
	sqlite3_exec(db, "COMMIT", 0, 0, 0);

}


//rc = sqlite3_open("c:\\work\\libby\\test.db", &db);
int main( int argc,      // Number of strings in array argv
	char *argv[],   // Array of command-line argument strings
	char *envp[] )  // Array of environment variable strings
{
	SetDebugTimer();
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("c:\\work\\libby\\test.db", &db);
	//rc = sqlite3_open(":memory:", &db);

	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE COMPANY("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"NAME           TEXT    NOT NULL," \
		"AGE            INT     NOT NULL," \
		"ADDRESS        CHAR(50)," \
		"SALARY         REAL );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}

	/* Create SQL statement */
	sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
		"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
		"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
		"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
		"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Records created successfully\n");
	}

	/* Create SQL statement */
	sql = "SELECT * from COMPANY";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}


	/* Create merged SQL statement */
	sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
		"SELECT * from COMPANY";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	/* Create merged SQL statement */
	sql = "DELETE from COMPANY where ID=2; " \
		"SELECT * from COMPANY";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}

	RunSQLAtomicTransaction(db);
	sqlite3_close(db);
	Utils::DebugTimer myTimer("Total");
	myTimer.ReportToDebugTerm();
	return 0;}