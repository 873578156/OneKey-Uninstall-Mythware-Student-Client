#include <windows.h>
#include <stdio.h>
#include "Function.h"

/* 递归删除指定注册表键及其所有子项 */
LONG RecursiveDeleteRegistryKey(HKEY hParentKey, LPCSTR lpSubKey) {

	HKEY hKey;
	LONG result;
	CHAR szSubKey[MAX_PATH];
	DWORD dwSize = MAX_PATH;

	/* 打开指定注册表键 */
	result = RegOpenKeyExA(hParentKey, lpSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &hKey);
	if (result != ERROR_SUCCESS) {
		printf("打开注册表项失败: %s, 错误码: %ld\n", lpSubKey, result);
		return result;
	}

	/* 递归遍历并删除所有子项 */
	while (RegEnumKeyExA(hKey, 0, szSubKey, &dwSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
		result = RecursiveDeleteRegistryKey(hKey, szSubKey);
		if (result != ERROR_SUCCESS) {
			printf("删除注册表子项失败: %s, 错误码: %ld\n", szSubKey, result);
			break;
		}
		dwSize = MAX_PATH;  /* 重置 dwSize 以便下一次枚举 */
	}

	RegCloseKey(hKey);  /* 关闭当前键 */

	result = RegOpenKeyExA(hParentKey, lpSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);	/* 重新打开指定注册表键，以删除当前键 */
	if (result == ERROR_SUCCESS) {
		result = RegDeleteKeyA(hParentKey, lpSubKey);
		if (result == ERROR_SUCCESS) {
			printf("成功删除注册表项: %s\n", lpSubKey);
		} else {
			printf("删除注册表项失败: %s, 错误码: %ld\n", lpSubKey, result);
		}
		RegCloseKey(hKey);
	} else {
		printf("打开注册表项失败: %s, 错误码: %ld\n", lpSubKey, result);
	}

	return result;
}

