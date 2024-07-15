#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "Function.h"

int TerminateProcessesInDirectory(char* directoryPath) {

	char searchPath[MAX_PATH];
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	sprintf(searchPath, "%s\\*", directoryPath);	/* 构建搜索路径 */

	hFind = FindFirstFile(searchPath, &findFileData);	/* 开始查找第一个文件 */
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("无效的目录路径或未找到文件.\n");
		return -1;
	}
	do {
		if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {	/* 排除当前目录和上级目录 */
			snprintf(searchPath, MAX_PATH, "%s\\%s", directoryPath, findFileData.cFileName);	/* 构建完整的文件路径 */
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	/* 检查是否是目录 */
				TerminateProcessesInDirectory(searchPath);	/* 递归调用自身处理子目录 */
			} else {
				if (strstr(findFileData.cFileName, ".exe") != NULL) {	/* 检查是否是 .exe 文件 */
					TerminateProcessByName(findFileData.cFileName);	/* 终止该进程 */
				}
			}
		}
	} while (FindNextFile(hFind, &findFileData));	/* 继续查找下一个文件 */

	FindClose(hFind);
	return 0;
}

int TerminateProcessByName(char* processName) {

	HANDLE hSnapShot;
	PROCESSENTRY32 pe;
	HANDLE hProcess;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		printf("CreateToolhelp32Snapshot 失败.\n");
		return -1;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapShot, &pe)) {
		printf("Process32First 失败.\n");
		CloseHandle(hSnapShot);
		return -1;
	}

	do {
		if (_stricmp(pe.szExeFile, processName) == 0) {
			hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			if (hProcess != NULL) {
				if (TerminateProcess(hProcess, 0)) {
					printf("终止进程成功: %s\n", pe.szExeFile);
				} else {
					printf("终止进程失败: %s\n", pe.szExeFile);
				}
				CloseHandle(hProcess);
			}
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);

	return 0;
}

