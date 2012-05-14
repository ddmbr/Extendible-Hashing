#include <stdio.h>
#define MAX_DEPTH 30

int
ehdb_hash_h(int key, int depth)
{
    int invert = 0, i;

    for(i = MAX_DEPTH - 1; i >= 0; i--)
        if((key & (1 << i)) > 0)
            break;
    int sig = i;

    //longer
    if(sig >= depth)
        key >>= (sig - depth);
    //shorter
    else
        key <<= (depth - sig);

    key &= ((1 << depth) - 1);

    for(i = 1; i <= depth; i++)
        invert += ((key & (1 << (i - 1))) > 0) << (depth - i);
    return invert;
}
int main()
{
    int key, depth;
    scanf("%d%d", &key, &depth);
    printf("%d\n", ehdb_hash_h(key, depth));
}
