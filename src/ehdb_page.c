#include "ehdb_page.h"
#include "ehdb_hash.h"
#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"
#define UNUSED -1
#define LINK_POS 2
#define DIRTY_FLAG_POS 5
#define LAST_KEY_POS 6
#define BUCKET_HEAD_LENGTH 16
#define NO_KEY 0

int
ehdb_get_record_num(page_t* page_ptr)
{
    return ((int*)(page_ptr->head))[1];
}

int
ehdb_get_depth(int bucket_id)
{
    page_t *page_ptr = ehdb_get_bucket_page(bucket_id);
    return ((int*)(page_ptr->head))[0];
}

int
ehdb_get_next_bucket(int bucket_id)
{
    page_t *page_ptr = ehdb_get_bucket_page(bucket_id);
    return ((int*)(page_ptr->head))[LINK_POS];
}

void*
ehdb_free_begin(page_t* page_ptr)
{
    return page_ptr->head + 16
        + ehdb_get_record_num(page_ptr) * RECORD_SIZE;
}

void*
ehdb_free_end(page_t* page_ptr)
{
    return page_ptr->head + ((int*)(page_ptr->head))[3];
}

void
ehdb_set_page_link(int src_bucket, int dest_bucket)
{
    page_t* src_page_ptr = ehdb_get_bucket_page(src_bucket);
    src_page_ptr->modified = 1;
    ((int*)(src_page_ptr->head))[LINK_POS] = dest_bucket;
}

int
ehdb_is_dirty(int bucket_id)
{
    page_t* page_ptr = ehdb_get_bucket_page(bucket_id);

    if(ehdb_get_record_num(page_ptr) == 0)
        return 0;

    int offset = BUCKET_HEAD_LENGTH, key;
    int prev = -1;
    record_t record;
    while(offset != -1)
    {
        offset = 
            ehdb_page_record2record(page_ptr, offset, &record);
#ifdef L_HASH
        key = ehdb_get_key(&record);
#endif
#ifdef H_HASH
        key = ehdb_get_invert_key(&record);
#endif
        if(prev == -1)
            prev = key;
        else if(prev != key)
            return 1;
    }
    return 0;
}

/*
void
ehdb_make_dirty(int bucket_id)
{
    page_t* page_ptr = ehdb_get_bucket_page(bucket_id);

    ((int*)(page_ptr->head))[DIRTY_FLAG_POS] = 1;
}

void
ehdb_init_clean(int bucket_id)
{
    page_t* page_ptr = ehdb_get_bucket_page(bucket_id);

    ((int*)(page_ptr->head))[DIRTY_FLAG_POS] = 0;
}
*/
void
ehdb_init_page_free_end(page_t *page_ptr)
{
    ehdb_set_free_end(page_ptr, page_ptr->head+PAGE_SIZE-1);
}

void
ehdb_init_page_record_num(page_t *page_ptr)
{
    ehdb_set_page_record_num(page_ptr, 0);
}

void
ehdb_init_page_link(int bucket_id)
{
    ehdb_set_page_link(bucket_id, 0);
}

void
ehdb_set_page_depth(page_t *page_ptr, int depth)
{
    ((int*)(page_ptr->head))[0] = depth;
    page_ptr->modified = 1;
}

void
ehdb_set_page_record_num(page_t *page_ptr, int record_num)
{
    ((int*)(page_ptr->head))[1] = record_num;
    page_ptr->modified = 1;
}

void
ehdb_set_free_end(page_t *page_ptr, void* free_end)
{
    ((int*)(page_ptr->head))[3] = free_end - page_ptr->head;
    page_ptr->modified = 1;
}

void
ehdb_set_index_map(int hv, int page_id)
{
    page_t *index_page =
        ehdb_get_index_page(hv / Dictpair_per_page);
    ((int*)index_page->head)[hv % Dictpair_per_page] = page_id;
    index_page->modified = 1;
}

int
ehdb_get_index_map(int hv)
{
    page_t *index_page =
        ehdb_get_index_page(hv / Dictpair_per_page);
    return ((int*)index_page->head)[hv % Dictpair_per_page];
}

void
ehdb_init_bucket_page(int bucket_id, int depth)
{
    page_t *page_ptr = 
        ehdb_get_bucket_page(bucket_id);
    page_ptr->page_type = BUCKET;
    ehdb_init_page_free_end(page_ptr);
    ehdb_init_page_record_num(page_ptr);

    ehdb_init_page_link(bucket_id);

    ehdb_set_page_depth(page_ptr, depth);

    page_ptr->modified = 1;
}

void
ehdb_init_index_page(page_t* page_ptr, int page_id)
{
    page_ptr->page_type = INDEX;
    page_ptr->page_id = page_id;
    page_ptr->modified = 1;
}

void
ehdb_copy_page(int src_page, int dest_page)
{
    page_t *src_page_ptr
        = ehdb_get_bucket_page(src_page);
    page_t *dest_page_ptr
        = ehdb_get_bucket_page(dest_page);
    memcpy(dest_page_ptr->head, src_page_ptr->head, PAGE_SIZE);
    dest_page_ptr->modified = 1;
    dest_page_ptr->page_type = src_page_ptr->page_type;
}

int
ehdb_bucket_grow(int bucket_id, int hv)
{
    int depth = ehdb_get_depth(bucket_id);
    int new_bucket = ehdb_new_page(BUCKET, depth);
    ehdb_set_page_link(new_bucket, bucket_id);

    int inc = 1 << depth, i;
    int n = 1 << Global_depth;
    for(i = hv; i < n; i += inc)
    {
        if(ehdb_get_index_map(i) == bucket_id) //TO BE DELETE
            ehdb_set_index_map(i, new_bucket);
    }

    return new_bucket;
}

/*
int
ehdb_get_last_key(int bucket_id)
{
    page_t* page_ptr = ehdb_get_bucket_page(src_bucket);
    page_ptr->modified = 1;
    return ((int*)(page_ptr->head))[LAST_KEY_POS];
}

int
ehdb_set_last_key(int bucket_id, int key)
{
    page_t* page_ptr = ehdb_get_bucket_page(src_bucket);
    page_ptr->modified = 1;
    ((int*)(page_ptr->head))[LAST_KEY_POS] = 0;
}

int
ehdb_init_last_key(int bucket_id)
{
    ehdb_set_last_key(bucket_id, 0);
}
*/
