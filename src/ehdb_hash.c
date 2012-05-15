#include "ehdb_hash.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_utils.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_DEPTH 30
#define UNUSED -1

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
    int invert = 0, i;

    for(i = MAX_DEPTH - 1; i >= 0; i--)
        if((key & (1 << i)) > 0)
            break;
    int sig = i;

    //longer
    if(sig >= depth)
        key >>= (sig - depth);
    //shorter
    else
        key <<= (depth - sig);

    key &= ((1 << depth) - 1);

    for(i = 1; i <= depth; i++)
        invert += ((key & (1 << (i - 1))) > 0) << (depth - i);
    return invert;
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
is_overflow(int bucket_id, record_t* record)
{
    page_t *page_ptr = ehdb_get_bucket_page(bucket_id);

    return ((int)ehdb_free_end(page_ptr)
            - (int)ehdb_free_begin(page_ptr) 
            - (int)ehdb_test_record_size(record)) <= 0;
}

void
ehdb_write_record(record_t *record)
{
    int hv, key;

    key = ehdb_get_key(record);
    hv = ehdb_hash_func(key, Global_depth);
    int bucket_id = ehdb_get_index_map(hv);

    // try to write the record into the page
    while(is_overflow(bucket_id, record))
    {
        if(!ehdb_is_dirty(bucket_id))
        {
            bucket_id = ehdb_bucket_grow(bucket_id, hv);
            break;
        }
        //can not write into it, need split the bucket
        ehdb_split_bucket(bucket_id, hv);

        // address the new bucket 
        hv = ehdb_hash_func(key, Global_depth);
        bucket_id = ehdb_get_index_map(hv);
    }
    ehdb_record2page_record(record, bucket_id);
}

void
ehdb_double_index()
{
    //ehdb_statistics();
    int n = (1 << Global_depth);
    int i, j;
    int bucket_id;
    int old_index_id, new_index_id;

    page_t *src_index_page,
           *dest_index_page;

    for(i = 0; i < (n / Dictpair_per_page); i++)
        ehdb_new_page(INDEX, UNUSED);

    for(i = 0; i < n; i++){
        // i is the index to be duplicated
        j = n + i;
        bucket_id = ehdb_get_index_map(i);
        ehdb_set_index_map(j, bucket_id);
    }
    Global_depth++;
}
