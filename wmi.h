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

	WMIWrapper(const WMIWrapper&) = delete;
	void operator=(const WMIWrapper&) = delete;

	static void initialize();
	static void finalize();

	bool ExecQuery(const std::wstring& query);
	bool GetTextProperty(const std::wstring& property, std::string& result);

private:
	CComPtr<IWbemLocator> locator;
	CComPtr<IWbemServices> server;
	CComPtr<IEnumWbemClassObject> enumerator;
	CComPtr<IWbemClassObject> obj;
	bool query_done {false}; /* true if query was done */
	bool query_empty {true}; /* true if query result is empty */

private:
	static bool is_init;
};
