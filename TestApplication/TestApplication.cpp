#include <iostream>
#include <string>

__declspec(dllimport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret);
__declspec(dllimport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              int& ret);

void test_attributes() {
	const std::string path = "C:\\Users\\luca9\\testo.txt";
	std::string get;
	int set = 0;
	std::string original_attributes;

	std::cout << "Testing Hazel\n\n";

	test_hazel_get(path, get);
	original_attributes = get;

	test_hazel_set(path, "rahs", set);

	test_hazel_get(path, get);

	test_hazel_set(path, "", set);

	test_hazel_get(path, get);

	test_hazel_set(path, original_attributes, set);

	test_hazel_get(path, get);
}

void test_file_locking() {
	const std::string path = "C:\\Users\\luca9\\testo.txt";
	std::string get;
	std::string mode;
	int set = 0;

	test_hazel_get("C:\\Windows\\System32\\Kernel32.dll", get);

	test_hazel_set(path, "rhl", set);

	test_hazel_get(path, get);

	test_hazel_set(path, "wvs", set);

	test_hazel_get(path, get);
}

void test_file_locking_advanced() {
	const std::string path = "C:\\Users\\luca9\\testo.txt";
	std::string get;
	int set = 0;

	test_hazel_set(path, "l", set);

	test_hazel_get(path, get);

	test_hazel_set(path, "l", set);

	test_hazel_get(path, get);

	test_hazel_set(path, "s", set);

	test_hazel_get(path, get);
}

void test_file_locking2() {
	const std::string path = "C:\\Users\\luca9\\testo.txt";
	std::string get;
	int set = 0;

	test_hazel_set(path, "r", set);
	test_hazel_get(path, get);

	test_hazel_set(path, "l", set);
	test_hazel_get(path, get);

	test_hazel_set(path, "s", set);
	test_hazel_get(path, get);

	test_hazel_set(path, "w", set);
	test_hazel_get(path, get);
}


int main()
{
	test_file_locking();
	test_file_locking_advanced();
	test_file_locking2();

	/* Fine */
	return 0;
}
