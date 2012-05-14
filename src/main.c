#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
int
main()
{
    ehdb_init();
    ehdb_bulk_insert("lineitemcut.tbl");
    ehdb_bulk_query("testinput.in");
    ehdb_save_pages();
    ehdb_statistics();
    ehdb_file_close();
    return 0;
}
