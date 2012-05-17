#pragma once
#include "ehdb_page.h"
#include "ehdb_record.h"

/* The main hash function
 */
int
ehdb_hash_func(int key, int depth);

/* The hash function, begin from LSB
 */
int
ehdb_hash_l(int key, int depth);

/* The hash function, begin from MSB
 */
int
ehdb_hash_h(int key, int depth);


/* check whether a page will be overflowed after
 * the insertion of the specific record
 */
short
ehdb_is_overflow(int bucket_id, record_t* record);

/* Write a record into a bucket.
 * It can handle the situation where
 * a bucket get full.
 */
void
ehdb_write_record(record_t* record);

/* double the index. notice that
 * it can also make a summary about
 * the database when STAT(see manual) is enable.
 */
void
ehdb_double_index();
