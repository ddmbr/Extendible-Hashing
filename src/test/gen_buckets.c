# include <stdio.h>
# include "io_utils.h"

FILE *f;

int
main()
{
    int num;

    f = fopen("buckets", "w");

    write_int(1, f);
    write_int(3, f);
    write_int(0, f);

    write_int(1, f);
    write_int(2, f);
    write_int(3, f);
    write_int(4, f);

    write_float(1.1, f);
    write_float(2.2, f);
    write_float(3.3, f);
    write_float(4.4, f);

    write_char('a', f);
    write_char('b', f);
}
