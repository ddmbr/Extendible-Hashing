int ehdb_get_record_num(struct page_t* page_ptr)
{
    return *((size_t*)(page_ptr + 1 * sizeof(int)));
}
int ehdb_get_depth(struct page_t* page_ptr)
{
    return *((size_t*)(page_ptr));
}
int ehdb_get_next_bucket(struct page_t* page_ptr)
{
    return *((size_t*)(page_ptr + 2 * sizeof(int)));
}
void *ehdb_free_begin(struct page_t* page_ptr)
{
    return 
}
void *ehdb_free_end(struct page_t* page_ptr)
{
    size_t free_end = *((size_t*)(page_ptr + 3 * sizeof(int)));
    return page_ptr + free_end;
}
