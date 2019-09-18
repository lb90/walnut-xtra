#pragma once

#include <string>
#include <windows.h>


std::wstring utf_8_to_utf_16(const std::string& string_utf_8);
std::string utf_16_to_utf_8(const std::wstring& string_utf_16);
bool is_all_ascii(const std::string& string_);
