#include <Windows.h>
#include <Combaseapi.h>
#include <Shellscalingapi.h>
#include <utility>
#include <filesystem>
#include <functional>
#include <cassert>

#include "platform.h"

#include "../core/src/win/util/wstring_utf8.h"

namespace fs = std::filesystem;

namespace {
    std::string GetExecutablePath() {
        wchar_t path[MAX_PATH];
        DWORD result = GetModuleFileNameW(nullptr, path, MAX_PATH);
        assert(result > 0);
        return WStringToUTF8(path);
    }
}



void* DeskGapPlatform::InitUIThread() { 
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    return new DWORD(GetCurrentThreadId());
}

void DeskGapPlatform::InitNodeThread() {
    
}

void DeskGapPlatform::Run() {
    MSG msg;
	BOOL res;
	while ((res = GetMessageW(&msg, nullptr, 0, 0)) != -1) {
		if (msg.hwnd) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else if (msg.message == WM_APP) {
			auto action = reinterpret_cast<std::function<void()>*>(msg.lParam);
			(*action)();
			delete action;
		}
		else if (msg.message == WM_QUIT) {
			return;
		}
	}
}

std::string DeskGapPlatform::PathOfResource(const std::vector<const char*>& paths) {
    static fs::path resourcesFolder = fs::path(GetExecutablePath()).parent_path() / "resources";
    
    fs::path resourcePath = resourcesFolder;
    for (const char* component: paths) {
        resourcePath.append(component);
    }

    return resourcePath.string();
}

bool DeskGapPlatform::ResourceExists(const std::vector<const char*>& paths) {
    return fs::exists(PathOfResource(paths));
}
