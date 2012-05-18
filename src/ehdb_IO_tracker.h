#pragma once
#include <stdio.h>

void
ehdb_IO_track_init();

void
ehdb_IO_split_start();

void
ehdb_IO_split_end();

void
ehdb_IO_double_start();

void
ehdb_IO_double_end();

void
ehdb_IO_mark_read();

void
ehdb_IO_mark_write();

void 
ehdb_IO_print(FILE *);
