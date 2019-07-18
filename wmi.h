#pragma once

#define _WIN32_DCOM
#include <comdef.h>
#include <wbemidl.h>
#include <atlbase.h>

#include <string>
/*#include <thread>*/

class WMIWrapper final {
public:
	WMIWrapper();
	~WMIWrapper();

	WMIWrapper(const WMIWrapper&) = delete;
	void operator=(const WMIWrapper&) = delete;

	static void initialize();
	static void finalize();

	bool ExecQuery(const std::wstring& query);
	std::string GetTextProperty(const std::wstring& property);

private:
	CComPtr<IWbemLocator> locator;
	CComPtr<IWbemServices> server;
	CComPtr<IEnumWbemClassObject> enumerator;
	CComPtr<IWbemClassObject> obj;

private:
	static bool is_init;
	/*static std::thread wmi_thread;*/
};
