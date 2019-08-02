#include "clipboard.h"
#include "window.h"
#include "log.h"

void Clipboard::CopyToClipboard(const std::string& text) {
	HGLOBAL movable_mem {};
	LPSTR buffer = NULL;

	Window::initialize();

	if (OpenClipboard(Window::Hwnd()) == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile aprire la clipboard", error_code);
		goto cleanup;
	}
	if (EmptyClipboard() == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile azzerare la clipboard", error_code);
		goto cleanup;
	}
	movable_mem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, text.size());
	if (movable_mem == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile allocare memoria con GlobalAlloc", error_code);
		goto cleanup;
	}
	buffer = (LPSTR) GlobalLock(movable_mem);
	if (buffer == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile aprire memoria con GlobalLock", error_code);
		goto cleanup;
	}
	memcpy(buffer, text.c_str(), text.size());
	buffer[text.size()] = 0;
	GlobalUnlock(movable_mem);

	if (SetClipboardData(CF_TEXT, (HANDLE)movable_mem) == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile impostare la clipboard", error_code);
		goto cleanup;
	}
	if (CloseClipboard() == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile chiudere la clipboard", error_code);
		goto cleanup;
	}

cleanup:
	return;
}
