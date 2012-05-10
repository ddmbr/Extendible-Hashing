#include "ehdb_page.h"
#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"

#define Record_size (13 + 3*2)

int
ehdb_get_record_num(page_t* page_ptr)
{
    return ((int*)page_ptr->head)[1];
}

int
ehdb_get_depth(page_t* page_ptr)
{
    return ((int*)page_ptr->head)[0];
}

page_t*
ehdb_get_next_bucket(page_t* page_ptr)
{
    return ehdb_get_bucket_page(((int*)page_ptr->head)[2]);
}

void*
ehdb_free_begin(page_t* page_ptr)
{
    return page_ptr + 4*sizeof(int) + ehdb_get_record_num(page_ptr) * Record_size;
}

int
ehdb_get_record_string_len(page_t* page){
    return ((int*)page->head)[3];
}

void*
ehdb_free_end(page_t* page_ptr)
{
    int string_len = ehdb_get_record_string_len(page_ptr);
    return page_ptr->head + (PAGE_SIZE - string_len);
}

void
ehdb_set_page_link(page_t* page_ptr, int page_id)
{
    /* ((int*)page_ptr->head)[3] = page_id; */
    /* int string_len = ehdb_get_record_string_len(page_ptr); */
    /* return page_ptr->head + (Page_size - string_len); */
}

/*
void
ehdb_inc_bucket_depth(page_t* page_ptr)
{

    ((int*)page_ptr->head)[0]++;
}
*/

void
ehdb_init_page_free_end(page_t *page_ptr)
{
    ((int*)page_ptr->head)[3] = 0;
}

void
ehdb_init_page_record_num(page_t *page_ptr)
{
    ((int*)page_ptr->head)[1] = 0;
}

void
ehdb_init_page_link(page_t *page_ptr)
{

}

void
ehdb_set_page_depth(page_t *page_ptr, int depth)
{
    ((int*)page_ptr->head)[0] = depth;
    page_ptr->modified = 1;
}
