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
ehdb_get_depth(struct page_t* page_ptr);

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
page_t*
ehdb_get_next_bucket(page_t* page_ptr);

void
ehdb_set_page_link(page_t* page_ptr, int page_id);

void
ehdb_init_page_free_end(page_t *page_ptr);

void
ehdb_init_page_record_num(page_t *page_ptr);

void
ehdb_init_page_link(page_t *page_ptr);

void
ehdb_set_page_depth(page_t *page_ptr, int depth);

void
ehdb_set_page_record_num(page_t *page_ptr, int record_num);

void
ehdb_set_free_end(page_t *page_ptr, void* free_end);
