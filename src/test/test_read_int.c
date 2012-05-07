# include <stdio.h>
# include "io_utils.h"

FILE *f;
void *ptr;

int
main()
{
    ptr = malloc(8 * 1024);

    f = fopen("test", "r");
    //f = fopen("lineitem.tbl", "r");
    fread(ptr, 8 * 1024, 1, f);
    ptr = read_integer(ptr);
    ptr = read_float(ptr);
    fclose(f);
}
