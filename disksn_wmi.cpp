#include "disksn_wmi.h"
#include "wmi.h"

int get_disk_sn_wmi(std::string& sn) {
	WMIWrapper wmi {};
	std::string date_prop;

	wmi.ExecQuery(L"SELECT SerialNumber FROM Win32_PhysicalMedia");
	sn = wmi.GetTextProperty(L"SerialNumber");

	return 0;
}