#include "ehdb_record.h"
#include "stdio.h"

// convert ints to date_t
date_t ints2date(int y, int m, int d){
    return (y << 8) + (m << 4) + d;
}

// convert date_t to ints
void date2ints(date_t date, int ymds[3]){
    ymds[0] = date >> 8;
    ymds[1] = (date >> 4) & 0xf;
    ymds[2] = (date) % 0xf;
}

char * 
ehdb_raw2record(char * start, record_t * record){
    int ymds[3][3];
    memset(record->shipinstruct, 0, sizeof record->shipinstruct);
    memset(record->shipmode, 0, sizeof record->shipmode);
    memset(record->comment, 0, sizeof record->comment);
    sscanf(start, 
            "%d|%d|%d|%d|%f|%f|%f|%f|%c|%c|%d-%d-%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^|]|%[^|]|\n",
            &record->orderkey, 
            &record->partkey,
            &record->suppkey,

            &record->linenumber,

            &record->quantity,
            &record->extendedprice,
            &record->discount,
            &record->tax,

            &record->returnflag,
            &record->linestatus,

            ymds[0], ymds[0]+1, ymds[0]+2,
            ymds[1], ymds[1]+1, ymds[1]+2,
            ymds[2], ymds[2]+1, ymds[2]+2,

            record->shipinstruct,
            record->shipmode,
            record->comment
                );
    record->shipdate = ints2date(ymds[0][0], ymds[0][1], ymds[0][2]);
    record->commitdate = ints2date(ymds[1][0], ymds[1][1], ymds[1][2]);
    record->receiptdate = ints2date(ymds[2][0], ymds[2][1], ymds[2][2]);
    char * p = start;
    while(*p && *p != '\n') p++;
    if(*p)
        return ++p;
    else
        return p;
}

void 
ehdb_record2raw(record_t * record, char * raw){
    int ymds[3][3];
    date2ints(record->shipdate, ymds[0]);
    date2ints(record->commitdate, ymds[1]);
    date2ints(record->receiptdate, ymds[2]);
    sprintf(raw, 
            "%d|%d|%d|%d|.2%f|.2%f|.2%f|.2%f|%c|%c|%d-%d-%d|%d-%d-%d|%d-%d-%d|%s|%s|%s|\n",
            record->orderkey, 
            record->partkey,
            record->suppkey,

            record->linenumber,

            record->quantity,
            record->extendedprice,
            record->discount,
            record->tax,

            record->returnflag,
            record->linestatus,

            ymds[0][0], ymds[0][1], ymds[0][2],
            ymds[1][0], ymds[1][1], ymds[1][2],
            ymds[2][0], ymds[2][1], ymds[2][2],

            record->shipinstruct,
            record->shipmode,
            record->comment
                );
}

int 
ehdb_page_record2record(page_t * page, int offset, record_t * record){
    //TODO
}

void 
ehdb_record2page_record(record_t * record, page_t * page){
    //TODO
}

size_t ehdb_test_record_size(record_t * record){
    size_t sum = 0;
    sum += sizeof(identifier_t) * 3;
    sum += sizeof(int) * 1;
    sum += sizeof(decimal_t) * 4;
    sum += sizeof(flag_t) * 2;
    sum += sizeof(date_t) * 3;
    sum += 2 + strlen(record->shipinstruct);
    sum += 2 + strlen(record->shipmode);
    sum += 2 + strlen(record->comment);
    return sum;
}
