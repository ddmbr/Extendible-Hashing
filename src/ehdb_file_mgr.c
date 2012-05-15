# include "ehdb_init.h"
# include "ehdb_buffer_mgr.h"
# include "ehdb_page.h"
# include "ehdb_hash.h"
# include "ehdb_record.h"
# include "ehdb_utils.h"
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>

/* #define WRITEMODE ("r+") */
 #define WRITEMODE ("w+") 
#define UNUSED -1

FILE *bucket_file;
FILE *index_file;
int Temp_bucket; /* A bucket which hold temp data */
                 /* while splitting a bucket      */

/* This will generate a new page
 * the provided page_t will be modified.
 */
void
ehdb_file_init()
{
#ifdef DEBUG
    fprintf(stderr, "file mgr initing...\n");
#endif
    bucket_file = fopen("bucket", WRITEMODE);
    index_file = fopen("index", WRITEMODE);
}

void
ehdb_file_buckets_init(){
    page_t *index_page;
    Temp_bucket = ehdb_new_page(BUCKET, 1);
    int bucket_0 = ehdb_new_page(BUCKET, 1);
    int bucket_1 = ehdb_new_page(BUCKET, 1);
    ehdb_new_page(INDEX, UNUSED);
    index_page = ehdb_get_index_page(0);
    ((int*)index_page->head)[0] = bucket_0;
    ((int*)index_page->head)[1] = bucket_1;
    index_page->modified = 1;
}

int
ehdb_new_page(page_type_t type, int depth)
{
#ifdef DEBUG
    fprintf(stderr, "call ehdb_new_page\n");
#endif
    assert(type == INDEX || type == BUCKET);

    page_t *page_ptr;
    page_ptr = ehdb_make_available_page();
    page_ptr->page_type = type;

    if(type == BUCKET)
    {
        page_ptr->page_id = Bucket_page_num;
        Bucket_page_num++;
        ehdb_init_bucket_page(Bucket_page_num - 1, depth);
    }
    else if(type == INDEX)
    {
        ehdb_init_index_page(page_ptr, Index_page_num);
        Index_page_num++;
    }
#ifdef DEBUG
    fprintf(stderr, "new page id=%d created.\n", page_ptr->page_id);
#endif
    return page_ptr->page_id;
}

/* Providing page_t, page_id and page_type,
 * this method will copy the corresponding
 * page from disk to the memory
 */
void
ehdb_copy_from_file(struct page_t *page_ptr)
{
    FILE *file;
#ifdef TRACKIO
    fprintf(stderr, "ehdb_copy_from_file: page id:%d\n", page_ptr->page_id);
#endif

    if(page_ptr->page_type == INDEX)
        file = index_file;
    else if(page_ptr->page_type == BUCKET)
        file = bucket_file;
#ifdef TRACKIO
    {
    int id, pos;
    id = (page_ptr->page_id);
    pos = (page_ptr->page_id) * PAGE_SIZE;
    fprintf(stderr, "fseek id=%d, pos=%d\n", id, pos);
    }
#endif
#ifdef DEBUG
    int id, pos;
    id = (page_ptr->page_id);
    assert(page_ptr->page_type == INDEX || page_ptr->page_type == BUCKET);
    assert(page_ptr->page_type != INDEX || id <= 640);
#endif

    fseek(file, (page_ptr->page_id) * PAGE_SIZE, SEEK_SET);
    fread(page_ptr->head, PAGE_SIZE, 1, file);
    page_ptr->modified = 0;
}

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr)
{
    FILE *file;

    if(page_ptr->page_type == INDEX)
    {
        file = index_file;
        assert((page_ptr->page_id) < Index_page_num);
    }
    else if(page_ptr->page_type == BUCKET)
    {
        file = bucket_file;
        assert((page_ptr->page_id) < Bucket_page_num);
    }
    fseek(file, (page_ptr->page_id) * PAGE_SIZE, SEEK_SET);
    fwrite(page_ptr->head, PAGE_SIZE, 1, file);
}

/* split the bucket and
 */
void 
ehdb_split_bucket(int page_id, int hvalue)
{
    //ehdb_statistics();
    int page_id_h, local_depth;
    int offset;
    struct record_t record;
    local_depth = ehdb_get_depth(page_id);

    //check if local depth = global depth
    if(local_depth == Global_depth)
    {
        ehdb_double_index();
    }

    //depth increase because of splitting
    local_depth++;
    page_t *page_ptr = ehdb_get_bucket_page(page_id);
    ehdb_set_page_depth(page_ptr, local_depth);

    // create a new page on the disk
    page_id_h = ehdb_new_page(BUCKET, local_depth);

    // write the new page's id to index
    int old_index = (((1 << (local_depth-1))-1) & hvalue);
    int new_index = (1 << (local_depth-1))+ old_index;

    int i, n, inc;
    n = 1 << Global_depth;
    inc = 1 << (local_depth - 1);
    for(i = old_index; i <= n; i += inc)
    {
        if(ehdb_get_index_map(i) == page_id)
        {
            if(((i >> (local_depth - 1)) & 1) == 1)
            {
                ehdb_set_index_map(i, page_id_h);
            }
            else
            {
                ehdb_set_index_map(i, page_id);
            }
        }
    }

    //copy the origin bucket to the temp bucket
    ehdb_copy_page(page_id, Temp_bucket);
    //remove all records from the origin bucket
    ehdb_init_bucket_page(page_id, local_depth);

    offset = 16;
    //loop through the page
    int cnt_new = 0;
    int cnt_old = 0;
    while(offset != -1)
    {
        page_ptr = ehdb_get_bucket_page(Temp_bucket); //DEBUG purpose
        offset = ehdb_page_record2record(page_ptr, offset, &record);
        if(ehdb_hash_func(record.orderkey, local_depth)
            == new_index)
        {
            // this record need to move to new bucket
            ehdb_record2page_record(&record, page_id_h);
            cnt_new ++;
        }
        else if(ehdb_hash_func(record.orderkey, local_depth) 
            == old_index)
        {
            ehdb_record2page_record(&record, page_id);
            cnt_old ++;
        }
    }
}

void
ehdb_file_close()
{
    fclose(bucket_file);
    fclose(index_file);
}
