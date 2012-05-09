#pragma once
#include "init.h"
#include "ehdb_file_mgr.h"

// how many (hash_value, bucket_id) tuple can a page store
const int Dictpair_per_page = Page_size / (sizeof(int));

page_t *
ehdb_get_bucket_page(int hash_value);

page_t * ehdb_make_available_page();
