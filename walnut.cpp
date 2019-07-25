#include "walnut.h"
#include "wmi.h"
#include "log.h"

std::string Walnut::Constants::illegal_or_failed_string = "1000001";
std::string Walnut::Constants::not_found_string         = "1111111";

void Walnut::initialize() {
	Log::initialize();
	WMIWrapper::initialize();
}

void Walnut::finalize() {
	WMIWrapper::finalize();
	Log::finalize();
}
