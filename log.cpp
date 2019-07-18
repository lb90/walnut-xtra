#include "log.h"

#include <comdef.h>
#include <atlbase.h>

#include <iostream>

/* Helper class for RAII */
class SystemErrorCodeString {
public:
	SystemErrorCodeString(DWORD code) {
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)string, 0, NULL);
	}
	~SystemErrorCodeString() {
		if (string) {
			LocalFree(string);
			string = NULL;
		}
	}
	SystemErrorCodeString(const SystemErrorCodeString&) = delete;
	void operator=(const SystemErrorCodeString&) = delete;

	bool IsValid() {
		return string && wcslen((wchar_t*)string);
	}
	LPCWSTR Get() {
		return (LPCWSTR) string;
	}
private:
	LPVOID string {NULL};
};
/**/

std::wofstream Log::stream {};

void Log::initialize() {
	CRegKey reg_key {};
	LONG ret = 0;
	ret = reg_key.Open(HKEY_CURRENT_USER, _T("Software\\Walnut"), KEY_READ);
	if (ret == ERROR_SUCCESS) {
		WCHAR temp_dir_path[MAX_PATH+2] {};
		GetTempPath(MAX_PATH+2, temp_dir_path);
		std::wstring path = temp_dir_path;
		path += L"\\WalnutLog.txt";
		stream.open(path, std::ios::app | std::ios::trunc);
	}
}

void Log::finalize() {
	stream.close();
}

void Log::print(const std::wstring& str) {
	if (!stream) return;

	stream << str << L"\n";
}

void Log::print_error_hr(const std::wstring& str, HRESULT hr) {
	if (!stream) return;

	std::wcout << str;
	if (FAILED(hr)) {
		_com_error err(hr);

		std::wcout << L"\n" << L"HRESULT: " << hr;
		auto error_description = err.ErrorMessage();
		if (error_description && wcslen(error_description)) {
			std::wcout << L"\n" << "Error string: " << error_description;
		}
	}
	std::wcout << L"\n";
}

void Log::print_error_code(const std::wstring& str, DWORD code) {
	if (!stream) return;

	std::wcout << str;
	if (code > 0) {
		SystemErrorCodeString error_string(code);

		std::wcout << L"\n" << L"Error code: " << code;
		if (error_string.IsValid()) {
			std::wcout << L"\n" << "Error string: " << error_string.Get();
		}
	}
	std::wcout << L"\n";
}
