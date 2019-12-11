#include <windows.h>

char *KMP(const char *source, int source_length, const char *destination, int destination_length)
{
    char *ptr = NULL;
    BOOL IsFind = TRUE;

    for (int i = 0; i != source_length; i++)
    {
        IsFind = TRUE;

        if (destination_length + i >= source_length)
        {
            break;
        }

        for (int x = 0; x != destination_length; x++)
        {
            if (source[i + x] != destination[x])
            {
                IsFind = FALSE;
                break;
            }
        }

        if (IsFind == TRUE)
        {
            ptr = (char *)source + i;
            break;
        }
    }

    return ptr;
}