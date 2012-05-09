/* The hash function, 
 * begin from LSB
 */
int
ehdb_hash_l(int key);

/* The hash function, begin from MSB
 */
int
ehdb_hash_h(int key);

/* check whether a page is overflowed
 */
int
ehdb_is_overflow(struct page_t* page_ptr);

/* generate a link list
 */
struct page_t*
ehdb_bucket_grow(int bucket_id);
