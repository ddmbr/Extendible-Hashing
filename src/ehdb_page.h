void *ehdb_free_begin(struct page_t* page_ptr);
void *ehdb_free_end(struct page_t* page_ptr);
int ehdb_get_depth(struct page_t* page_ptr);
int ehdb_get_record_num(struct page_t* page_ptr);
int ehdb_get_next_bucket(struct page_t* page_ptr);
