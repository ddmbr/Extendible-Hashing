#include "ehdb_parser.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"
#include "string.h"

const int Line_size = 200;
FILE * fin;
char buf[Page_size + 1];
char line[Line_size];
char * current_pos;

void
ehdb_parse_start(char * fileaddr){
    fin = fopen(fileaddr, "r");
    buf[0] = '\0';
    current_pos = 0;
}

record_t
ehdb_next_line(){
    char * end_pos;
    size_t rest_size;
    while(1){
        end_pos = current_pos;
        while(end_pos - buf < Page_size && *end_pos != '\n')
            end_pos++;
        rest_size = end_pos - current_pos;
        if(end_pos - buf >= Page_size){
            // can not collect a line
            // copy the rest string(the half line) to the begining of buf
            strcpy(buf, current_pos, rest_size);
            current_pos = 0;
            // read new lines into buf after 
            int readnum = fread(buf + rest_size, 1, (Page_size - rest_size), fin);
            if(readnum < Page_size - rest_size){
                if(buf[rest_size + readnum - 1] != '\n')
                    buf[rest_size + readnum] = '\n';
            }
        }else{
            break;
        }
    }
    record_t record;
    current_pos = ehdb_raw2record(current_pos, &record);
    return record;
}
