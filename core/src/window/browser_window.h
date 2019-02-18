#ifndef browser_window_h
#define browser_window_h

#include <string>
#include <array>
#include <vector>
#include <functional>
#include <optional>
#include "../menu/menu.h"
#include "../webview/webview.h"

namespace DeskGap {

    class BrowserWindow {
    private:
        friend class Dialog;
        struct Impl;
        std::unique_ptr<Impl> impl_;
    public:
        struct EventCallbacks {
            std::function<void()> onBlur;
            std::function<void()> onFocus;
            std::function<void()> onResize;
            std::function<void()> onMove;
            std::function<void()> onClose;
        };
        explicit BrowserWindow(const WebView&, EventCallbacks&&);
        BrowserWindow(const BrowserWindow&) = delete;

        void SetMaximizable(bool);
        void SetMinimizable(bool);
        void SetResizable(bool);
        void SetHasFrame(bool);
        void SetClosable(bool);

        void Minimize();

        void Show();
        void Center();

        void Destroy();
        void Close();

        void SetTitle(const std::string& utf8title);

        void SetSize(int width, int height, bool animate);
        void SetPosition(int x, int y, bool animate);
   
        std::array<int, 2> GetSize();
        std::array<int, 2> GetPosition();

        void SetMaximumSize(int width, int height);
        void SetMinimumSize(int width, int height);

    #ifndef __APPLE__
        void SetMenu(const Menu*);
        void SetIcon(const std::optional<std::string>& iconPath);
    #endif

    #ifdef __APPLE__
        enum class TitleBarStyle: int {
            DEFAULT = 0,
            HIDDEN = 1,
            HIDDEN_INSET = 2
        };
        void SetTitleBarStyle(TitleBarStyle);

        struct Vibrancy {
            std::string material;
            std::string blendingMode;
            std::string state;
            struct Constraint {
                std::string attribute;
                double value;
                enum class Unit { POINT, PERCENTAGE };
                Unit valueUnit;
            };
            std::array<Constraint, 4> constraints;
        };
        void SetVibrancies(const std::vector<Vibrancy>&);
    #endif

        void PopupMenu(const Menu&, const std::array<int, 2>* location, int positioningItem);
        ~BrowserWindow();
    };
}

#endif
