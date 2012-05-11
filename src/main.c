#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
int
main()
{
    ehdb_init();
    ehdb_bulk_insert("lineitemcut.tbl");
    ehdb_save_pages();
    ehdb_file_close();
    return 0;
}
