#include <windows.h>
#include <stdio.h>
#include "Function.h"

int DeleteWindwosService(char* serviceName) {	/* 定义 deleteService 函数 */

	SC_HANDLE scManager;
	SC_HANDLE scService;

	scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);	/* Step 1: 打开SCManager */
	if (scManager == NULL) {
		printf("无法打开SCManager.\n");
		return -1;
	}

	scService = OpenService(scManager, serviceName, SERVICE_ALL_ACCESS);	/* Step 2: 打开服务 */
	if (scService == NULL) {
		printf("无法打开服务 %s.\n", serviceName);
		CloseServiceHandle(scManager);
		return -1;
	}

	if (!DeleteService(scService)) {	/* Step 3: 删除服务 */
		printf("删除服务失败 %s.\n", serviceName);
	} else {
		printf("服务 %s 已成功删除.\n", serviceName);
	}

	CloseServiceHandle(scService);	/* Step 4: 关闭句柄 */
	CloseServiceHandle(scManager);
	return 0;
}

