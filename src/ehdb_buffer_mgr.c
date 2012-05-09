#include "ehdb_buffer_mgr.h"

page_t *
ehdb_get_index_page(int hash_value){
    int index_id, offset;
    index_id = get_index_id(hash_value);
    //get the offset
    offset = hash_value % Dictpair_per_page;
}
