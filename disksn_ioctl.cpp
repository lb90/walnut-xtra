#include "disksn_ioctl.h"
#include "log.h"

#include <windows.h>

int get_disk_sn_ioctl(std::string& sn) {
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	STORAGE_PROPERTY_QUERY spq {};
	STORAGE_DESCRIPTOR_HEADER sdh {};
	BYTE *buffer = NULL;
	STORAGE_DEVICE_DESCRIPTOR *desc = NULL;
	DWORD dw_ret = 0;
	int ret = -1;

	hDevice = CreateFile(L"\\\\.\\PhysicalDrive0", 0,
	                     FILE_SHARE_READ | FILE_SHARE_WRITE,
	                     NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == NULL || hDevice == INVALID_HANDLE_VALUE) {
		DWORD last_error = GetLastError();
		Log::print_error_code(L"Cannot open PhysicalDrive0.", last_error);
		goto cleanup;
	}

	spq.PropertyId = StorageDeviceProperty;
	spq.QueryType = PropertyStandardQuery;

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
	                       &spq, sizeof(spq), &sdh, sizeof(sdh),
	                       &dw_ret, NULL))
	{
		DWORD last_error = GetLastError();
		Log::print_error_code(L"Cannot query PhysicalDrive properties using DeviceIoControl.", last_error);
		goto cleanup;
	}

	buffer = new BYTE[sdh.Size];
	ZeroMemory(buffer, sdh.Size);

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
	                       &spq, sizeof(spq), buffer,sdh.Size,
	                       &dw_ret, NULL))
	{
		DWORD last_error = GetLastError();
		Log::print_error_code(L"Cannot query PhysicalDrive properties using DeviceIoControl.", last_error);
		goto cleanup;
	}

	desc = (STORAGE_DEVICE_DESCRIPTOR*) buffer;
	if (desc->SerialNumberOffset == 0) {
		Log::print(L"Cannot find serial number for PhysicalDrive0.");
	}

	sn = ((LPCSTR)desc) + desc->SerialNumberOffset;

	ret = 0;
cleanup:
	if (buffer) {
		delete[] buffer;
	}
	if (hDevice != NULL && hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(hDevice);
	}

	return ret;
}