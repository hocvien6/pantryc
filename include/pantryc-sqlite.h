#ifndef PANTRYC_SQLITE_H_
#define PANTRYC_SQLITE_H_

#include <sqlite3.h>

#define PANTRYC_SQLITE__DATABASE								"PantrycDatabase.db"
#define PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS			"RejectedReceiptAddress"
#define PANTRYC_SQLITE__COLUMN_ADDRESS							"Address"
#define PANTRYC_SQLITE__TEXT_LENGTH								"50"

sqlite3 *pantryc_sqlite__database;

/**
 * Open Pantryc Database
 */
void pantryc__open_database();

/**
 * Add an address to rejected recipients list
 * @address:		rejected address to add
 * @database:		pantryc database
 */
void pantryc_sqlite__add_rejected_receipt_address(char *address);

/**
 * Close database
 */
void pantryc_sqlite__close();

#endif /* PANTRYC_SQLITE_H_ */