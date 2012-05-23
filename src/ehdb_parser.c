#include "ehdb_parser.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_record.h"
#include "ehdb_init.h"
#include "ehdb_hash.h"
#include "string.h"
#include "stdio.h"

#define LINE_SIZE (200)

FILE * fin;
static char buf[PAGE_SIZE + 1];
char * current_pos;
int buf_size;
int eof;
int total_fsize;
int total_read;

/* this function must be called
 * before calling ehdb_next_line()
 */
void
ehdb_parse_start(char * fileaddr){
    fin = fopen(fileaddr, "r");
    if(fin == NULL){
        fprintf(stderr, "open file %s failed\n", fileaddr);
    }else{
        fprintf(stderr, "loaded file %s \n", fileaddr);
    }
    fseek(fin, 0, SEEK_END);
    total_fsize = ftell(fin) + 1;
    printf("total file size is %d\n", total_fsize);
    fseek(fin, 0, SEEK_SET);
    
    total_read = 0;
    buf_size = 0;
    current_pos = buf;
    eof = 0;
}

/* test whether end-of-file is reached
 */
int
ehdb_test_eof(){
    if(fin == NULL) return 1;
    return (current_pos - buf >= buf_size && eof == 1);
}

/* get next line in the file,
 * convert and store in record
 */
int
ehdb_next_line(record_t * record){
    char * end_pos;
    size_t rest_size;
    while(1){
        if(ehdb_test_eof()) return 0;
        end_pos = current_pos;

        /* find the next '\n'   */
        while(end_pos - buf < buf_size && *end_pos != '\n')
            end_pos++;

        rest_size = end_pos - current_pos;
        if(end_pos - buf >= buf_size){
            /* can not collect a line   */
            /* copy the rest string(the half line) to the begining of buf   */
            strncpy(buf, current_pos, rest_size);
            current_pos = buf;

            /* read new lines into buf after        */
            int readnum = fread(buf + rest_size, 1, (PAGE_SIZE - rest_size), fin);
            total_read += readnum;

            fprintf(stderr, "\rPARSER: building database...");
            fprintf(stderr, "%.2lf%%", (total_read * 100.0 / total_fsize));

            buf_size = rest_size + readnum;
            if(readnum < PAGE_SIZE - rest_size){
                if(buf[rest_size + readnum - 1] != '\n')
                    buf[rest_size + readnum] = '\n';
                eof = 1;
            }
        }else{
            if(end_pos == current_pos)
                return 0;
            break;
        }
    }
    current_pos = ehdb_raw2record(current_pos, record);
    return 1;
}

/* load data from file
 * and build the database
 */
void
ehdb_bulk_insert(char * fileaddr)
{
    ehdb_parse_start(fileaddr);
    record_t record;
    int record_num = 0;

    while(!ehdb_test_eof())
    {
        if(ehdb_next_line(&record) == 0) continue;
        ehdb_write_record(&record);

        record_num++;
    }
    fprintf(stderr, "\nPARSER: Done.\n");
}

