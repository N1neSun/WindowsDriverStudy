#include "ntddk.h"
#include "windows.h"

#define SER_NAME _T("FirstDriver")

int main() 
{
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hSer = NULL;
	do 
	{
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (hSCM == NULL)
		{
			break;
		}
		hSer = CreateService(
			hSCM,
			SER_NAME,
			SER_NAME,
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			_T("C:\\FirstDriver.sys"),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
		if (hSer == NULL)
		{
			DWORD dwErrorCode = GetLastError();
			if (dwErrorCode == ERROR_SERVICE_EXISTS)
			{
				hSer = OpenService(hSCM, SER_NAME, SERVICE_ALL_ACCESS);
				if (hSer == NULL)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		printf("CreateService or OpenService suceess \n");
		getchar();

		BOOL bSucc = StartService(hSer, NULL, NULL);
		printf("StartService:%u\n", bSucc);
		getchar();

		SERVICE_STATUS SerStatus = { 0 };
		bSucc = ControlService(hSer, SERVICE_CONTROL_STOP, &SerStatus);
		printf("ControlService:%u\n", bSucc);
		getchar();

		DeleteService(hSer);
	} while (FALSE);
	if (hSCM != NULL)
	{
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}
	if (hSer != NULL)
	{
		CloseServiceHandle(hSCM);
		hSer = NULL;
	}
	return 0;
}

