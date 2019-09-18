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
	std::string file_name_utf_8;
	std::string get_ret;
	std::string mode_string;
	int set_ret = 0;

	std::cout << "Testing Hazel\n\n";

	file_name_utf_8 = "C:\\Users\\luca9\\testo.txt";

	/* Leggi gli attributi iniziali */
	test_hazel_get(file_name_utf_8, get_ret);

	std::string original_attributes = get_ret;

	/* Prova a settare tutti e quattro gli attributi in una sola chiamata */
	mode_string = "rahs";
	test_hazel_set(file_name_utf_8, mode_string, set_ret);

	/* Leggi gli attributi per verificare la precedente operazione Set */
	test_hazel_get(file_name_utf_8, get_ret);

	/* Prova a chiamare Set con una stringa vuota (rimuove tutti e quattro gli attributi) */
	mode_string = "";
	test_hazel_set(file_name_utf_8, mode_string, set_ret);

	/* Leggi gli attributi per verificare la precedente operazione Set */
	test_hazel_get(file_name_utf_8, get_ret);

	/* Setta gli attributi originali */
	mode_string = original_attributes;
	test_hazel_set(file_name_utf_8, mode_string, set_ret);

	/* Verifica con un Get */
	test_hazel_get(file_name_utf_8, get_ret);
}

void test_file_locking() {
	std::string file_name;
	std::string get;
	std::string mode;
	int set = 0;

	test_hazel_get("C:\\Windows\\System32\\Kernel32.dll", get);

	test_hazel_set("C:\\Users\\luca9\\testo.txt", "rhl", set);

	test_hazel_get("C:\\Users\\luca9\\testo.txt", get);

	test_hazel_set("C:\\Users\\luca9\\testo.txt", "wvs", set);

	test_hazel_get("C:\\Users\\luca9\\testo.txt", get);
}

void test_file_locking_advanced() {
	std::string file_name;
	std::string get;
	std::string mode;
	int set = 0;

	test_hazel_set("C:\\Users\\luca9\\testo.txt", "l", set);

	test_hazel_get("C:\\Users\\luca9\\testo.txt", get);

	test_hazel_set("C:\\Users\\luca9\\testo.txt", "l", set);

	test_hazel_get("C:\\Users\\luca9\\testo.txt", get);

	test_hazel_set("C:\\Users\\luca9\\testo.txt", "s", set);

	test_hazel_get("C:\\Users\\luca9\\testo.txt", get);
}

int main()
{
	test_file_locking();
	test_file_locking_advanced();

	/* Fine */
	return 0;
}
