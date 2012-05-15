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
ehdb_is_overflow(int bucket_id, record_t* record);

/* write a record into a bucket
 */
void
ehdb_single_insert(record_t* record);

void
ehdb_double_index();

int
ehdb_get_bucket_id_by_hvalue(int hvalue);

/* split the bucket and
 * return a bucket id
 */
void
ehdb_split_bucket(int page_id, int hvalue);

