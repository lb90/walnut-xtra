#include "common_wmi.h"
#include "util.h"

#include <windows.h>

#pragma comment(lib, "wbemuuid.lib")

bool WMIWrapper::is_init = false;
CComPtr<IWbemLocator> WMIWrapper::locator {};
CComPtr<IWbemServices> WMIWrapper::server {};

void WMIWrapper::init() {
	HRESULT hr = S_OK;

	if (is_init)
		return;

	hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);

	hr =  ::CoInitializeSecurity(
        NULL,                        // Security descriptor    
        -1,                          // COM negotiates authentication service
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities of the client or server
        NULL);                       // Reserved

	hr = ::CoCreateInstance(
	    CLSID_WbemLocator,
	    0,
	    CLSCTX_INPROC_SERVER,
	    IID_IWbemLocator,
	    (LPVOID*) &locator);

	hr = locator->ConnectServer(
	    L"ROOT\\CIMV2",
	    NULL, NULL, 0,
	    NULL, 0, 0,
	    &server);

	hr = ::CoSetProxyBlanket(
	    server, 
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE, 
        NULL, 
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

	is_init= true;
}

void WMIWrapper::deinit() {
	is_init = false;
}

WMIWrapper::WMIWrapper() {
	if (!is_init)
		init();
}

WMIWrapper::~WMIWrapper() {

}

bool WMIWrapper::execQuery(LPCTSTR query) {
	return true;
}

std::string WMIWrapper::getTextProp(LPCTSTR prop) {
	return "";
}
