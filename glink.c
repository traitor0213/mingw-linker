#undef UNICODE

#include "FindFile.h"

#include <windows.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char ExecuteParameter[1024];
    char MingWPath[1024];

    //get program full path
    char ProgramPath[1024];
    if (GetModuleFileName(NULL, ProgramPath, (DWORD)sizeof(ProgramPath)) == FALSE)
    {
        //get program full path fails, return 0;
        return 0;
    }

    int ProgramPathLength = lstrlen(ProgramPath);
    int LastSlash = 0;

    //get last slash word index
    for (int index = 0; index != ProgramPathLength; index++)
    {
        if (ProgramPath[index] == '\\' || ProgramPath[index] == '/')
        {
            LastSlash = index + 1;
        }
    }

    //setting file
    char InitializeFileName[] = "setting.ini";
    char InitializeFilePath[1024];

    //check BOF
    if (lstrlen(ProgramPath) + sizeof(InitializeFileName) <= sizeof(ProgramPath))
    {
        //BOF safe
        memcpy(ProgramPath + LastSlash, InitializeFileName, sizeof(InitializeFileName));

        //setting.ini open
        HANDLE InitializeFileHandle = CreateFile(ProgramPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
        if (InitializeFileHandle == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        //error holds variable
        BOOL FileReadError = FALSE;

        //ReadIndex
        int ReadIndex = 0;

        for (;; ReadIndex++)
        {
            //read to one byte from setting.ini

            //ReadFile API fails, FileReadError variable holds TRUE
            if (ReadFile(InitializeFileHandle, &MingWPath[ReadIndex], sizeof(MingWPath[ReadIndex]), NULL, NULL) == FALSE)
            {
                FileReadError = TRUE;
                break;
            }

            //if readed semicolon, break;
            if (MingWPath[ReadIndex] == ';')
            {
                break;
            }
        }

        //close file handle
        CloseHandle(InitializeFileHandle);

        //is not FileReadError holds TRUE
        if (FileReadError == FALSE)
        {
            //replace semicolon to NULL
            MingWPath[ReadIndex] = 0;

            //MingWPath copy to CmdLine
            char CmdLine[1024];
            memcpy(CmdLine, MingWPath, lstrlen(MingWPath) + 1);

            //Create gcc command line
            if (argc > 1)
            {
                char *ptr = GetCommandLineA();
                ptr = strchr(ptr, ' ');

                if(ptr == NULL)
                {
                    return 0;
                }

                ptr++;

                if (IsFileExist(MingWPath) == TRUE)
                {
                    //if setting path is exist
                    wsprintf(CmdLine, "%s%s", CmdLine, ptr);
                }
                else
                {
                    //if setting path is not exist
                    char FullFilePath[MAX_PATH];
                    RecursiveFindFile("C:", "gcc.exe", FullFilePath, sizeof(FullFilePath));
                    
                    wsprintf(CmdLine, "%s%s", FullFilePath, ptr);
                }

                //start gcc
                system(CmdLine);
            }
            else
            {
                //argc == 1

                return 0;
            }
        }
    }

    return 0;
}