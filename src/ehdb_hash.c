#include "ehdb_hash.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_utils.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_DEPTH 19

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
    int invert_key = 0, i;
    for(i = 1; i <= MAX_DEPTH; i++)
        invert_key += ((key & (1 << (i - 1))) > 0) << (MAX_DEPTH - i);
    return ehdb_hash_l(invert_key, depth);
}

int
ehdb_hash_l(int key, int depth)
{
    int result = key & ((1 << depth) - 1);
#ifdef DEBUG
    fprintf(stderr, "ehdb_hash_l: key: %d, depth: %d, hv: %d\n", key, depth, result);
#endif
    return result;
}

/* check whether a page is overflowed after the insertion of record
 */
short
is_overflow(page_t* page_ptr, record_t* record)
{
#ifdef DEBUG
    fprintf(stderr, "page:{begin=%d, end=%d, end-begin=%d, recordsize=%d\n",
            (int)ehdb_free_begin(page_ptr) - (int)page_ptr->head
            ,(int)ehdb_free_end(page_ptr)  - (int)page_ptr->head
            ,(int)ehdb_free_end(page_ptr) - (int)ehdb_free_begin(page_ptr)
            ,(int)ehdb_test_record_size(record));

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
#ifdef DEBUG
    if(page_ptr == 0)
        fprintf(stderr, "NULL POINTER! hv = %d\n", hv);
#endif
        ehdb_split_bucket(page_ptr, 
                ehdb_hash_func(ehdb_get_key(record), Global_depth));
        // address the new bucket 
        key = ehdb_get_key(record);
        hv = ehdb_hash_func(key, Global_depth);
        page_ptr = ehdb_get_bucket_page_by_hvalue(hv);
    }
    ehdb_record2page_record(record, page_ptr);
}

int
ehdb_get_bucket_id_by_hvalue(int hvalue){
    page_t * index_page = load_page(hvalue / Dictpair_per_page, INDEX);
    int bucket_id = ((int *)(index_page->head))[hvalue % Dictpair_per_page];
    return bucket_id;
}

void
ehdb_double_index(page_t *page_ptr)
{
    // TODO: implement a fast version
    int n = (1 << Global_depth);
    int i, j;
    int bucket_id;
    int old_index_id, new_index_id;
    int n1 = n;
#ifdef DEBUG
    fprintf(stderr, "INDEX DOUBLE! Global_depth = %d, n(index)=%d\n", Global_depth, n);
#endif
    page_t *src_index_page,
           *dest_index_page;

    for(i = 0; i < n; i++){
        // i is the index to be duplicated
        j = n + i;
        old_index_id = i / Dictpair_per_page;
        src_index_page = ehdb_get_index_page(old_index_id);
        bucket_id = ((int*)(src_index_page->head))[i % Dictpair_per_page];

        new_index_id = j / Dictpair_per_page;
        if(new_index_id >= n1){
            // create new index page on disk
            ehdb_new_page(INDEX, new_index_id);
            n1 ++;
        }
        dest_index_page = ehdb_get_index_page(new_index_id);
        ((int*)(dest_index_page->head))[j % Dictpair_per_page] = bucket_id;
        dest_index_page->modified = 1;
    }
    Global_depth++;
}
