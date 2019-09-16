#include "hazel.h"
#include "log.h"

void xtra_initialize() {
	Log::initialize();
}

int xtra_hazel_get(const std::string& file_name_utf_8,
                   std::string& ret)
{

}

int xtra_hazel_set(const std::string& file_name_utf_8,
                   const std::string& mode_string,
                   std::string& ret)
{

}

void xtra_finalize() {
	Log::finalize();
}
