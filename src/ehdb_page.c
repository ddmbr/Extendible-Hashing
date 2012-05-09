#include "ehdb_page.h"
#include "ehdb_init.h"

#define Record_size (13 + 3*2)

int
ehdb_get_record_num(struct page_t* page_ptr)
{
    return ((int*)page_ptr->head)[1];
    /* return *((size_t*)(page_ptr->head + 1 * sizeof(int))); */
}

int
ehdb_get_depth(struct page_t* page_ptr)
{
    return ((int*)page_ptr->head)[0];
    /* return *((size_t*)(page_ptr)); */
}

int
ehdb_get_next_bucket(struct page_t* page_ptr)
{
    return ((int*)page_ptr->head)[2];
    /* return *((size_t*)(page_ptr + 2 * sizeof(int))); */
}

void*
ehdb_free_begin(struct page_t* page_ptr)
{
    return page_ptr + 4*sizeof(int) + ehdb_get_record_num(page_ptr) * Record_size;
}

int
ehdb_get_record_string_len(page_t* page){
    return ((int*)page_ptr->head)[3];
}

void*
ehdb_free_end(struct page_t* page_ptr)
{
    /* size_t free_end = *((size_t*)(page_ptr + 3 * sizeof(int))); */
    int string_len = ehdb_get_record_string_len(page_ptr);
    return page_ptr->head + (Page_size - string_len);
}
