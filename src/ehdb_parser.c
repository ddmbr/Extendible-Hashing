#include "ehdb_parser.h"
#include "ehdb_buffer_mgr.h"

FILE * fin;

void
ehdb_parse_start(char * fileaddr){
    fin = fopen(fileaddr, "r");
    //TODO
}

record_t
ehdb_next_line();
