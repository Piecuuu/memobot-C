#include "db.h"

sqlite3* memobot_DB;

sqlite3* memobot_connectDB() {
  sqlite3 *db;
  int rc = sqlite3_open("memobot.db", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return NULL;
  }

  memobot_DB = db;
  return db;
}

void memobot_init(sqlite3* db) {
  char* sql = "CREATE TABLE IF NOT EXISTS posts ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
    "userid text NOT NULL,"
    "messageid text NOT NULL,"
    "mediaurl text,"
    "isposted boolean"
    ")";
  sqlite3_exec(db, sql, 0, 0, NULL);
}
