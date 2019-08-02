#pragma once

#include <string>

class Clipboard final {
public:
	static void CopyToClipboard(const std::string& text);
private:
};