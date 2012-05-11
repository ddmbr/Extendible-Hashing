# include "ehdb_init.h"
# include "ehdb_buffer_mgr.h"
# include "ehdb_page.h"
# include "ehdb_record.h"
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>

/* #define WRITEMODE ("r+") */
#define WRITEMODE ("w+")

FILE *bucket_file;
FILE *index_file;
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
    int bucket_0 = ehdb_new_page(BUCKET, 1);
    int bucket_1 = ehdb_new_page(BUCKET, 1);
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
    int *page_num;
#ifdef DEBUG
    assert(type == INDEX || type == BUCKET);
#endif

    if(type == INDEX)
    {
        //page_num = &Index_page_num;
        //Necessary?
    }
    else if(type == BUCKET)
    {
        page_num = &Bucket_page_num;
    }
    (*page_num) ++;

    page_t *page_ptr;

    page_ptr = ehdb_make_available_page();

    page_ptr->page_type = type;
    page_ptr->page_id = *page_num - 1;
    page_ptr->modified = 0;
    ehdb_init_page_free_end(page_ptr);
    ehdb_init_page_record_num(page_ptr);
    ehdb_init_page_link(page_ptr);
    ehdb_set_page_depth(page_ptr, depth);
    page_ptr->modified = 1;

#ifdef DEBUG
    char* ffk[2] = {"INDEX", "BUCKET"};
    fprintf(stderr, "new page created, page{id=%d, type=%s\n",
            page_ptr->page_id, ffk[page_ptr->page_type]);
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
#ifdef DEBUG
    fprintf(stderr, "ehdb_copy_from_file: page id:%d\n", page_ptr->page_id);
#endif

    if(page_ptr->page_type == INDEX)
        file = index_file;
    else if(page_ptr->page_type == BUCKET)
        file = bucket_file;

    fseek(file, (page_ptr->page_id) * PAGE_SIZE, SEEK_SET);
    fread(page_ptr->head, PAGE_SIZE, 1, file);
}

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr)
{
    FILE *file;

    if(page_ptr->page_type == INDEX)
        file = index_file;
    else if(page_ptr->page_type == BUCKET)
        file = bucket_file;

    fseek(file, (page_ptr->page_id) * PAGE_SIZE, SEEK_SET);
    fwrite(page_ptr->head, PAGE_SIZE, 1, file);
}

/* split the bucket and
 * return a bucket id
 */
/* The split algorithm
 * TODO
 *
 */
void
ehdb_split_bucket(struct page_t *page_ptr, int hvalue)
{
    int pid_h, depth;
    int offset;
    struct record_t record;
#ifdef DEBUG
    fprintf(stderr, "2222222222222 now split a bucket(page{id=%d})\n", page_ptr->page_id);
#endif
    depth = ehdb_get_depth(page_ptr);
    //check if local depth = global depth
    if(depth == Global_depth)
    {
#ifdef DEBUG
        fprintf(stderr, "222222222222222 gdepth==ldepth=%d, need double index\n", Global_depth);
#endif
        ehdb_double_index(page_ptr);
    }
    depth++;
    ehdb_set_page_depth(page_ptr, depth);

    page_t *page_l, *page_h;

    // create a new page on the disk
    pid_h = ehdb_new_page(BUCKET, depth);
    page_h = ehdb_get_bucket_page(pid_h);
#ifdef DEBUG
    fprintf(stderr, "2222222222222222 new bucket(page{id=%d})\n", page_h->page_id);
#endif

    // write the new page's id to index
    page_t *index_page;
    int new_index = (1 << (depth-1))+(((1 << (depth-1))-1) & hvalue);
    index_page = ehdb_get_index_page(new_index / Dictpair_per_page);
    ((int*)index_page->head)[new_index % Dictpair_per_page] = page_h->page_id;
    index_page->modified = 1;

    // page_l is temp page, it will write back to current page later
    page_l = (page_t*)malloc(sizeof(page_t));
    page_l->head = malloc(PAGE_SIZE);
    ehdb_init_page_free_end(page_l);
    ehdb_init_page_record_num(page_l);
    ehdb_init_page_link(page_l);
    ehdb_set_page_depth(page_l, depth);

    offset = 16;
    //loop through the page
    while(offset != -1)
    {
#ifdef DEBUG
        fprintf(stderr, "offset = %d\n", offset);
#endif
        offset = ehdb_page_record2record(page_ptr, offset, &record);
        if(ehdb_hash_func(record.orderkey, depth) 
                != ehdb_hash_func(record.orderkey, depth - 1))
        {
            // this record need to move to new bucket
            ehdb_record2page_record(&record, page_h);
        }
        else
        {
            ehdb_record2page_record(&record, page_l);
        }
    }

    memcpy(page_ptr->head, page_l->head, PAGE_SIZE);
    page_ptr->modified = 1;

    free(page_l->head);
    free(page_l);
}

int
ehdb_bucket_grow(struct page_t* page_ptr)
{
    int prev_id = page_ptr->page_id;
    int bucket_id = ehdb_new_page(BUCKET, ehdb_get_depth(page_ptr));
    page_ptr = ehdb_get_bucket_page(bucket_id);
    ehdb_set_page_link(page_ptr, prev_id);

    return bucket_id;
}

void
ehdb_file_close()
{
    fclose(bucket_file);
    fclose(index_file);
}
