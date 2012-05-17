#include "ehdb_init.h"
#include "ehdb_utils.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_query.h"
#include "ehdb_parser.h"

char out_path[200];
char in_path[200];

inline void
init_path(char * data_path, char *given_path, char *file_name)
{
    strcpy(data_path, given_path);
    strcat(data_path, "/");
    strcat(data_path, file_name);
}

int
main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: ./ehdb /path/to/lineitem\n\n");
        return 0;
    }
    /* init the program */
    ehdb_init();

    /* Build the database   */
    init_path(in_path, argv[1], "lineitem.tbl");
    ehdb_bulk_insert(in_path);

    /* start query  */
    init_path(in_path, argv[1], "testinput.in");
    init_path(out_path, argv[1], "testoutput.out");
    ehdb_bulk_query(in_path, out_path);

    /* Output the result    */
    init_path(out_path, argv[1], "hashindex.out");
    ehdb_print_hashindex(out_path);

    /* clean an save        */
    ehdb_save_pages();
    ehdb_file_close();

    return 0;
}
