#include "walnut.h"
#include "wmi.h"
#include "window.h"
#include "log.h"

void walnut_initialize() {
	Log::initialize();
	Window::initialize();
	WMIWrapper::initialize();
}

void walnut_finalize() {
	WMIWrapper::finalize();
	Window::finalize();
	Log::finalize();
}
