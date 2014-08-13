#include <string.h>
#include <stdlib.h>

#include "../include/pantryc-sqlite.h"

#define PANTRYC_SQLITE__SQL_LENGTH 100

static void pantryc_sqlite__initialize(char *column);

void pantryc__open_database() {
	int result;
	char *filename = (char*) malloc(
			sizeof(char)
					* (strlen(pantryc_global__working_directory)
							+ strlen(PANTRYC_SQLITE__DATABASE) + 1));
	strcpy(filename, pantryc_global__working_directory);
	strcat(filename, PANTRYC_SQLITE__DATABASE);
	result = sqlite3_open(filename, &pantryc_sqlite__database);

	if (result) {
		pantryc_sqlite__database = NULL;
	}
}

void pantryc_sqlite__add_rejected_receipt_address(address)
	char *address; {
	pantryc_sqlite__initialize( PANTRYC_SQLITE__COLUMN_ADDRESS);
	char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
	strcpy(sql, "insert into " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS
	" (" PANTRYC_SQLITE__COLUMN_ADDRESS ")"
	" values ('");
	strcat(sql, address);
	strcat(sql, "');");

	char *error;

	int result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
			&error);
	if (result != SQLITE_OK) {
		return;
	}
}

PantrycList* pantryc_sqlite__get_rejected_receipt_address_list() {
	PantrycList *list = pantryc_list__new();
	char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
	strcpy(sql, "select " PANTRYC_SQLITE__COLUMN_ADDRESS
	" from " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS ";");

	char *error;
	int result =
			sqlite3_exec(pantryc_sqlite__database, sql,
					( {	int $(void *data, int size, char **value, char **column) {
									PantrycList *l = (PantrycList*)data;
									printf("value: %p\n",l->next); // TESTING
									printf("value: %p %s\n",&value[0],value[0]); // TESTING
									pantryc_list__append((PantrycList*)data,value[0] ? value[0] : NULL);
									return 0;}$;}), (PantrycList*) list,
					&error);
	// TESTING
	PantrycList *seek;
	PANTRYC_LIST__TRAVERSE(seek, list)
	{
		printf("add: %s\n", (char*) pantryc_list__get_value(seek));
	}
	////
	if (result != SQLITE_OK) {
		return NULL;
	}
	return list;
}

int pantryc_sqlite__close() {
	return sqlite3_close(pantryc_sqlite__database);
}

/* Private function */
static void pantryc_sqlite__initialize(column)
	char *column; {

	/* Create SQL statement */
	char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
	char *error;

	/* Execute SQL statement */
	strcpy(sql,
			"select * from " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS ";");
	int result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
			&error);
	if (result != SQLITE_OK) {
		// no such table
		strcpy(sql,
				"create table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS
				" (" PANTRYC_SQLITE__COLUMN_ADDRESS " char(" PANTRYC_SQLITE__TEXT_LENGTH
				") primary key not null);");
		result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
				&error);
		if (result != SQLITE_OK) {
		}
		sqlite3_free(error);
	} else {
		strcpy(sql, "select ");
		strcat(sql, column);
		strcat(sql,
				" from " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS ";");
		int result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
				&error);
		if (result != SQLITE_OK) {
			// no such column
			strcpy(sql,
					"alter table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS
					" add column ");
			strcat(sql, column);
			strcat(sql, " char(" PANTRYC_SQLITE__TEXT_LENGTH
			") primary key not null );");
			sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL, &error);
		}
	}
}
