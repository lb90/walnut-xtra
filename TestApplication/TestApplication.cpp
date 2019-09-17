#include <iostream>
#include <string>

__declspec(dllimport)
void __stdcall test_hazel_get(const std::string& file_name_utf_8,
                              std::string& ret);
__declspec(dllimport)
void __stdcall test_hazel_set(const std::string& file_name_utf_8,
                              const std::string& mode_string,
                              int& ret);

int main()
{
	std::string file_name_utf_8;
	std::string get_ret;
	std::string mode_string;
	int set_ret = 0;

	std::cout << "Testing Hazel\n\n";

	file_name_utf_8 = "C:\\Users\\luca9\\testo.txt";

	/* Leggi gli attributi iniziali */
	test_hazel_get(file_name_utf_8, get_ret);
	std::cout << "[GET] Argument: \"" << file_name_utf_8 << "\" Attributes: " << get_ret << "\n";

	std::string original_attributes = get_ret;

	/* Prova a settare tutti e quattro gli attributi in una sola chiamata */
	mode_string = "rahs";
	test_hazel_set(file_name_utf_8, mode_string, set_ret);
	std::cout << "[SET] Argument: \"" << file_name_utf_8 << "\" ModeString: " << mode_string << " Return: " << set_ret << "\n";

	/* Leggi gli attributi per verificare la precedente operazione Set */
	test_hazel_get(file_name_utf_8, get_ret);
	std::cout << "[GET] Argument: \"" << file_name_utf_8 << "\" Attributes: " << get_ret << "\n";

	/* Prova a chiamare Set con una stringa vuota (rimuove tutti e quattro gli attributi) */
	mode_string = "";
	test_hazel_set(file_name_utf_8, mode_string, set_ret);
	std::cout << "[SET] Argument: \"" << file_name_utf_8 << "\" ModeString: " << mode_string << " Return: " << set_ret << "\n";

	/* Leggi gli attributi per verificare la precedente operazione Set */
	test_hazel_get(file_name_utf_8, get_ret);
	std::cout << "[GET] Argument: \"" << file_name_utf_8 << "\" Attributes: " << get_ret << "\n";

	/* Setta gli attributi originali */
	mode_string = original_attributes;
	test_hazel_set(file_name_utf_8, mode_string, set_ret);
	std::cout << "[SET] Argument: \"" << file_name_utf_8 << "\" ModeString: " << mode_string << " Return: " << set_ret << "\n";

	/* Verifica con un Get */
	test_hazel_get(file_name_utf_8, get_ret);
	std::cout << "[GET] Argument: \"" << file_name_utf_8 << "\" Attributes: " << get_ret << "\n";

	/* Fine */
	return 0;
}
