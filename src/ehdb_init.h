#pragma once
/* Initailize the system */
#define Page_size (8 * 1024)
#define Page_num (8)

extern int Global_depth;
extern int Bucket_page_num;
extern int Index_page_num;

void
ehdb_init();
