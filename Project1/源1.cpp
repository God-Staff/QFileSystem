#include <windows.h>
#include <tchar.h>
#include <iostream>
#ifdef UNICODE
#define   tcout   std::wcout
#else
#define   tcout   std::cout
#endif

//此程序在程序当前目录下1.txt文件头处插入一字符串.

TCHAR CONST *szFileName = TEXT("1.txt");
//In this program, we assume the contents of 1.txt is Ascii.
CHAR CONST *szInsertText = "You are the sunshine that makes my day.";
INT nInsertTextSize = strlen(szInsertText);

int mai1n( )
{
    HANDLE hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
                              0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFile)
    {
        tcout << TEXT("未找到") << szFileName << TEXT("文件\r\n");
        exit(EXIT_FAILURE);
    }
    DWORD dwSizeLow;
    dwSizeLow = GetFileSize(hFile, NULL);
    DWORD dwModifyFileSize = dwSizeLow + nInsertTextSize;
    HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwModifyFileSize, NULL);
    if (!hMapFile)
    {
        tcout << TEXT("创建内存映射对象失败!\r\n");
        CloseHandle(hFile);
        exit(EXIT_FAILURE);
    }
    LPVOID pStart = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pStart)
    {
        tcout << TEXT("映射地址空间失败\r\n");
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        exit(EXIT_FAILURE);
    }
    INT Result = -1;
    BOOL bIsTextUnicode = IsTextUnicode(pStart, dwSizeLow, &Result);
    //In this program, we assume that the contents of 1.txt is Ascii.
    if (!bIsTextUnicode)
    {
        PSTR pCh = (PSTR) pStart;
        pCh[dwSizeLow / sizeof(CHAR)] = 0;
        memmove(pCh + nInsertTextSize, pCh, dwSizeLow);
        memcpy(pCh, szInsertText, nInsertTextSize);
    }
    else
    {
        tcout << szFileName << TEXT("为非Ascii文本\r\n");
        UnmapViewOfFile(pStart);
        CloseHandle(hMapFile);
        CloseHandle(hFile);
        exit(EXIT_FAILURE);
    }
    UnmapViewOfFile(pStart);
    CloseHandle(hMapFile);
    SetFilePointer(hFile, dwModifyFileSize, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);
    CloseHandle(hFile);
    tcout << TEXT("追加字符串成功!\r\n");
    return 0;
}
