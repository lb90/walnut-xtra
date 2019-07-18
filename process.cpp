#include "process.h"

#include "biosdate_wmi.h"
#include "disksn_wmi.h"
#include "disksn_ioctl.h"

int process(const std::string& argument, std::string& result) {
	std::string arg = argument;
	int ret = 0;

	result = "";

	if (arg == "biosdt") {
		get_bios_date_wmi(result);
	}
	else if (arg == "biosdt-wmi") {
		get_bios_date_wmi(result);
	}
	else if (arg == "disksn") {
		get_disk_sn_wmi(result);
	}
	else if (arg == "disksn-wmi") {
		get_disk_sn_wmi(result);
	}
	else if (arg == "disksn-fast") {
		get_disk_sn_ioctl(result);
	}
	else return ret;

	/*TODO mangle*/
}
