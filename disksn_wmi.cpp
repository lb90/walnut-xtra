#include "disksn_wmi.h"
#include "common_wmi.h"

int get_disk_sn_wmi(std::string& sn) {
	WMIWrapper wmi {};
	std::string date_prop;

	wmi.execQuery(_T("SELECT SerialNumber FROM Win32_PhysicalMedia"));
	sn = wmi.getTextProp(_T("SerialNumber"));

	return 0;
}