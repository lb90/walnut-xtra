#include "hazel.h"
#include "util.h"
#include "log.h"

#include <windows.h>

#include <vector>

#ifdef _DEBUG
#include <iostream>
#endif

CRITICAL_SECTION file_lock_handles_lock;
std::vector<HANDLE> file_lock_handles;

void xtra_initialize() {
	static bool once = false;
	if (!once) {
		InitializeCriticalSection(&file_lock_handles_lock);
		Log::initialize();
		once = true;
	}
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

	ret += (attributes & FILE_ATTRIBUTE_READONLY) ? 'r' : 'w';
	ret += (attributes & FILE_ATTRIBUTE_HIDDEN) ? 'h' : 'v';

	HANDLE handle_file = CreateFileW(file_name_utf_16.c_str(),
	                                 DELETE,
	                                 FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
	                                 NULL, /* NULL security attributes */
	                                 OPEN_EXISTING,
	                                 FILE_ATTRIBUTE_NORMAL,
	                                 NULL);
	if (handle_file == INVALID_HANDLE_VALUE) {
		DWORD error_code = GetLastError();
		ret += (error_code == ERROR_SHARING_VIOLATION) ? 'l' : 's';
	}
	else {
		ret += 's';
		CloseHandle(handle_file);
	}
}

void xtra_hazel_set(const std::string& file_name_utf_8,
                    const std::string& mode_string,
                    int& ret)
{
	ret = 1;

	std::wstring file_name_utf_16 = utf_8_to_utf_16(file_name_utf_8);
	if (file_name_utf_16.empty()) {
		ret = 0;
		return;
	}

	DWORD attributes = GetFileAttributesW(file_name_utf_16.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le lettura degli attributi del file", error_code);
		ret = 0;
		return;
	}

	DWORD new_attributes = attributes;
	int set_locked = -1;
	if (mode_string.empty()) {
		/* Se la mode_string è vuota togli tutti gli attributi e il lock (azzera tutto) */
		new_attributes &= ~(FILE_ATTRIBUTE_READONLY
		                    | FILE_ATTRIBUTE_HIDDEN);
		set_locked = 0;
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
			else if (a == 'l')
				set_locked = 1;
			else if (a == 's')
				set_locked = 0;
		}
	}
	if (SetFileAttributesW(file_name_utf_16.c_str(), new_attributes) == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le scrittura degli attributi del file", error_code);
		ret = 0;
	}

	if (set_locked == 1) {
		HANDLE handle_file = CreateFileW(file_name_utf_16.c_str(),
		                                 GENERIC_READ,
		                                 FILE_SHARE_READ,
		                                 NULL, /* NULL security attributes */
		                                 OPEN_EXISTING,
		                                 FILE_ATTRIBUTE_NORMAL, /* ignored with OPEN_EXISTING */
		                                 NULL);
		if (handle_file == INVALID_HANDLE_VALUE) {
			DWORD error_code = GetLastError();
			if (error_code == ERROR_SHARING_VIOLATION) {
				/* Il file è già locked, ma non da Hazel. Ok, non è un errore */
			}
			else {
				Log::print_error_code(L"Errore durante l'apertura del file per locking", error_code);
				ret = 0;
			}
		}
		else {
			/* Tutto liscio. Controlla se l'abbiamo già nella lista e in caso chiudi questo handle.
			   Altrimento salva questo handle nella lista */
			EnterCriticalSection(&file_lock_handles_lock);

			BY_HANDLE_FILE_INFORMATION bhfi{};
			if (GetFileInformationByHandle(handle_file, &bhfi) == 0) {
				DWORD error_code = GetLastError();
				Log::print_error_code(L"Errore in GetFileInformationByHandle", error_code);
				ret = 0;
				CloseHandle(handle_file);
			}
			else {
				bool found = false;
				for (std::vector<HANDLE>::iterator it = file_lock_handles.begin();
					it != file_lock_handles.end();
					++it)
				{
					BY_HANDLE_FILE_INFORMATION bhfi2{};
					if (GetFileInformationByHandle(*it, &bhfi2)) {
						if ((bhfi.nFileIndexHigh == bhfi2.nFileIndexHigh)
							&& (bhfi.nFileIndexLow == bhfi2.nFileIndexLow)
							&& (bhfi.dwVolumeSerialNumber == bhfi2.dwVolumeSerialNumber))
						{
							found = true;
							break;
						}
					}
				}
				if (found)
					CloseHandle(handle_file);
				else
					file_lock_handles.emplace_back(handle_file);
			}

			LeaveCriticalSection(&file_lock_handles_lock);
		}
	}
	else if (set_locked == 0) {
		HANDLE handle_file = CreateFileW(file_name_utf_16.c_str(),
		                                 GENERIC_READ,
		                                 FILE_SHARE_READ,
		                                 NULL, /* NULL security attributes */
		                                 OPEN_EXISTING,
		                                 FILE_ATTRIBUTE_NORMAL, /* ignored with OPEN_EXISTING */
		                                 NULL);
		if (handle_file == INVALID_HANDLE_VALUE) {
			DWORD error_code = GetLastError();
			Log::print_error_code(L"Errore durante l'apertura del file per rimuovere lock", error_code);
			ret = 0;
		}
		BY_HANDLE_FILE_INFORMATION bhfi{};
		if (GetFileInformationByHandle(handle_file, &bhfi) == 0) {
			DWORD error_code = GetLastError();
			Log::print_error_code(L"Errore in GetFileInformationByHandle", error_code);
			ret = 0;
		}
		else {
			EnterCriticalSection(&file_lock_handles_lock);
			std::vector<HANDLE>::iterator it = file_lock_handles.begin();
			while (it != file_lock_handles.end())
			{
				BY_HANDLE_FILE_INFORMATION bhfi2{};
				if (GetFileInformationByHandle(*it, &bhfi2)) {
					if ( (bhfi.nFileIndexHigh == bhfi2.nFileIndexHigh)
						 && (bhfi.nFileIndexLow == bhfi2.nFileIndexLow)
						 && (bhfi.dwVolumeSerialNumber == bhfi2.dwVolumeSerialNumber) )
					{
						CloseHandle(*it);
						file_lock_handles.erase(it);
						break;
					}
				}
				++it;
			}
			LeaveCriticalSection(&file_lock_handles_lock);
		}
		CloseHandle(handle_file);
	}
}

void xtra_finalize() {
	static bool once = false;
	if (!once) {
		Log::finalize();
		DeleteCriticalSection(&file_lock_handles_lock);
		once = true;
	}
}

#ifdef _DEBUG
__declspec(dllexport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret)
{
	xtra_initialize();
	xtra_hazel_get(file_name_utf_8, ret);
	std::cout << "[GET] Argument: \"" << file_name_utf_8 << "\" Attributes: " << ret << "\n";
}
__declspec(dllexport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              int& ret)
{
	xtra_initialize();
	xtra_hazel_set(file_name_utf_8, mode_string, ret);
	std::cout << "[SET] Argument: \"" << file_name_utf_8 << "\" ModeString: " << mode_string << " Return: " << ret << "\n";
}
#endif
