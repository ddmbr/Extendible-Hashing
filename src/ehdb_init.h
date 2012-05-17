#pragma once
/* Initailize the system */
#define PAGE_SIZE (8 * 1024)

extern int Global_depth;
extern int Bucket_page_num;
extern int Index_page_num;

void
ehdb_init();
