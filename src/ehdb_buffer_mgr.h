#pragma once
#include "ehdb_init.h"
#include "ehdb_file_mgr.h"
#include "ehdb_page.h"

// how many (hash_value, bucket_id) tuple can a page store
#define Dictpair_per_page (2048); 

page_t *
ehdb_get_bucket_page(int hash_value);

page_t * ehdb_make_available_page();
