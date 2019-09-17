#include "util.h"
#include "log.h"

#include <vector>

std::wstring utf_8_to_utf_16(const std::string& string_utf_8) {
	if (string_utf_8.empty())
		return L"";

	const int utf_16_length = MultiByteToWideChar(
		CP_UTF8,
		0,
		string_utf_8.c_str(),
		string_utf_8.size(),
		NULL,
		0);
	if (utf_16_length == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore nella conversione UTF8 -> UTF16", error_code);
	}

	std::vector<wchar_t> buffer(utf_16_length+1, 0);
	const int result = MultiByteToWideChar(
		CP_UTF8,
		0,
		string_utf_8.c_str(),
		string_utf_8.size(),
		buffer.data(),
		buffer.size() * sizeof(wchar_t));
	if (result == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore nella conversione UTF8 -> UTF16", error_code);
	}

	std::wstring string_utf_16 = buffer.data();
	return string_utf_16;
}