#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_hash.h"
#include "ehdb_query.h"
#include "string.h"
#include "ehdb_parser.h"
#include "stdio.h"
#include "ehdb_record.h"
#include "ehdb_page.h"
#include "ehdb_utils.h"
char path[200], path1[200];

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
    // start insert
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
    // start query
    ehdb_bulk_query(path, path1);

    if(argc == 2){
        strcpy(path, argv[1]);
        strcat(path, "/");
    }else{
        strcpy(path, "./");
    }
    strcat(path, "hashindex.out");
    // print to hashindex.out
    ehdb_print_hashindex(path);

    // clean an save
    ehdb_save_pages();
    ehdb_file_close();
    return 0;
}
