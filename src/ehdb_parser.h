#pragma once
#include "ehdb_record.h"
#include "ehdb_page.h"

/* this function must be called before calling ehdb_next_line()
 */
void
ehdb_parse_start(char * fileaddr);

/* get next record in the file
 */
record_t
ehdb_next_line();

int
ehdb_get_key(record_t* record){
    return record->orderkey;
}
