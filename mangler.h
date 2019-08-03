#pragma once

#include <string>

void mangle_date(std::string& text);
void mangle_serial_number(std::string& text);
bool detect_and_decode_hex(std::string& text);

bool check_zero_date(const std::string& text);
bool check_zero_serial_number(const std::string& text);
