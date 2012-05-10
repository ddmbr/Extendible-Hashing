/* This will generate a new page
 * the provided page_t will be modified.
 */
int
ehdb_new_page(page_type_t type)
{
    Page_num += 1;
    struct page_t* page_ptr;

    //TODO ray's
    page_ptr = ehdb_available_page();

    page_ptr->page_type = type;
    page_ptr->page_id = Page_num;
    page_ptr->modified = 0;
    ehdb_init_page_free_end(page_ptr);
    ehdb_init_page_depth(page_ptr);
    ehdb_init_page_record_num(page_ptr);
    ehdb_init_page_link(page_ptr);

    return page_ptr->page_id;
}

/* Providing page_t, page_id and page_type,
 * this method will copy the corresponding
 * page from disk to the memory
 */
void
ehdb_copy_from_file(struct page_t *page_ptr, int page_id, int page_type);

/* this method will save the corresponding
 * page from memory to disk
 */
void
ehdb_save_to_file(struct page_t *page_ptr)
{
    FILE *file;

    if(page_ptr->page_type == INDEX)
        file = fopen("index", "w");
    else if(page_ptr->page_type == INDEX)
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
    int hv_l, hv_h, pid_l, pid_h;

    //inc bucket depth TODO
    ehdb_inc_bucket_depth(page_ptr);

    //loop through the page
    counter = ehdb_get_record_num(page_ptr);
    size_t offset = 16;
    struct record_t record;
    while(counter--)
    {
        offset = ehdb_ehdb_page_record2record(page_ptr, offset, &record);
    }

    //check if local depth > global depth
    if(ehdb_get_depth(page_ptr) > Global_depth)
    {
        //if yes, double the index
        //TODO
        ehdb_double_index(page_ptr);
        //TODO init 4 variables
    }
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
