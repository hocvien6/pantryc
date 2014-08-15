#include <string.h>
#include <stdlib.h>

#include "../include/pantryc-sqlite.h"

#define PANTRYC_SQLITE__SQL_LENGTH 100

static void pantryc_sqlite__initialize(char *column);

pBOOL pantryc_sqlite__open_database() {
	int result;
	char *filename = (char*) malloc(
			sizeof(char)
					* (strlen(pantryc_global__working_directory)
							+ strlen(PANTRYC_SQLITE__DATABASE) + 1));
	strcpy(filename, pantryc_global__working_directory);
	strcat(filename, PANTRYC_SQLITE__DATABASE);
	result = sqlite3_open(filename, &pantryc_sqlite__database);

	if (result) {
		fprintf(pantryc_global__log_file,
				"**ERROR**		Cannot open database " PANTRYC_SQLITE__DATABASE "\n");
		pantryc_sqlite__database = NULL;
		return pFALSE;
	}
	return pTRUE;
}

void pantryc_sqlite__add_rejected_receipt_address(address)
	char *address; {
	if (address != NULL) {
		pantryc_sqlite__initialize(PANTRYC_SQLITE__COLUMN_ADDRESS);
		char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
		strcpy(sql,
				"insert into " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS
				" (" PANTRYC_SQLITE__COLUMN_ADDRESS ")"
				" values ('");
		strcat(sql, address);
		strcat(sql, "');");

		char *error;
		int result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
				&error);
		if (result != SQLITE_OK) {
			fprintf(pantryc_global__log_file,
					"**ERROR**		Cannot add rejected receipt address, %s\n",
					error);
			sqlite3_free(error);
			return;
		} else {
			fprintf(pantryc_global__log_file,
					"Add address %s to column " PANTRYC_SQLITE__COLUMN_ADDRESS
					" on table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS "\n",
					address);
		}
		sqlite3_free(error);
		return;
	}
}

void pantryc_sqlite__remove_rejected_receipt_address(address)
	char *address; {
	pantryc_sqlite__initialize(PANTRYC_SQLITE__COLUMN_ADDRESS);
	char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
	strcpy(sql, "delete from " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS
	" where " PANTRYC_SQLITE__COLUMN_ADDRESS " = '");
	strcat(sql, address);
	strcat(sql, "';");

	char *error;
	int result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
			&error);
	if (result != SQLITE_OK) {
		fprintf(pantryc_global__log_file,
				"**ERROR**		Cannot remove rejected receipt address, %s\n",
				error);
		sqlite3_free(error);
		return;
	} else {
		fprintf(pantryc_global__log_file,
				"Remove address %s from column " PANTRYC_SQLITE__COLUMN_ADDRESS
				" on table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS "\n",
				address);
	}
	sqlite3_free(error);
	return;
}

PantrycList* pantryc_sqlite__get_rejected_receipt_address_list() {
	pantryc_sqlite__initialize(PANTRYC_SQLITE__COLUMN_ADDRESS);
	PantrycList *list = pantryc_list__new();
	char *sql = (char*) malloc(sizeof(char) * PANTRYC_SQLITE__SQL_LENGTH);
	strcpy(sql, "select " PANTRYC_SQLITE__COLUMN_ADDRESS
	" from " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS ";");

	char *error;
	int result =
			sqlite3_exec(pantryc_sqlite__database, sql,
					( {	int $(void *data, int size, char **value, char **column) {
									pantryc_list__append((PantrycList*) data, value[0] ? strdup(value[0]) : NULL);
									return 0;}$;}), (void*) list, &error);
	if (result != SQLITE_OK) {
		fprintf(pantryc_global__log_file,
				"**ERROR**		Cannot get rejected receipt address list, %s\n",
				error);
		return NULL;
	}
	return list;
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
			fprintf(pantryc_global__log_file,
					"**ERROR**		Cannot initialize, %s\n", error);
			sqlite3_free(error);
			return;
		} else {
			fprintf(pantryc_global__log_file,
					"**WARNING**	Create table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS "\n");
		}
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
			result = sqlite3_exec(pantryc_sqlite__database, sql, NULL, NULL,
					&error);
			if (result != SQLITE_OK) {
				fprintf(pantryc_global__log_file,
						"**ERROR** Cannot initialize, %s\n", error);
				sqlite3_free(error);
				return;
			} else {
				fprintf(pantryc_global__log_file,
						"Add column %s"
								" on table " PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS "\n",
						column);
			}
		}
	}
	sqlite3_free(error);
	return;
}
