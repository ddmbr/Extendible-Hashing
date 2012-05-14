#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"
#include "ehdb_utils.h"

int Global_depth;
int Bucket_page_num;
int Index_page_num; 

void
ehdb_init()
{
    Global_depth = 1;
    ehdb_buffer_init();
    ehdb_file_init();
    ehdb_file_buckets_init();
    ehdb_statistics_init();
}
