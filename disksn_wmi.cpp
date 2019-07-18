#include "disksn_wmi.h"
#include "wmi.h"

int get_disk_sn_wmi(std::string& sn) {
	WMIWrapper wmi {};
	std::string date_prop;

	if (!wmi.ExecQuery(L"SELECT SerialNumber FROM Win32_PhysicalMedia")) {
		sn = "1000001";
		return -1;
	}
	sn = wmi.GetTextProperty(L"SerialNumber");
	if (sn.empty())
		sn = "1111111";

	return 0;
}