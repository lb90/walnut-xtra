#include <iostream>
#include <string>

__declspec(dllimport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret);
__declspec(dllimport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              std::string& ret);

int main()
{
	std::string file_name_utf_8 = "C:\\Windows";
	std::string ret = "";
	test_hazel_get(file_name_utf_8, ret);

	std::cout << "Testing Hazel\n\n";
	std::cout << "Argument: \"" << file_name_utf_8 << "\": " << ret << "\n";

	return 0;
}
