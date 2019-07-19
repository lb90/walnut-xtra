#include "log.h"

#ifdef _DEBUG

#include <comdef.h>
#include <shlobj.h>

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
	PWSTR temp_dir_path = NULL;
	std::wstring path;

	if (stream)
		return;

	try {
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &temp_dir_path);
		path = temp_dir_path;
		path += L"\\WalnutLog.txt";

		std::ios_base::iostate exception_mask;
		exception_mask = stream.exceptions();
		exception_mask |= std::ios::failbit;
		stream.exceptions(exception_mask);
		stream.open(path, std::ios::app);
	}
	catch (std::ios_base::failure& e) {
		OutputDebugStringA(e.what());
	}
	catch (...) {
	}
	if (temp_dir_path) {
		CoTaskMemFree((LPVOID)temp_dir_path);
		temp_dir_path = NULL;
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

	stream << str;
	if (FAILED(hr)) {
		_com_error err(hr);

		stream << L"\n" << L"HRESULT: " << hr;
		auto error_description = err.ErrorMessage();
		if (error_description && wcslen(error_description)) {
			stream << L"\n" << "Error string: " << error_description;
		}
	}
	stream << L"\n";
}

void Log::print_error_code(const std::wstring& str, DWORD code) {
	if (!stream) return;

	stream << str;
	if (code > 0) {
		SystemErrorCodeString error_string(code);

		stream << L"\n" << L"Error code: " << code;
		if (error_string.IsValid()) {
			stream << L"\n" << "Error string: " << error_string.Get();
		}
	}
	stream << L"\n";
}

#else

void Log::initialize() {}
void Log::finalize() {}
void Log::print(const std::wstring& str) {}
void Log::print_error_hr(const std::wstring& str, HRESULT hr) {}
void Log::print_error_code(const std::wstring& str, DWORD code) {}

#endif // _DEBUG
