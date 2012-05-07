#pragma once

void
write_int(unsigned int num, FILE *f)
{
    fwrite((void*)(&num), sizeof(num), 1, f);
}
write_float(unsigned double num, FILE *f)
{
    fwrite((void*)(&num), sizeof(num), 1, f);
}
write_float(char ch, FILE *f)
{
    fwrite((void*)(&ch), sizeof(ch), 1, f);
}
char*
read_integer(char *ptr)
{
    char *start_pos = ptr;
    char *temp;
    int num;

    while(*ptr != '|')
        ptr++;
    
    temp = (char*)malloc(ptr - start_pos);
    strncpy(temp, start_pos, ptr - start_pos);
    sscanf(start_pos, "%d", &num);
    printf("%d\n", num);
    return ptr + 1;
}
char*
read_float(char *ptr)
{
    char *start_pos = ptr;
    char *temp;
    double num;

    while(*ptr != '|')
        ptr++;
    
    temp = (char*)malloc(ptr - start_pos);
    strncpy(temp, start_pos, ptr - start_pos);
    sscanf(start_pos, "%lf", &num);
    printf("%lf\n", num);
    return ptr + 1;
}
char*
read_string(char *ptr, char *free_start)
{
    char *start_pos = ptr;
    char *temp;
    double num;

    while(*ptr != '|')
        ptr++;
}

