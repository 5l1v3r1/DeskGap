#include "../shell/shell.h"
#include "./util/wstring_utf8.h"

bool DeskGap::Shell::OpenExternal(const std::string& urlString) {
	std::wstring wUrlString = UTF8ToWString(urlString.c_str());
    return ShellExecuteW(
    	nullptr, L"open",
    	wUrlString.c_str(),
    	nullptr, nullptr,
    	SW_SHOWNORMAL
    ) > (HINSTANCE)32;
}
