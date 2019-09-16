#include "hazel.h"
#include "log.h"

void xtra_initialize() {
	Log::initialize();
}

void xtra_hazel_get(const std::string& file_name_utf_8,
                    std::string& ret)
{

}

void xtra_hazel_set(const std::string& file_name_utf_8,
                    const std::string& mode_string,
                    std::string& ret)
{

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