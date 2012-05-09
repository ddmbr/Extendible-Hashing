/* get the begin of the free space
 * of a specific page
 */
void* 
ehdb_free_begin(struct page_t* page_ptr);

/* get the end of the free space
 * of a specific page
 */
void*
ehdb_free_end(struct page_t* page_ptr);

/* get the local depth
 * of a specific page
 */
int
ehdb_get_depth(struct page_t* page_ptr);

/* get the number of records
 * of a specific page
 */
int
ehdb_get_record_num(struct page_t* page_ptr);

/* get to the next bucket
 * if there exits a linked list
 */
int
ehdb_get_next_bucket(struct page_t* page_ptr);
