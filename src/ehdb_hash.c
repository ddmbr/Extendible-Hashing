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

}

int
ehdb_hash_l(int key, int depth)
{
    return key >> depth;
}

short
ehdb_is_overflow(struct page_t* page_ptr)
{
    return ehdb_free_begin(page_ptr) >= ehdb_free_end(page_ptr);
}

void
ehdb_write_record(struct page_t* page_ptr, struct record_t record*)
{
    if(is_overflow(page_ptr))
    {
        int bucket_id = ehdb_split_bucket(page_ptr);
        page_ptr = ehdb_get_bucket_page(bucket_id);
        if(is_overflow(page_ptr))
        {
            bucket_id = ehdb_bucket_grow(page_ptr);
            page_ptr = ehdb_get_bucket_page(bucket_id);
        }
    }
    ehdb_record2page_record(record, page_ptr);
}

void
ehdb_double_index(struct page_t *page_ptr)
{
    //if the size of index is smaller than one page

    //if bigger than one page
    
}
