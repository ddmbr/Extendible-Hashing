# include "ehdb_init.h"
# include "ehdb_buffer_mgr.h"
# include "ehdb_page.h"
# include "ehdb_hash.h"
# include "ehdb_record.h"
# include "ehdb_utils.h"
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <unistd.h>
# include <fcntl.h>

/* #define WRITEMODE ("r+") */
#define WRITEMODE ("w+")

int fd_bucket, 
    fd_index;
page_t temp_page;
/* This will generate a new page
 * the provided page_t will be modified.
 */

int static
get_fd(char * faddr){
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    fd = open(faddr, O_RDWR|O_CREAT|O_TRUNC, mode);
    if(fd == -1){
        perror("Can not open/create data file");
    }
    posix_fadvise(fd, 0, 0, POSIX_FADV_WILLNEED|POSIX_FADV_RANDOM);
    return fd;
}

void
ehdb_file_init()
{
#ifdef DEBUG
    fprintf(stderr, "file mgr initing...\n");
#endif
    fd_bucket = get_fd("./bucket");
    fd_index = get_fd("./index");
    if(fd_bucket == -1 || fd_index == -1){
        perror("Can not open/create data file.");
        exit(EXIT_FAILURE);
    }

    temp_page.head = malloc(PAGE_SIZE);
}

void
ehdb_file_buckets_init(){
    Index_page_num = 0;
    Bucket_page_num = 0;
    page_t *index_page;
    int bucket_0 = ehdb_new_page(BUCKET, 1);
    int bucket_1 = ehdb_new_page(BUCKET, 1);
    ehdb_new_page(INDEX, -1);
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
        page_num = &Index_page_num;
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
    if(type == BUCKET){
        ehdb_init_page_free_end(page_ptr);
        ehdb_init_page_record_num(page_ptr);
        ehdb_init_page_link(page_ptr);
        ehdb_set_page_depth(page_ptr, depth);
    }
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
    int fd;
#ifdef TRACKIO
    fprintf(stderr, "ehdb_copy_from_file: page id:%d\n", page_ptr->page_id);
#endif

    if(page_ptr->page_type == INDEX)
        fd = fd_index;
    else if(page_ptr->page_type == BUCKET)
        fd = fd_bucket;
#ifdef TRACKIO
    int id, pos;
    id = (page_ptr->page_id);
    pos = (page_ptr->page_id) * PAGE_SIZE;
    fprintf(stderr, "fseek id=%d, pos=%d\n", id, pos);
#endif
#ifdef DEBUG
    int id, pos;
    id = (page_ptr->page_id);
    assert(page_ptr->page_type == INDEX || page_ptr->page_type == BUCKET);
    assert(page_ptr->page_type != INDEX || id <= 640);
#endif

    ehdb_inc_IO_record();
    pread(fd, page_ptr->head, PAGE_SIZE, (page_ptr->page_id) * PAGE_SIZE);
    page_ptr->modified = 0;
}

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr)
{
    int fd;

    if(page_ptr->page_type == INDEX)
        fd = fd_index;
    else if(page_ptr->page_type == BUCKET)
        fd = fd_bucket;

    ehdb_inc_IO_record();
    pwrite(fd, page_ptr->head, PAGE_SIZE, (page_ptr->page_id) * PAGE_SIZE);
}

/* split the bucket and
 */
void 
ehdb_split_bucket(struct page_t *page_ptr, int hvalue)
{
#ifdef DEBUG
    fprintf(stderr, "split a bucket id=%d, type=%d\n", page_ptr->page_id, page_ptr->page_type);
#endif
    int pid_h, depth;
    int page_id = page_ptr->page_id;
    int offset;
    int i, n, inc;
    record_t record;
    depth = ehdb_get_depth(page_ptr);
    //check if local depth = global depth
    if(depth == Global_depth)
    {
        ehdb_double_index(page_ptr);
    }
    page_ptr = ehdb_get_bucket_page(page_id);
    depth++;
    ehdb_set_page_depth(page_ptr, depth);

    page_t *page_l, *page_h;

    // create a new page on the disk
    pid_h = ehdb_new_page(BUCKET, depth);
    page_h = ehdb_get_bucket_page(pid_h);
    page_ptr = ehdb_get_bucket_page(page_id);

    // write the new page's id to index
    page_t *index_page;
#ifdef L_HASH
    int old_index = (((1 << (depth-1))-1) & hvalue);
    /* int old_index = ehdb_hash_func(hvalue, depth-1); */
    int new_index = (1 << (depth - 1)) + old_index;
    inc = 1 << (depth-1);
#elif H_HASH
    int old_index = ehdb_hash_func(hvalue, depth-1) << 1;
    int new_index = old_index + 1;
    inc = 1;
#endif
    int new_key;

#ifdef DEBUG
    fprintf(stderr, "relink index, depth = %d\n", depth);
#endif

    n = 1 << Global_depth;
#ifdef H_HASH
    int j, rlen;
    for(rlen = 1; rlen + depth - 1 <= Global_depth; rlen++)
        for(j = 0; j < (1 << rlen); j++){
            i = (old_index << (rlen-1)) + j;
    /* for(i = 0; i < n; i ++){ */
    /*     j = ehdb_hash_func(i, depth); */
    /*     if(!(j == new_index || j == old_index)) */
    /*         continue; */
#elif L_HASH
    for(i = old_index; i < n; i += inc){
#endif
        //TODO: wrap the set index procedure
        index_page = ehdb_get_index_page(i / Dictpair_per_page);
        if(((int*)index_page->head)[i % Dictpair_per_page] == page_id){
            assert(i >= old_index);
            index_page->modified = 1;

            new_key = ehdb_hash_func(i, depth);
            if(new_key == new_index){
                ((int*)index_page->head)[i % Dictpair_per_page] = pid_h;
            }else if(new_key == old_index){
                ((int*)index_page->head)[i % Dictpair_per_page] = page_id;
            }else{
                fprintf(stderr, "key error when relink");
                exit(EXIT_FAILURE);
            }
        }
    }

    page_ptr = ehdb_get_bucket_page(page_id);
    page_h = ehdb_get_bucket_page(pid_h);
    // page_l is temp page, it will write back to current page later
    page_l = &temp_page;
    page_l->page_id = -1;
    page_l->page_type = BUCKET;
    ehdb_init_page_free_end(page_l);
    ehdb_init_page_record_num(page_l);
    ehdb_init_page_link(page_l);
    ehdb_set_page_depth(page_l, depth);

    offset = 16;
    //loop through the page
#ifdef DEBUG
    int cnt1 = 0;
    int cnt2 = 0;
    fprintf(stderr, "start redistribution of records, depth= %d\n", depth);
#endif
    while(offset != -1)
    {
        offset = ehdb_page_record2record(page_ptr, offset, &record);
#ifdef DEBUG
        fprintf(stderr, "key = %d, hv=%d \n", record.orderkey,ehdb_hash_func(record.orderkey, depth));
#endif
        new_key = ehdb_hash_func(record.orderkey, depth);
        if(new_key == new_index)
        {
            // this record need to move to new bucket
            ehdb_record2page_record(&record, page_h);
#ifdef DEBUG
            fprintf(stderr, " => new page(id=%d)\n", pid_h);
            cnt1++;
#endif
        }
        else if(new_key == old_index)
        {
            ehdb_record2page_record(&record, page_l);
#ifdef DEBUG
            fprintf(stderr, "============ => old page(id=%d)\n", page_id);
            cnt2++;
#endif
        }else{
            fprintf(stderr, "key error");
            ehdb_print_hashindex();
            exit(EXIT_FAILURE);
        }
    }
#ifdef DEBUG
    fprintf(stderr, "cnt:new_bucket = %d, old_bucket = %d\n", cnt1, cnt2);
#endif

    memcpy(page_ptr->head, page_l->head, PAGE_SIZE);
    page_ptr->modified = 1;
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
    close(fd_bucket);
    close(fd_index);
}
