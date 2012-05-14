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
        ehdb_single_insert(&record);
    }
}

int
main(int argc, char *argv[])
{
    ehdb_init();
    if(argc == 2){
        strcpy(path, argv[1]);
        strcat(path, "/");
    }else{
        strcpy(path, "./");
    }
    strcat(path, "lineitemcut.tbl");
    ehdb_bulk_insert(path);

    if(argc == 2){
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcpy(path1, argv[1]);
        strcat(path1, "/");
    }else{
        strcpy(path, "./");
        strcpy(path1, "./");
    }
    strcat(path, "testinput.in");
    strcat(path1, "testoutput.out");
    ehdb_bulk_query(path, path1);

    if(argc == 2){
        strcpy(path, argv[1]);
        strcat(path, "/");
    }else{
        strcpy(path, "./");
    }
    strcat(path, "hashindex.out");
    ehdb_print_hashindex(path);
    ehdb_statistics();

    ehdb_save_pages();
    ehdb_file_close();
    return 0;
}
