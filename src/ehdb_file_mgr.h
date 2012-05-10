#pragma once
#include "ehdb_page.h"

/* Initialize this module
 */
void
ehdb_file_init();

/* This will generate a new page
 * the provided page_t will be modified.
 * return value is the bucket id
 */
int
ehdb_new_page(page_type_t type, int depth);

/* Providing page_t, page_id and page_type,
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

/* split the bucket and
 * return a bucket id
 */
void
ehdb_split_bucket(page_t *page_ptr);

/* generate a link list
 */
int
ehdb_bucket_grow(page_t* page_ptr);
