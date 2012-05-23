#include "ehdb_IO_tracker.h"
#include "ehdb_init.h"
#include <stdio.h>

int icnt, ocnt,
    icnt_split, ocnt_split,
    icnt_double, ocnt_double;
int is_split;
int is_double;

void
ehdb_IO_track_init(){
    is_split = 0;
    is_double = 0;
}

void
ehdb_IO_split_start(){
    is_split = 1;
}

void
ehdb_IO_split_end(){
    is_split = 0;
}

void
ehdb_IO_double_start(){
    is_double = 1;
}

void
ehdb_IO_double_end(){
    is_double = 0;
}
void
ehdb_IO_mark_read(){
    icnt ++;
    if(is_split) icnt_split++;
    if(is_double) icnt_double++;
}

void
ehdb_IO_mark_write(){
    ocnt ++;
    if(is_split) ocnt_split++;
    if(is_double) ocnt_double++;
}

void
ehdb_IO_print(FILE* fout){
    int tot = icnt + ocnt;
    fprintf(fout, "I/O count:\n  in: %d(%.3f%%)\n  out: %d(%.3f%%)\n  total: %d\n\n", 
            icnt, icnt*100.0/tot, 
            ocnt, ocnt*100.0/tot,
            tot);
    fprintf(fout, "split: I: %.2f%% O: %.2f%% (percentage of all I/O)\n", 
            icnt_split*100.0/icnt, ocnt_split*100.0/ocnt);
    fprintf(fout, "double: I: %.2f%% O: %.2f%% (percentage of all I/O)\n", 
            icnt_double*100.0/icnt, ocnt_double*100.0/ocnt);
    fprintf(fout, "index count: %d\nbucket count: %d\n", Index_page_num, Bucket_page_num);
}

