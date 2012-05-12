#include "ehdb_hash.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_utils.h"
#include <stdlib.h>
#include <stdio.h>

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
    int result = key & ((1 << depth) - 1);
#ifdef DEBUG
    fprintf(stderr, "key: %d, depth: %d, hv: %d\n", key, depth, result);
#endif
    return result;
}

/* check whether a page is overflowed after the insertion of record
 */
short
is_overflow(page_t* page_ptr, record_t* record)
{
#ifdef DEBUG
    /* fprintf(stderr, "page:{begin=%d, end=%d, end-begin=%d, recordsize=%d\n", */
    /*         (int)ehdb_free_begin(page_ptr) - (int)page_ptr->head */
    /*         ,(int)ehdb_free_end(page_ptr)  - (int)page_ptr->head */
    /*         ,(int)ehdb_free_end(page_ptr) - (int)ehdb_free_begin(page_ptr) */
    /*         ,(int)ehdb_test_record_size(record)); */

#endif
    return ((int)ehdb_free_end(page_ptr)
            - (int)ehdb_free_begin(page_ptr) 
            - (int)ehdb_test_record_size(record)) <= 0;
}

void
ehdb_write_record(struct page_t* page_ptr, struct record_t *record)
{
    int hv, key;
    // try to write the record into the page
    while(is_overflow(page_ptr, record))
    {
        //can not write into it, need split the bucket
#ifdef DEBUG
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!page{id=%d, record_num=%d} will OVERFLOW!\n", page_ptr->page_id, ehdb_get_record_num(page_ptr));
#endif
        ehdb_split_bucket(page_ptr, 
                ehdb_hash_func(ehdb_get_key(record), Global_depth));
        // address the new bucket 
        key = ehdb_get_key(record);
        hv = ehdb_hash_func(key, Global_depth);
        page_ptr = ehdb_get_bucket_page_by_hvalue(hv);
    }
    ehdb_record2page_record(record, page_ptr);
    page_ptr->modified = 1;
}

void
ehdb_double_index(page_t *page_ptr)
{
    int n = (1 << Global_depth);
    int i, j;
    int bucket_id;
#ifdef DEBUG
    fprintf(stderr, "INDEX DOUBLE! Global_depth = %d\n, n(index)=%d", Global_depth, n);
#endif
    page_t *src_index_page,
           *dest_index_page;

    for(i = 0; i < n; i++){
        // i is the index to be duplicated
        j = n + i;
        src_index_page = ehdb_get_index_page(i / Dictpair_per_page);
        bucket_id = ((int*)(src_index_page->head))[i % Dictpair_per_page];

        dest_index_page = ehdb_get_index_page(j / Dictpair_per_page);
        ((int*)(dest_index_page->head))[j % Dictpair_per_page] = bucket_id;
        dest_index_page->modified = 1;
#ifdef DEBUG
        fprintf(stderr, "copy value(%d) from index page(id=%d) to page(id=%d)\n",
                bucket_id, src_index_page->page_id, dest_index_page->page_id);
#endif
    }
    Global_depth++;
}
