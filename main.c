#include <windows.h>
#include <stdio.h>
#include "Function.h"


int main() {

	struct Folder {	/* 定义一个结构体，用于存储文件夹路径信息 */
		char path[MAX_PATH];
	};

	typedef struct {	/* 定义一个结构体，用于存储注册表信息 */
		HKEY hKeyParent;    /* 父键句柄 */
		LPCSTR lpSubKey;    /* 子键路径 */
	} RegistryInfo;

	struct Folder folders[] = {	/* 文件夹路径数组 */
		{ "C:\\Program Files (x86)\\Mythware" },
		{ "C:\\Program Files\\云智谷" },
		{ "C:\\Program Files (x86)\\Mythware" },
		{ "C:\\Program Files\\云智谷" }   /* 此处路径，需要检查 */
	};

	size_t i;

	/* 要结束的进程名称数组 *
	char* processes[] = {
	"StudentMain.exe",
	"StudentMain64.exe",
	"GATESRV.exe",
	"GATESRV64.exe",
	"MasterHelper.exe",
	"MasterHelper64.exe",
	"ProcHelper.exe",
	"ProcHelper64.exe"
	};
	*/

	char* serviceName = "STUDSRV";

	/* 假设有一个注册表键数组 */
	RegistryInfo registryKeys[] = {
		{HKEY_CLASSES_ROOT, "Installer\\Products\\FDEE4BF597A63C540B94FE23C70AF3DC" },
		{HKEY_CURRENT_USER, "SOFTWARE\\WOW6432Node\\TopDomain" },
		{HKEY_CURRENT_USER, "SOFTWARE\\TopDomain" },
		{HKEY_CURRENT_USER, "Software\\Classes\\Installer\\Products\\FDEE4BF597A63C540B94FE23C70AF3DC" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\TopDomain" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\TopDomain" },
		{HKEY_LOCAL_MACHINE, "Software\\Classes\\Installer\\Products\\FDEE4BF597A63C540B94FE23C70AF3DC" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Products\\FDEE4BF597A63C540B94FE23C70AF3DC" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Components\\1BD5604FE0B0B694FA20D27FCA8639F8" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\UFH\\ARP" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{5FB4EEDF-6A79-45C3-B049-EF327CA03FCD}" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\e-Learning Class V6.0" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\Mythware" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{5FB4EEDF-6A79-45C3-B049-EF327CA03FCD}" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\e-Learning Class V6.0" },
		{HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Mythware" },
		{HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet001\\Services\\STUDSRV" },
		{HKEY_LOCAL_MACHINE, "SYSTEM\\ControlSet002\\Services\\STUDSRV" },  /* 此处键名 */
		{HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\STUDSRV" }
	};

	Declare();  /* 调用作者声明 */

	if (InputKeyGo("y", "Y") == -1) {	/* 如果用户输入不是'y'或'Y'，则退出程序 */
		return -1;
	}

	for ( i = 0; i < sizeof(registryKeys) / sizeof(registryKeys[0]); ++i) {	/* 遍历注册表键数组，调用递归删除注册表键的函数 */
		LONG result = RecursiveDeleteRegistryKey(registryKeys[i].hKeyParent, registryKeys[i].lpSubKey);
		if (result == ERROR_SUCCESS) {
			printf("成功删除注册表项: %s\n", registryKeys[i].lpSubKey);
		} else {
			printf("删除注册表项失败: %s, 错误码: %ld\n", registryKeys[i].lpSubKey, result);
		}
	}

	/*	for (i = 0; i < sizeof(processes) / sizeof(processes[0]); ++i) {	结束每个进程
		TerminateProcessByName(processes[i]);
	}
	*/

	for (i = 0; i < sizeof(folders) / sizeof(folders[0]); ++i) {
		TerminateProcessesInDirectory(folders[i].path);
	}

	DeleteWindwosService(serviceName);	/* 调用 deleteService 函数删除服务 */

	for (i = 0; i < sizeof(folders) / sizeof(folders[0]); ++i) {	/* 循环删除所有文件夹及其内容 */
		DeleteFolderContents(folders[i].path);
	}

	return 0;
}

