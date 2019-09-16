#include "xtra.h"
#include "log.h"

void xtra_initialize() {
	Log::initialize();
}

int xtra_process(const std::string& argument, const std::string& ret) {

}

void xtra_finalize() {
	Log::finalize();
}
