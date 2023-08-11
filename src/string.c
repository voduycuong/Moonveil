#include "string.h"

/* Function for comparing 2 strings
 * return 1 if 2 strings is equal
 * return 0 if 2 strings is NOT equal
 */
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

/* Function for extracting option of a command
 * return a substring from 'pos' position of original string
 */
char *subst(char *string, int pos)
{
    char *result;
    result = &string[pos];
    return result;
}

// Function for counting string length
int strlen(char *string)
{
    int length = 0;
    while (string[length] != '\0')
        length++;
    return length;
}

/* Function for searching a string in a string array
 * return its index if found
 * return 0 if not found
 */
// int search_str(char *source, char *keyword)
// {
// }
