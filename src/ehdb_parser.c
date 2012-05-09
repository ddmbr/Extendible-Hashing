#include "ehdb_parser.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"
#include "ehdb_init.h"
#include "string.h"
#include "stdio.h"

#define Line_size (200)
FILE * fin;
static char buf[Page_size + 1];
static char line[Line_size];
char * current_pos;
int buf_size;
int eof;

void
ehdb_parse_start(char * fileaddr){
    fin = fopen(fileaddr, "r");
    buf_size = 0;
    current_pos = buf;
    eof = 0;
}

int
ehdb_get_key(record_t* record){
    return record->orderkey;
}

int
ehdb_test_eof(){
    return (current_pos - buf >= buf_size && eof == 1);
}

void
ehdb_next_line(record_t * record){
    if(ehdb_test_eof()) return;
    char * end_pos;
    size_t rest_size;
    while(1){
        end_pos = current_pos;
        // find the next '\n'
        while(end_pos - buf < buf_size && *end_pos != '\n')
            end_pos++;
        rest_size = end_pos - current_pos;
        if(end_pos - buf >= buf_size){
            // can not collect a line
            // copy the rest string(the half line) to the begining of buf
            strncpy(buf, current_pos, rest_size);
            current_pos = buf;
            // read new lines into buf after 
            int readnum = fread(buf + rest_size, 1, (Page_size - rest_size), fin);
            buf_size = rest_size + readnum;
            if(readnum < Page_size - rest_size){
                if(buf[rest_size + readnum - 1] != '\n')
                    buf[rest_size + readnum] = '\n';
                eof = 1;
            }
        }else{
            break;
        }
    }
    current_pos = ehdb_raw2record(current_pos, record);
}
