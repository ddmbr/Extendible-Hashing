#include "ehdb_parser.h"
#include "ehdb_page.h"
#include "ehdb_record.h"
#include "stdio.h"

char str[300];

int main(){
    ehdb_parse_start("in");
    record_t record;

    while(!ehdb_test_eof()){
        ehdb_next_line(&record);
        printf("read a line------------------------\n");
        ehdb_record2raw(&record, str);
        printf(str);
    }
    return 0;
}
