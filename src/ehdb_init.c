#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"

int Global_depth;
int Bucket_page_num;

void
ehdb_init()
{
    ehdb_buffer_init();
    ehdb_file_init();
}
