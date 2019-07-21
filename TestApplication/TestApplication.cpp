#include <iostream>
#include <string>
#include <vector>

__declspec(dllimport)
int __stdcall walnut_process(const std::string& argument,
                             std::string& result);

int main()
{
	std::vector<std::string> arguments = {
		"disksn",
		"disksn-fast",
		"biosdt-wmi",
		"biosdt",
	};

	for (const std::string& argument : arguments) {
		std::string result;

		walnut_process(argument, result);
		std::cout << argument << ": " << result << "\n";
	}

	return 0;
}
