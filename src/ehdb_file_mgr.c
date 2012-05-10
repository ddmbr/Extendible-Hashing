# include "ehdb_init.h"

/* This will generate a new page
 * the provided page_t will be modified.
 */
void ehdb_file_init()
{
    Bucket_page_num = 0;
    Index_page_num = 0;
}

int
ehdb_new_page(page_type_t type, int depth)
{
    int page_num;
    if(type == INDEX)
        page_num = &Index_page_num;
    else
        page_num = &Bucket_page_num;
    *page_num ++;

    struct page_t* page_ptr;

    page_ptr = ehdb_make_available_page();

    page_ptr->page_type = type;
    page_ptr->page_id = page_num;
    page_ptr->modified = 0;
    ehdb_init_page_free_end(page_ptr);
    ehdb_init_page_record_num(page_ptr);
    ehdb_init_page_link(page_ptr);
    ehdb_set_page_depth(page_ptr);

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

    if(page_ptr->page_type == INDEX)
        file = fopen("index", "w");
    else if(page_ptr->page_type == BUCKET)
        file = fopen("buckets", "w");

    fseek(file, (page_ptr->page_id - 1) * Page_size, SEEK_SET);
    fread(page_ptr, Page_size, 1, file);
}

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr)
{
    FILE *file;

    if(page_ptr->page_type == INDEX)
        file = fopen("index", "w");
    else if(page_ptr->page_type == BUCKET)
        file = fopen("buckets", "w");

    fseek(file, (page_ptr->page_id - 1) * Page_size, SEEK_SET);
    fwrite(page_ptr, Page_size, 1, file);
}

/* split the bucket and
 * return a bucket id
 */
int
ehdb_split_bucket(struct page_t *page_ptr);
{
    int hv_l, hv_h, pid_l, pid_h, depth;

    depth = ehdb_get_depth(page_ptr);
    depth++;

    pid_l = ehdb_new_page(BUCKET, depth);
    pid_h = ehdb_new_page(BUCKET, depth);
    page_t *page_l, *page_h;
    page_l = ehdb_get_bucket_page(pid_l);
    page_h = ehdb_get_bucket_page(pid_h);

    //loop through the page
    counter = ehdb_get_record_num(page_ptr);
    size_t offset = 16;
    struct record_t record;
    while(counter--)
    {
        offset = ehdb_page_record2record(page_ptr, offset, &record);
        if(record->orderkey & (1 << (depth - 1)))
        {
            ehdb_record2page_record(&record, page_h);
            hv_h = ehdb_hash_func(record->orderkey);
        }
        else
        {
            ehdb_record2page_record(&record, page_l);
            hv_l = ehdb_hash_func(record->orderkey);
        }
    }

    //check if local depth > global depth
    if(ehdb_get_depth(page_ptr) > Global_depth)
    {
        ehdb_double_index(page_ptr);
    }
    index_page = ehdb_get_index_page(hv_l / Dictpair_per_page);
    ((int*)index_page->head)[hv_l % Dictpair_per_page] = page_l;
    index_page_l->modified = 1;

    index_page = ehdb_get_index_page(hv_h / Dictpair_per_page);
    ((int*)index_page->head)[hv_h % Dictpair_per_page] = page_h;
    index_page->modified = 1;
}

int
ehdb_bucket_grow(struct page_t* page_ptr)
{
    int prev_id = page_ptr->page_id;
    int bucket_id = ehdb_new_page(page_ptr);
    page_ptr = ehdb_get_bucket_page(bucket_id);
    ehdb_set_page_link(page_ptr, prev_id);

    return bucket_id;
}
