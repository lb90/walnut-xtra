#include "wmi.h"
#include "log.h"

#include <windows.h>

#pragma comment(lib, "wbemuuid.lib")

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
	if (is_init) {
		CoUninitialize();
		is_init = false;
	}
}

WMIWrapper::WMIWrapper() {
	if (!is_init)
		initialize();
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

	query_done = true;

	ULONG ul_ret = 0;
	hr = enumerator->Next(WBEM_INFINITE, 1, &obj, &ul_ret);
	if (FAILED(hr)) {
		Log::print(L"Query is empty");
		query_empty = true;
		return true;
	}

	query_empty = false;
	return true;
}

bool WMIWrapper::GetTextProperty(const std::wstring& property, std::string& result) {
	HRESULT hr = S_OK;
	variant_t variant;

	if (!query_done) {
#ifdef _DEBUG
		throw std::runtime_error("WIMWrapper::GetTextProperty() called with no query ready");
#else
		result = "1111111";
		return false;
#endif
	}

	if (query_empty) {
		result = "1000001";
		return true;
	}

	hr = obj->Get(property.c_str(), 0, &variant, 0, 0);
	if (FAILED(hr)) {
		result = "1111111";
		return false;
	}

	_bstr_t bstr(variant.bstrVal);
	result = (LPCSTR) bstr;
	return true;
}
