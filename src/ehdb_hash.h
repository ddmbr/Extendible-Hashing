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

short
ehdb_is_overflow(page_t* page_ptr);

/* write a record into a bucket
 */
void
ehdb_write_record(page_t* page_ptr, record_t* record);

void
ehdb_double_index(page_t *page_ptr);
