int ehdb_hash_l(int key);
int ehdb_hash_h(int key);
int ehdb_is_overflow(struct page_t* page_ptr);
struct page_t* ehdb_bucket_grow(int bucket_id);
int ehdb_add_record(struct page_t* page_ptr, struct record_t *record);
