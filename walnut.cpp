#include "walnut.h"
#include "wmi.h"
#include "log.h"

void walnut_initialize() {
	Log::initialize();
	WMIWrapper::initialize();
}

void walnut_finalize() {
	WMIWrapper::finalize();
	Log::finalize();
}