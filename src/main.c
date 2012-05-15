#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "string.h"
#include "stdio.h"
#include "ehdb_record.h"
#include "ehdb_page.h"
char path[200], path1[200];

void ehdb_single_insert(record_t *record)
{
    int key = ehdb_get_key(record);
    int hv = ehdb_hash_func(key, Global_depth);
#ifdef DEBUG
    fprintf(stderr, "key: %d, global depth: %d, hash value: %d\n", key, Global_depth, hv);
#endif

    page_t *page_ptr;
    page_ptr = ehdb_get_bucket_page_by_hvalue(hv);

    ehdb_write_record(page_ptr, record);
}

void
ehdb_bulk_insert(char * fileaddr)
{
    ehdb_parse_start(fileaddr);
    record_t record;
    while(!ehdb_test_eof())
    {
        ehdb_next_line(&record);
        ehdb_write_record(&record);
    }
}

int
main()
{
    ehdb_init();
    ehdb_bulk_insert("lineitem.tbl");
    ehdb_bulk_query("testinput.in");
    ehdb_save_pages();
    //ehdb_statistics();
    ehdb_file_close();
    return 0;
}
