#pragma once
#include "ehdb_record.h"
#include "ehdb_page.h"

/* this function must be called
 * before calling ehdb_next_line()
 */
void
ehdb_parse_start(char * fileaddr);

/* get next line in the file,
 * convert and store in record
 */
void
ehdb_next_line(record_t* record);

/* test whether end-of-file is reached
 */
int
ehdb_test_eof();

/* load data from file
 * and build the database
 */
void
ehdb_bulk_insert(char * fileaddr);
