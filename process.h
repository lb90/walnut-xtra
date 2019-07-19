#pragma once

#include <string>

#ifdef _DEBUG
#define DEBUG_EXPORT __declspec(dllexport) __stdcall
#else
#define DEBUG_EXPORT __stdcall
#endif

int DEBUG_EXPORT process(const std::string& arg, std::string& result);
