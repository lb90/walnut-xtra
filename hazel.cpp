#include "hazel.h"
#include "util.h"
#include "log.h"

#include <windows.h>

void xtra_initialize() {
	Log::initialize();
}

void xtra_hazel_get(const std::string& file_name_utf_8,
                    std::string& ret)
{
	ret = "";

	std::wstring file_name_utf_16 = utf_8_to_utf_16(file_name_utf_8);
	if (file_name_utf_16.empty())
		return;

	DWORD attributes = GetFileAttributesW(file_name_utf_16.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le lettura degli attributi del file", error_code);
		return;
	}

	ret += (attributes & FILE_ATTRIBUTE_READONLY) ? "r" : "w";
	ret += (attributes & FILE_ATTRIBUTE_HIDDEN) ? "h" : "v";
}

void xtra_hazel_set(const std::string& file_name_utf_8,
                    const std::string& mode_string,
                    int& ret)
{
	ret = 0;

	std::wstring file_name_utf_16 = utf_8_to_utf_16(file_name_utf_8);
	if (file_name_utf_16.empty())
		return;

	DWORD attributes = GetFileAttributesW(file_name_utf_16.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le lettura degli attributi del file", error_code);
		return;
	}

	DWORD new_attributes = attributes;
	if (mode_string.empty()) {
		/* Se la mode_string è vuota togli tutti gli attributi */
		new_attributes &= ~(FILE_ATTRIBUTE_READONLY
		                    | FILE_ATTRIBUTE_HIDDEN);
	}
	else {
		for (char a : mode_string) {
			if (a == 'r')
				new_attributes |= FILE_ATTRIBUTE_READONLY;
			else if (a == 'w')
				new_attributes &= ~FILE_ATTRIBUTE_READONLY;
			else if (a == 'h')
				new_attributes |= FILE_ATTRIBUTE_HIDDEN;
			else if (a == 'v')
				new_attributes &= ~FILE_ATTRIBUTE_HIDDEN;
		}
	}
	if (SetFileAttributesW(file_name_utf_16.c_str(), new_attributes) == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le scrittura degli attributi del file", error_code);
		return;
	}

	ret = 1;
}

void xtra_finalize() {
	Log::finalize();
}

#ifdef _DEBUG
__declspec(dllexport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret)
{
	xtra_initialize();
	xtra_hazel_get(file_name_utf_8, ret);
}
__declspec(dllexport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              int& ret)
{
	xtra_initialize();
	xtra_hazel_set(file_name_utf_8, mode_string, ret);
}
#endif