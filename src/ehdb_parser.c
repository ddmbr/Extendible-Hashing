#include "ehdb_parser.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"
#include "ehdb_init.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE_SIZE (200)
int fd;
int read_size;
static char buf[PAGE_SIZE + 1];
static char line[LINE_SIZE];
char * current_pos;
int buf_size;
int eof;

void
ehdb_parse_start(char * fileaddr){
    fd = open(fileaddr, O_RDONLY);
    int fsize;
    fsize = lseek(fd, -1, SEEK_END) + 1;
    printf("fsize=%d\n", fsize);
    posix_fadvise(fd, 0, fsize, POSIX_FADV_WILLNEED|POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
    read_size = 0;
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
            int readnum;
            readnum = pread(fd, buf + rest_size, (PAGE_SIZE - rest_size), read_size);
            read_size += readnum;
            buf_size = rest_size + readnum;
            if(readnum < PAGE_SIZE - rest_size){
                if(buf[rest_size + readnum - 1] != '\n')
                    buf[rest_size + readnum] = '\n';
                eof = 1;
                close(fd);
            }
        }else{
            break;
        }
    }
    current_pos = ehdb_raw2record(current_pos, record);
}

