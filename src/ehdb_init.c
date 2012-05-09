#include "ehdb_init.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"

int global_depth;

void
ehdb_init()
{
    ehdb_buffer_init();
    //TODO: init file manager
}
