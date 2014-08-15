#ifndef PANTRYC_SQLITE_H_
#define PANTRYC_SQLITE_H_

#include <sqlite3.h>

#include <pantryc-global.h>
#include <pantryc-list.h>

#define PANTRYC_SQLITE__DATABASE								"PantrycDatabase.db"
#define PANTRYC_SQLITE__TABLE_REJECTED_RECEIPT_ADDRESS			"RejectedReceiptAddress"
#define PANTRYC_SQLITE__COLUMN_ADDRESS							"Address"

#define PANTRYC_SQLITE__TEXT_LENGTH								"50"

sqlite3 *pantryc_sqlite__database;

/**
 * Open Pantryc Database
 * return			open successfully or not
 */
pBOOL pantryc_sqlite__open_database();

/**
 * Add an address to rejected recipients list
 * @address:		rejected address to add
 */
void pantryc_sqlite__add_rejected_receipt_address(char *address);

/**
 * Remove an address from rejected recipients list
 * @address:		rejected address to remove
 */
void pantryc_sqlite__remove_rejected_receipt_address(char *address);

/**
 * Get rejected receipt address list
 * @address			address to check
 * return			address is rejected or not
 */
pBOOL pantryc_sqlite__check_rejected_receipt_address_list(char *address);

/**
 * Close Pantryc Database
 * return			open successfully or not
 */
pBOOL pantryc_sqlite__close_database();

#endif /* PANTRYC_SQLITE_H_ */
