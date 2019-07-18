#include "mangler.h"
#include "log.h"

int string_is_ascii(const char *text) {
  const unsigned char *iter = (const unsigned char*) text;

  for (; *iter != 0; iter++)
    if (*iter > 127)
      return 0;

  return 1;
}

void mangle(std::string& text) {
	if (!string_is_ascii(text.c_str())) {
		Log::print(L"Stringa fuori dal set ASCII");
	}

	size_t src_len = strlen(text.c_str());
	char *dst = (char*) malloc(src_len + 1);
	const unsigned char *src_iter = (const unsigned char*) text.c_str();
	unsigned char *dst_iter = (unsigned char*) dst;

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