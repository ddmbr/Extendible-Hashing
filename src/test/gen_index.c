# include <stdio.h>
# include "io_utils.h"

FILE *f;

int
main()
{
    unsigned int num = 0;
    f = fopen("index", "w");
    write_data(1, f);
    write_data(1, f);
    write_data(2, f);
    write_data(2, f);
    write_data(3, f);
    write_data(1, f);
    fclose(f);

    #ifdef DEBUG
    f = fopen("index", "r");
    fread((&num), sizeof(num), 1, f);
    printf("%d\n", num);
    fclose(f);
    #endif

}
