#include "hazel.h"
#include "util.h"
#include "log.h"

#include <windows.h>

void xtra_initialize() {
	Log::initialize();
}

/*TODO:
  Soggetto alle stesse limitazioni riportate in xtra_hazel_set()
*/
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

	if (attributes & FILE_ATTRIBUTE_READONLY)
		ret += "r";
	if (attributes & FILE_ATTRIBUTE_ARCHIVE)
		ret += "a";
	if (attributes & FILE_ATTRIBUTE_HIDDEN)
		ret += "h";
	if (attributes & FILE_ATTRIBUTE_SYSTEM)
		ret += "s";
}

/*TODO:
1) Non supporta path oltre il limite di MAX_PATH. Per farlo funzionare oltre
   questo limite è possibile:
   a) Usare Windows 10 Anniversary Update o successivi e abilitare il supporto
	  a percorsi lunghi nel registro di sistema
   b) Passare, da Lingo, un path con "\\?\" preposto. Ad esempio:
	  HazPeaSet("\\?\C:\Documents And Settings\Users"", "h")
	  Notare che in questo modo il normale processing dei percorsi di Windows
	  non viene eseguito e perciò non è possibile usare percorsi relativi,
	  forward slash '/' e altro.
2) L'uso di GetFileAttributes e SetFileAttributes è soggetto a race condition.
   Per ovviare a questo è possibile usare CreateFile e GetFileInformationByHandle.
*/
void xtra_hazel_set(const std::string& file_name_utf_8,
                    const std::string& mode_string,
                    int& ret)
{
	ret = 0;

	std::wstring file_name_utf_16 = utf_8_to_utf_16(file_name_utf_8);
	if (file_name_utf_16.empty())
		return;

	DWORD attributes = GetFileAttributesW(file_name_utf_16.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le lettura degli attributi del file", error_code);
		return;
	}

	DWORD new_attributes = attributes;
	if (mode_string.find('r') != std::string::npos)
		new_attributes |= FILE_ATTRIBUTE_READONLY;
	if (mode_string.find('a') != std::string::npos)
		new_attributes |= FILE_ATTRIBUTE_ARCHIVE;
	if (mode_string.find('h') != std::string::npos)
		new_attributes |= FILE_ATTRIBUTE_HIDDEN;
	if (mode_string.find('s') != std::string::npos)
		new_attributes |= FILE_ATTRIBUTE_SYSTEM;
	if (SetFileAttributesW(file_name_utf_16.c_str(), new_attributes) == 0) {
		DWORD error_code = GetLastError();
		Log::print_error_code(L"Errore durante le scrittura degli attributi del file", error_code);
		return;
	}

	ret = 1;
}

void xtra_finalize() {
	Log::finalize();
}

#ifdef _DEBUG
__declspec(dllexport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret)
{
	xtra_initialize();
	xtra_hazel_get(file_name_utf_8, ret);
}
__declspec(dllexport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              std::string& ret)
{
	xtra_initialize();
	xtra_hazel_set(file_name_utf_8, mode_string, ret);
}
#endif