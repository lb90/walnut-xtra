#include "mangler.h"
#include "log.h"

#include <vector>
#include <algorithm>

typedef unsigned char uchar_t;

void mangle_date(std::string& text) {
	if (text.length() < 8) {
		text = "1000001";
		return;
	}

	for (int i = 0; i < 8; i++) {
		if (!isdigit(text[i])) {
			text = "1000001";
			return;
		}
	}

	std::string year = text.substr(2, 2);
	std::string month = text.substr(4, 2);
	std::string day = text.substr(6, 2);
	text = "1" + day + month + year;

	std::replace(text.begin(), text.end(), '0', '3');
}


uchar_t hex_util_char_to_val(uchar_t c) {
	if (c >= 48 && c <= 57) /* 0-9 */
		c = (c - 48);
	else if (c >= 65 && c <= 70) /* A-F */
		c = (c - 65) + 10;
	else if (c >= 97 && c <= 102) /* a-f */
		c = (c - 97) + 10;
	else {
#ifdef _DEBUG
		throw std::runtime_error("non hex character passed to decoder");
#else
		return 48; /* carattere 0 */
#endif
	}
	return c;
}
uchar_t decode_two_chars_hex(const uchar_t* str) {
	unsigned val = 16 * hex_util_char_to_val(str[0]) +
	                    hex_util_char_to_val(str[1]);
#ifdef _DEBUG
	if (val > 255)
		throw std::runtime_error("invalid value");
#endif
	return uchar_t(val);
}
bool detect_and_decode_hex(std::string& text) {
	const uchar_t *c_iter = NULL;

	if (text.length() % 2 != 0)
		return false;

	for (c_iter = (uchar_t*) text.c_str(); *c_iter; ++c_iter) {
		uchar_t c = *c_iter;
		if (!((c >= 48 && c <= 57) /* 0-9 */
			|| (c >= 65 && c <= 70) /* A-F */
			|| (c >= 97 && c <= 102))) /* a-f */
			return false;
	}

	std::vector<uchar_t> new_string_buffer;
	new_string_buffer.reserve(text.length() / 2);

	for (c_iter = (uchar_t*)text.c_str(); *c_iter; c_iter+=2) {
		uchar_t c = decode_two_chars_hex(c_iter);
		if (!(c >= 20 && c <= 126))
			return false;
		if (std::isalnum(c))
			new_string_buffer.push_back(decode_two_chars_hex(c_iter));
	}

	new_string_buffer.push_back(0); /* terminating character */
	text = (char*) new_string_buffer.data();

	return true;
}

int string_is_ascii(const char *text) {
  const uchar_t *iter = (const uchar_t*) text;

  for (; *iter != 0; iter++)
    if (*iter > 127)
      return 0;

  return 1;
}
void mangle_serial_number(std::string& text) {
	if (!string_is_ascii(text.c_str())) {
		Log::print(L"Stringa fuori dal set ASCII");
	}

	size_t src_len = strlen(text.c_str());
	char *dst = (char*) malloc(src_len + 1);
	const uchar_t *src_iter = (const uchar_t*) text.c_str();
	uchar_t *dst_iter = (uchar_t*) dst;

	if (!dst) {
		Log::print(L"Impossibile allocare memoria");
		text = "1000001";
		return;
	}

	memset(dst, 0, src_len + 1);

	for (; *src_iter != 0; src_iter++) {
      unsigned char val = *src_iter;
      if ( (val >= 48 && val <= 57) ) /* 0-9 */
        *dst_iter++ = val;
      else if ( (val >= 65 && val <= 73) ) /* A-I */
        *dst_iter++ = val - 64 + 48;
      else if ( (val >= 74 && val <= 82) ) /* J-R */
        *dst_iter++ = val - 64 - 9 + 48;
      else if ( (val >= 83 && val <= 90) ) /* S-Z */
        *dst_iter++ = val - 64 - 9 - 9 + 48;
      else if ( (val >= 97 && val <= 105) ) /* a-i */
        *dst_iter++ = val - 96 + 48;
      else if ( (val >= 106 && val <= 114) ) /* j-r */
        *dst_iter++ = val - 96 - 9 + 48;
      else if ( (val >= 115 && val <= 122) ) /* s-z */
        *dst_iter++ = val - 96 - 9 - 9 + 48;
	}
	*dst_iter = 0;

	text = dst;
}

bool check_zero_date(const std::string& text) {
	const uchar_t* iter = (const uchar_t*) text.c_str();

	for (; *iter; ++iter) {
		uchar_t c = *iter;

		if (c != 48)
			return false;
	}

	return true;
}

bool check_zero_serial_number(const std::string& text) {
	const uchar_t* iter = (const uchar_t*) text.c_str();

	for (; *iter; ++iter) {
		uchar_t c = *iter;

		if (c != 48 && c != 32)
			return false;
	}

	return true;
}
