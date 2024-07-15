#include <windows.h>
#include <stdio.h>
#include "Function.h"

int DeleteFolderContents(char *folderPath) {

	char searchPath[MAX_PATH];
	HANDLE hFind;
	WIN32_FIND_DATA findFileData;

	snprintf(searchPath, MAX_PATH, "%s\\*", folderPath);	/* 正确初始化searchPath */

	hFind = FindFirstFile(searchPath, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		printf("在文件夹中查找文件时出错: %s\n", folderPath);
		return -1;
	}

	do {
		if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
			char filePath[MAX_PATH];
			snprintf(filePath, MAX_PATH, "%s\\%s", folderPath, findFileData.cFileName);

			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				DeleteFolderContents(filePath);	/* 递归删除子目录内容 */
				RemoveDirectory(filePath);
			} else {
				DeleteFile(filePath);	/* 删除文件 */
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
	return 0;
}

