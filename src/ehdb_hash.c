int ehdb_hash_l(int key)
{

}
int ehdb_hash_h(int key)
{
    return key % (1 << Global_depth);
}
int ehdb_is_overflow(struct page_t* page_ptr)
{
    return ehdb_free_begin(page_ptr) >= ehdb_free_end(page_ptr);
}
struct page_t* ehdb_bucket_grow(int bucket_id)
{

}
