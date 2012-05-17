#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_hash.h"
#include <stdio.h>

void
select_sort(record_t * a, int n);

void
ehdb_query(int key, FILE *fout);

void
ehdb_bulk_query(char * in_path, char * out_path);
