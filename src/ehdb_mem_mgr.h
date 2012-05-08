/*
 * This module is responsible for
 * the management of memory.
 */

void *page_ptr;
void *page_head;

/* a simple method to cycle
 * through the pages
 */
void inc_page_ptr(void);

/* initialize the module */
void ehdb_mem_init(void);

/* create an empty page */
void* ehdb_new_page(void);

/* search for a page labeled
 * with the given key
 */
void ehdb_fetch_page(int key, int *is_full);

void*
ehdb_save_string(source_ptr, is_full)
    void *source_ptr,
    int *is_full
;

/* Get the begin of free space */
void*
free_end();

/* Get the end of free space */
void*
free_begin();

/*
void*
ehdb_save_char(source_ptr, is_full)
    void *source_ptr,
    int *is_full
;

void*
ehdb_save_int(source_ptr, is_full)
    void *source_ptr,
    int *is_full
;

void*
ehdb_save_float(source_ptr, is_full)
    void *source_ptr,
    int *is_full
;

void*
ehdb_save_date(source_ptr, is_full)
    void *source_ptr,
    int *is_full
;
*/
