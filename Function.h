#include <windows.h>
#ifndef Function
#define Function

/* 声明函数 */

int Declare();

int DeleteFolderContents(char *folderPath);

int DeleteWindwosService(char* serviceName);

int InputKeyGo(char* inputS, char* inputB);

LONG RecursiveDeleteRegistryKey(HKEY hParentKey, LPCSTR lpSubKey);

int TerminateProcessByName(char* processName);

int TerminateProcessesInDirectory(char* directoryPath);

#endif


