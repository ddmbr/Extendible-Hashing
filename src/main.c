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
char in_path[200], out_path[200];

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
        strcpy(in_path, argv[1]);
        strcat(in_path, "/");
    }else{
        strcpy(in_path, "./");
    }
    strcat(in_path, "lineitemcut.tbl");
    // start insert
    ehdb_bulk_insert(in_path);

    if(argc == 2){
        strcpy(in_path, argv[1]);
        strcat(in_path, "/");
        strcpy(out_path, argv[1]);
        strcat(out_path, "/");
    }else{
        strcpy(in_path, "./");
        strcpy(out_path, "./");
    }
    strcat(in_path, "testinput.in");
    strcat(out_path, "testoutput.out");
    // start query
    ehdb_bulk_query(in_path, out_path);

    if(argc == 2){
        strcpy(in_path, argv[1]);
        strcat(in_path, "/");
    }else{
        strcpy(in_path, "./");
    }
    strcat(in_path, "hashindex.out");
    // print to hashindex.out
    ehdb_print_hashindex(in_path);

    // clean an save
    ehdb_save_pages();
    ehdb_file_close();
    return 0;
}
