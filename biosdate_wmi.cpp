#include "biosdate_wmi.h"
#include "wmi.h"
#include "mangler.h"

#include "log.h"

int get_bios_date_wmi(std::string& date) {
	static std::string cache;
	WMIWrapper wmi {};

	if (!cache.empty()) {
		date = cache;
		return 0;
	}

	date = "1000001";

	if (!wmi.ExecQuery(L"SELECT ReleaseDate FROM Win32_BIOS")) {
		date = "1000001";
		return -1;
	}
	date = wmi.GetTextProperty(L"ReleaseDate");
	if (date.empty()) {
		date = "1111111";
		return 0;
	}

	/*
	 * date_prop is a string formatted as YYYYMMDDHHMMSS.MMMMMM(+-)OOO
	 * https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-bios
	 */

	mangle_date(date);
	cache = date;

	return 0;
}