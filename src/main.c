#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "string.h"
char path[200], path1[200];
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
