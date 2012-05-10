#include "ehdb_hash.h"
#include "ehdb_buffer_mgr.h"

int
ehdb_hash_func(int key, int depth)
{
    #ifdef H_HASH
        return ehdb_hash_h(key, depth);
    #endif
    #ifdef L_HASH
        return ehdb_hash_l(key, depth);
    #endif
}

int
ehdb_hash_h(int key, int depth)
{
    //TODO
}

int
ehdb_hash_l(int key, int depth)
{
    /* return key >> depth; */
    return (key & (1 << depth));
}

/* check whether a page is overflowed after the insertion of record
 */
short
is_overflow(page_t* page_ptr, record_t* record)
{
    return (size_t)(ehdb_free_end(page_ptr) - ehdb_free_begin(page_ptr)
            + ehdb_test_record_size(record)) > PAGE_SIZE;
}

void
ehdb_write_record(struct page_t* bucket_page, struct record_t *record)
{
    int hv;
    while(is_overflow(bucket_page, record))
    {
        ehdb_split_bucket(bucket_page);
        hv = ehdb_hash_func(key);
        bucket_page = ehdb_get_bucket_page_by_hvalue(hv);
    }
/*
    page_t * new_bucket_page = ehdb_get_bucket_page(bucket_id);
    if(is_overflow(bucket_page, record) 
        && is_overflow(new_bucket_page, record))
    {
        bucket_id = ehdb_bucket_grow(bucket_page);
        bucket_page = ehdb_get_bucket_page(bucket_id);
    }
*/
    ehdb_record2page_record(record, bucket_page);
}

void
ehdb_double_index(page_t *page_ptr)
{
    int n = (1 << Global_depth);
    int i, j;
    int bucket_id;
    page_t *src_index_page,
           *dest_index_page;

    for(i = 0; i < n; i++){
        // i is the index to be duplicated
        j = n + i;
        src_index_page = ehdb_get_index_page(i / Dictpair_per_page);
        bucket_id = ((int*)src_index_page->head)[i % Dictpair_per_page];

        dest_index_page = ehdb_get_index_page(j / Dictpair_per_page);
        ((int*)dest_index_page->head)[i % Dictpair_per_page] = bucket_id;
        dest_index_page->modified = 1;
    }
}
