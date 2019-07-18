#include "wmi.h"
#include "log.h"

#include <windows.h>

#pragma comment(lib, "wbemuuid.lib")

/*std::thread WMIWrapper::wmi_thread {};*/
bool WMIWrapper::is_init = false;

void WMIWrapper::initialize() {
	HRESULT hr = S_OK;

	if (is_init)
		return;

	hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr) && hr != S_FALSE && hr != RPC_E_CHANGED_MODE) {
		Log::print_error_hr(L"Cannot initialize COM", hr);
		return;
	}

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
	if (FAILED(hr) && hr != RPC_E_TOO_LATE) {
		Log::print_error_hr(L"Cannot set COM security process-wide", hr);
		return;
	}

	is_init = true;
}

void WMIWrapper::finalize() {
	CoUninitialize();

	is_init = false;
}

WMIWrapper::WMIWrapper() {
	if (!is_init)
		initialize();
}

WMIWrapper::~WMIWrapper() {

}

bool WMIWrapper::ExecQuery(const std::wstring& query) {
	HRESULT hr = S_OK;

	hr = ::CoCreateInstance(
	    CLSID_WbemLocator,
	    0,
	    CLSCTX_INPROC_SERVER,
	    IID_IWbemLocator,
	    (LPVOID*) &locator);
	if (FAILED(hr)) {
		Log::print_error_hr(L"Cannot create WbemLocator COM object (WMI)", hr);
		return false;
	}

	hr = locator->ConnectServer(
	    L"ROOT\\CIMV2",
	    NULL, NULL, 0,
	    NULL, 0, 0,
	    &server);
	if (FAILED(hr)) {
		Log::print_error_hr(L"Cannot connect to WbemLocator server (WMI)", hr);
		return false;
	}

	hr = ::CoSetProxyBlanket(
	    server, 
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE, 
        NULL, 
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);
	if (FAILED(hr)) {
		Log::print_error_hr(L"Cannot set WbemLocator proxy blanket (WMI)", hr);
		return false;
	}

	hr = server->ExecQuery(
		bstr_t(L"WQL"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&enumerator);
	if (FAILED(hr)) {
		Log::print_error_hr(L"Cannot execute WMI query", hr);
		return false;
	}

	ULONG ul_ret = 0;
	hr = enumerator->Next(WBEM_INFINITE, 1, &obj, &ul_ret);
	if (FAILED(hr)) {

	}

	return true;
}

std::string WMIWrapper::GetTextProperty(const std::wstring& property) {
	HRESULT hr = S_OK;
	variant_t variant;

	hr = obj->Get(property.c_str(), 0, &variant, 0, 0);
	if (FAILED(hr)) {

	}

	_bstr_t bstr(variant.bstrVal);
	return std::string((LPCSTR)bstr);
}
