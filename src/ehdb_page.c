#include "ehdb_page.h"
#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"


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
    return page_ptr->head + 16
        + ehdb_get_record_num(page_ptr) * RECORD_SIZE;
}

void*
ehdb_free_end(page_t* page_ptr)
{
    return page_ptr->head + ((int*)(page_ptr->head))[3];
}

void
ehdb_set_page_link(page_t* page_ptr, int page_id)
{
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
    ehdb_set_free_end(page_ptr, page_ptr->head+PAGE_SIZE-1);
}

void
ehdb_init_page_record_num(page_t *page_ptr)
{
    ehdb_set_page_record_num(page_ptr, 0);
}

void
ehdb_init_page_link(page_t *page_ptr)
{
    //TODO
}

void
ehdb_set_page_depth(page_t *page_ptr, int depth)
{
    ((int*)page_ptr->head)[0] = depth;
    page_ptr->modified = 1;
}

void
ehdb_set_page_record_num(page_t *page_ptr, int record_num)
{
    ((int*)page_ptr->head)[1] = record_num;
}

void
ehdb_set_free_end(page_t *page_ptr, void* free_end)
{
    ((int*)page_ptr->head)[3] = free_end - page_ptr->head;
}
