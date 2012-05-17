#pragma once
#include "ehdb_page.h"

/* Initialize this module
 */
void
ehdb_file_init();

/* Init 3 buckets, namely, the
 * temp bucket, `0' bucket and `1' bucket.
 * (based on hash values)
 */
void
ehdb_file_buckets_init();

/* This will generate a new page with the
 * specific type and depth(if appropriate)
 * return value is the bucket id
 */
int
ehdb_new_page(page_type_t type, int depth);

/* Providing a page_t pointer,
 * this method will copy the corresponding
 * page from disk to the memory
 */
void
ehdb_copy_from_file(struct page_t *page_ptr);

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(page_t *page_ptr);

/* split the bucket
 */
void
ehdb_split_bucket(int page_id, int hvalue);

/* Close all files that have been
 * previously opened.
 */
void
ehdb_file_close();
