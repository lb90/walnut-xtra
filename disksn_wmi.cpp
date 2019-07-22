#include "disksn_wmi.h"
#include "wmi.h"
#include "mangler.h"

int get_disk_sn_wmi(std::string& sn) {
	static std::string cache;
	WMIWrapper wmi {};

	if (!cache.empty()) {
		sn = cache;
		return 0;
	}

	sn = "1000001";

	bool success = wmi.ExecQuery(L"SELECT SerialNumber FROM Win32_PhysicalMedia");
	if (!success) {
		sn = "1000001";
		return -1;
	}
	bool got_sn = wmi.GetTextProperty(L"SerialNumber", sn);
	if (got_sn) {
		if (sn.empty()) {
			sn = "1111111";
			return 0;
		}
	}

	detect_and_decode_hex(sn);
	mangle_serial_number(sn);
	cache = sn;

	return 0;
}