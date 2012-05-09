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

/* check whether a page is overflowed
 */
short
ehdb_is_overflow(struct page_t* page_ptr);

/* write a record into a bucket
 */
void
ehdb_write_record(struct page_t* page_ptr, struct record_t record*);

void
ehdb_double_index(struct page_t *page_ptr);
