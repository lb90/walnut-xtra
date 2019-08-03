#pragma once

#include <windows.h>

class Window final {
public:
	static bool initialize();
	static void finalize();

	static HWND Hwnd() { return hwnd; }
	static bool IsInit() { return is_init; }
private:
	static ATOM window_class;
	static LPCWSTR window_class_name;
	static HWND hwnd;

	static bool is_init;
};
