#pragma once

#define _WIN32_DCOM
#include <comdef.h>
#include <wbemidl.h>
#include <atlbase.h>

#include <string>

class WMIWrapper final {
public:
	WMIWrapper();
	~WMIWrapper();

	WMIWrapper(const WMIWrapper&) = delete;
	void operator=(const WMIWrapper&) = delete;

	bool execQuery(LPCTSTR query);
	std::string getTextProp(LPCTSTR prop);

private:
	static CComPtr<IWbemLocator> locator;
	static CComPtr<IWbemServices> server;

private:
	static void init();
	static void deinit();

	static bool is_init;
};
