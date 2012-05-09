/* This will generate a new page
 * the provided page_t will be modified.
 */
void
ehdb_new_page(struct page_t *page_ptr);

/* Providing page_t, page_id and page_type,
 * this method will copy the corresponding
 * page from disk to the memory
 */
void
ehdb_copy_from_file(struct page_t *page_ptr, int page_id, int page_type);

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr);

/* split the bucket and
 * return a bucket id
 */
int
ehdb_split_bucket();
