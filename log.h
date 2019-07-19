#pragma once

#include <Windows.h>

#include <string>
#include <fstream>
#include <memory>

class Log {
public:
	static void initialize();
	static void finalize();

	static void print(const std::wstring& str);
	static void print_error_hr(const std::wstring& str, HRESULT hr);
	static void print_error_code(const std::wstring& str, DWORD code);

private:
#ifdef _DEBUG
	static std::wofstream stream;
#endif
};