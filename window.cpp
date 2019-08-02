#include "window.h"
#include "log.h"

ATOM Window::window_class = 0;
LPCWSTR Window::window_class_name = L"WalnutMessageWindowCLASS";
HWND Window::hwnd = NULL;

bool Window::is_init = false;

bool Window::initialize() {
	WNDCLASSEX wndclassex {};

	if (is_init)
		return true;

	is_init = false;

	wndclassex.cbSize = sizeof(wndclassex);
	wndclassex.lpszClassName = window_class_name;
	wndclassex.lpfnWndProc = DefWindowProc;
	wndclassex.hInstance = GetModuleHandle(NULL);

	window_class = RegisterClassEx(&wndclassex);
	if (window_class == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile registrare classe per finestra di soli messaggi", error_code);
		goto cleanup;
	}

	hwnd = CreateWindowEx(0, window_class_name, L"Walnut Message Window",
	                      0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hwnd == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile creare finestra di soli messaggi", error_code);
		goto cleanup;
	}

	is_init = true;

cleanup:
	if (!is_init)
		finalize();

	return is_init;
}

void Window::finalize() {
	if (hwnd) {
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	if (window_class) {
		UnregisterClass(window_class_name, GetModuleHandle(NULL));
		window_class = 0;
	}

	is_init = false;
}
