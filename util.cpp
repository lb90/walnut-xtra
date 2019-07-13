#include "util.h"

#include <comdef.h>
#include <cstring>

#include <iostream> /*TODO*/

void util_print(LPCWSTR str) {
	std::wcout << str << L"\n";
}

void util_print_error_hr(LPCWSTR str, HRESULT hr) {
	std::wcout << str;
	if (FAILED(hr)) {
		std::wcout << L"\n" << L"HRESULT: " << hr;
		_com_error err(hr);
		auto error_description = err.ErrorMessage();
		if (error_description && wcslen(error_description)) {
			std::wcout << L"\n" << "Error string: " << error_description;
		}
	}
	std::wcout << L"\n";
}

void util_print_error_last_error(LPCWSTR str, DWORD error_code) {
	LPCWSTR error_string = NULL;
	std::wcout << str;
	if (error_code > 0) {
		std::wcout << L"\n" << L"Error code: " << error_code;
		error_string = get_win32_error_code_description(error_code);
		if (error_string && wcslen(error_string) > 0) {
			std::wcout << L"\n" << "Error string: " << error_string;
		}
	}
	std::wcout << L"\n";
}



/*
 * Usage:
 * Free returned value with LocalFree()
 */
static
LPCWSTR get_win32_error_code_description(DWORD error_code) {
	LPVOID msg_buffer = NULL;
	DWORD ret = 0;

	ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
	                    FORMAT_MESSAGE_FROM_SYSTEM |
	                    FORMAT_MESSAGE_IGNORE_INSERTS,
	                    NULL, error_code,
	                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	                    (LPTSTR) &msg_buffer, 0, NULL);
	if (ret == 0)
		return NULL;

	return (LPCWSTR) msg_buffer;
}
