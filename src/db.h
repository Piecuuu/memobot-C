#include <sqlite3.h>
#include <stdio.h>

extern sqlite3* memobot_DB;

sqlite3* memobot_connectDB();
void memobot_init(sqlite3* db);
