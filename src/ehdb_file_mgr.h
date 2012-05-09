/* This will generate a new page
 * the provided page_t will be modified.
 */
void
ehdb_new_page(struct page_t *page_ptr);

/* Providing page_id and page_t,
 * this method will copy the corresponding
 * page from disk to the memory
 */
void
ehdb_copy_from_file(struct page_t *page_ptr, int page_id);
