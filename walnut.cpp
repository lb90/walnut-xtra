#include "xtra.h"
#include "log.h"

void xtra_initialize() {
	Log::initialize();
}

void xtra_finalize() {
	Log::finalize();
}
