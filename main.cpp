#include "window_copier.h"
#include <QApplication>
#include "Windows.h"
#include <QDebug>

// needed here for window callback
WindowCopier* w;

void CALLBACK ShellHookProc(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
                            LONG idobject, LONG idchild, DWORD dwEventThread,
                            DWORD dwEventTime) {
    OutputDebugString(L"Window: ");
    if (hwnd == nullptr) {
        return; // No focused window
    }

    if (w == nullptr) {
        return;
    }

    switch (w->getState()) {
    case WindowCopier::State::LOOKING_FOR_BASE_WINDOW:
        w->setBaseWindow(hwnd);
        break;
    case WindowCopier::State::LOOKING_FOR_TARGET_WINDOW:
        w->setTargetWindow(hwnd);
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    w = new WindowCopier;
    // TODO keep w in foreground
    w->show();

    auto shellHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, ShellHookProc,
        0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    const auto statusResult = a.exec();

    UnhookWinEvent(shellHook);
    return statusResult;
}
