#pragma once
#include "init.h"
#include "ehdb_file_mgr.h"

// how many (hash_value, bucket_id) tuple can a page store
const int Dictpair_per_page = Page_size / (2 * (sizeof(int)));

/* load bucket page from disk
 */
static void
load_bucket_page(int bucket_id);

/* load index page from disk
 */
static void
load_index_page(int index_id);

static void inline 
get_index_id(int hash_value){
    return hash_value / Dictpair_per_page;
}

/*
 * get a page by the hash_value
 */
page_t *
ehdb_get_index_page(int hash_value);

page_t *
ehdb_get_bucket_page(int bucket_id);


