#include "biosdate_wmi.h"
#include "common_wmi.h"

int get_bios_date_wmi(std::string& date) {
	WMIWrapper wmi {};
	std::string date_prop;

	wmi.execQuery(_T("SELECT ReleaseDate FROM Win32_BIOS"));
	date_prop = wmi.getTextProp(_T("ReleaseDate"));

	/*
	 * date_prop is a string formatted as YYYYMMDDHHMMSS.MMMMMM(+-)OOO
	 * https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-bios
	 */

	if (date_prop.length() < 8) {
		date = "10010010"; /*TODO*/
		return 0;
	}

	for (int i = 0; i < 8; i++) {
		if (!isdigit(date_prop[i])) {
			date = "10010010"; /*TODO*/
			return 0;
		}
	}

	std::string year = date_prop.substr(0, 4);
	std::string month = date_prop.substr(4, 2);
	std::string day = date_prop.substr(6, 2);
	date = '1' + day +
	       '1' + month +
	       '1' + year;

	return 0;
}