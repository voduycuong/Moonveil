#include "string.h"

// Function for comparing 2 strings
int strcmp(char *a, char *b)
{
    int i = 0, c = 0;
    while ((a[i] != '\0') || (b[i] != '\0'))
    {
        if (a[i] != b[i])
            c++;
        i++;
    }

    if (c == 0)
        return 1;
    else
        return 0;
}

// Function for extracting option of a command
char *subst(char *string, int pos)
{
    char *result;
    result = &string[pos];
    return result;
}