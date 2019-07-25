#pragma once

#include <string>

class Walnut {
public:
	static void initialize();
	static void finalize();

	class Constants {
	public:
		static std::string illegal_or_failed_string;
		static std::string not_found_string;
	};
private:
	static bool is_init;
};
