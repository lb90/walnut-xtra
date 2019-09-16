#pragma once

#include <string>

void xtra_initialize();
void xtra_hazel_get(const std::string& file_name_utf_8,
                    std::string& ret);
void xtra_hazel_set(const std::string& file_name_utf_8,
                    const std::string& mode_string,
                    std::string& ret);
void xtra_finalize();
