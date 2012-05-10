#pragma once
#include "ehdb_init.h"
#include "ehdb_file_mgr.h"
#include "ehdb_page.h"

//define how many (hash_value, bucket_id) tuple can a page store
#define Dictpair_per_page (2048) 

void
ehdb_buffer_init();

page_t *
ehdb_get_bucket_page_by_hvalue(int hash_value);

page_t *
ehdb_get_bucket_page(int bucket_id);

page_t *
ehdb_get_index_page(int index_id);

page_t * ehdb_make_available_page();