#include <iostream>
#include <string>
#include <vector>

/*
__declspec(dllimport)
int __stdcall process(const std::string& argument,
                      std::string& result);
*/
int __stdcall process(const std::string& argument,
                      std::string& result);

int main()
{
	std::vector<std::string> arguments = {
		"disksn"
	};

	for (const std::string& argument : arguments) {
		std::string result;

		process(argument, result);
		std::cout << argument << ": " << result << "\n";
	}

	return 0;
}
