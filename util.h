#pragma once

#include <windows.h>

void util_print(LPCWSTR str);
void util_print_error_hr(LPCWSTR str, HRESULT hr);
void util_print_error_last_error(LPCWSTR str, DWORD error_code);