#pragma once
#include "page.h"
typedef int identifier_t;
typedef short flag_t;
typedef int decimal_t;
typedef int date_t;

struct record_t{
    identifier_t orderkey, 
               partkey, 
               suppkey;
    int linenumber;
    decimal_t quantity,
            extendedprice,
            discount,
            tax;
    flag_t returnflag,
           linestatus;
    date_t shipdate,
           commitdate,
           receiptdate;

    char shipinstruct[25];
    char shipmode[10];
    char comment[44];
};

/* read the string and convert to `record`, starting from `start`
 * return: the end position
 */
char * 
ehdb_raw2record(char * start, record_t * record);

/* convert a record to a raw string, ends with '\0'
 */
void 
ehdb_record2raw(record_t * record, char * raw);

/* read and convert a record from `page` start with `offset`
 * return: the new offset when conversion finished
 */
int 
ehdb_page_record2record(page_t * page, int offset, record_t * record);

/* write a record to page
 */
void 
ehdb_record2page_record(record_t * record, page_t * page);

/* check the total size if the record is to be converted to page_record
 */
size_t test_record_size(record_t * record);
