#include "clipboard.h"
#include "window.h"
#include "log.h"

void Clipboard::CopyToClipboard(const std::string& text) {
	HGLOBAL movable_mem {};
	LPSTR buffer = NULL;

	if (!Window::IsInit()) {
		Log::print(L"Impossibile copiare nella clipboard: la finestra di soli messaggi non è inizializzata");
		return;
	}

	if (OpenClipboard(Window::Hwnd()) == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile aprire la clipboard", error_code);
		return;
	}
	if (EmptyClipboard() == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile azzerare la clipboard", error_code);
		CloseClipboard();
		return;
	}
	movable_mem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, text.size() + 1);
	if (movable_mem == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile allocare memoria con GlobalAlloc", error_code);
		CloseClipboard();
		return;
	}
	buffer = (LPSTR) GlobalLock(movable_mem);
	if (buffer == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile aprire memoria con GlobalLock", error_code);
		GlobalFree(movable_mem);
		CloseClipboard();
		return;
	}
	memcpy(buffer, text.c_str(), text.size());
	buffer[text.size()] = 0;
	GlobalUnlock(movable_mem);

	if (SetClipboardData(CF_TEXT, (HANDLE)movable_mem) == NULL) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile impostare la clipboard", error_code);
		GlobalFree(movable_mem);
		CloseClipboard();
		return;
	}
	if (CloseClipboard() == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Impossibile chiudere la clipboard", error_code);
	}
}
