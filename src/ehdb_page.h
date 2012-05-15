#pragma once

enum page_type_t{INDEX, BUCKET};
typedef enum page_type_t page_type_t;

struct page_t{
    page_type_t page_type;
    int page_id;
    void * head;
    short modified;
};
typedef struct page_t page_t;

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
ehdb_get_depth(int bucket_id);

/* get the number of records
 * of a specific page
 */
int
ehdb_get_record_num(page_t* page_ptr);

/* get the strings' length from the back
 */
int
ehdb_get_record_string_len(page_t* page);

/* get to the next bucket
 * if there exits a linked list
 */
int
ehdb_get_next_bucket(int bucket_id);

void
ehdb_set_page_link(int src_bucket, int dest_bucket);

void
ehdb_init_page_free_end(page_t *page_ptr);

void
ehdb_init_page_record_num(page_t *page_ptr);

void
ehdb_init_page_link(int bucket_id);

void
ehdb_set_page_depth(page_t *page_ptr, int depth);

void
ehdb_set_page_record_num(page_t *page_ptr, int record_num);

void
ehdb_set_free_end(page_t *page_ptr, void* free_end);

void
ehdb_set_index_map(int hv, int page_id);

int
ehdb_get_index_map(int hv);

void
ehdb_init_bucket_page(int bucket_id, int depth);

void
ehdb_init_index_page(page_t* page_ptr, int page_id);

void
ehdb_copy_page(int src_page, int dest_page);

int
ehdb_bucket_grow(int bucket_id, int hv);

int
ehdb_is_dirty(int bucket_id);

/*
void
ehdb_make_dirty(int bucket_id);

void
ehdb_init_clean(int bucket_id);

int
ehdb_get_last_key(int bucket_id);

int
ehdb_set_last_key(int bucket_id, int key);

int
ehdb_init_last_key(bucket_id);
*/
