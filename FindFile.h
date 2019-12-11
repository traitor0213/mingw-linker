
#undef UNICODE

#include "KMP.h"
#include <windows.h>

int IsFileExist(const char *FilePath)
{
    HANDLE handle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
    if(handle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    CloseHandle(handle);

    return TRUE;
}

int RecursiveFindFile(const char *FindFilePath, const char *FindFileName, char *FullFilePath, int SizeOfFullFilePath)
{
    char FindFileFullPath[MAX_PATH * 2];
    wsprintf(FindFileFullPath, "%s/*.*", FindFilePath);

    WIN32_FIND_DATA FindData;
    HANDLE FindFileHandle = FindFirstFile(FindFileFullPath, &FindData);
    if (FindFileHandle == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    for (;;)
    {
        if (lstrcmpA(".", FindData.cFileName) != 0 && lstrcmpA("..", FindData.cFileName) != 0)
        {
            wsprintf(FindFileFullPath, "%s/%s", FindFilePath, FindData.cFileName);

            if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (RecursiveFindFile(FindFileFullPath, FindFileName, FullFilePath, SizeOfFullFilePath) == TRUE)
                {
                    return TRUE;
                }
            }
            else
            {
                if (lstrcmpiA(FindData.cFileName, FindFileName) == 0)
                {
                    if (SizeOfFullFilePath > lstrlen(FindFileFullPath))
                    {
                        memcpy(FullFilePath, FindFileFullPath, lstrlen(FindFileFullPath) + 1);
                        return TRUE;
                    }

                    return FALSE;
                }
            }
        }

        if (FindNextFile(FindFileHandle, &FindData) == FALSE)
        {
            break;
        }
    }

    FindClose(FindFileHandle);
    return FALSE;
}