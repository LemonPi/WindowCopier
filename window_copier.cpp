#include "window_copier.h"
#include "ui_window_copier.h"
#include <QDebug>
#include <QTimer>

WindowCopier::WindowCopier(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::WindowCopier) {
    setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    changeState(State::INACTIVE);
}

WindowCopier::~WindowCopier() {
    delete ui;
}

WindowCopier::State WindowCopier::getState() const {
    return _appState;
}

void WindowCopier::changeState(State state) {
    _appState = state;
    QString instructionText = "";
    switch (_appState) {
    case State::INACTIVE: {
        instructionText = "Click to start";
        _baseWindow = nullptr;
        _targetWindow = nullptr;
        break;
    }
    case State::LOOKING_FOR_BASE_WINDOW:
        instructionText = "Select a window to resize";
        break;
    case State::LOOKING_FOR_TARGET_WINDOW:
        instructionText = "Select a window to resize to";
        break;
    default: {}
    }

    ui->instructionLabel->setText(instructionText);
}

void WindowCopier::on_startButton_clicked() {
    changeState(State::LOOKING_FOR_BASE_WINDOW);
}

bool WindowCopier::setBaseWindow(HWND baseWindow) {
    if (_appState != State::LOOKING_FOR_BASE_WINDOW) {
        return false;
    }
    _baseWindow = baseWindow;
    changeState(State::LOOKING_FOR_TARGET_WINDOW);
    return true;
}

bool WindowCopier::setTargetWindow(HWND targetWindow) {
    if (_appState != State::LOOKING_FOR_TARGET_WINDOW) {
        return false;
    }
    _targetWindow = targetWindow;
    // TODO move this out into separate action
    if (resizeBaseToTarget() == false) {
        return false;
    }

    changeState(State::INACTIVE);
    return true;
}

bool WindowCopier::resizeBaseToTarget() {
    if (_baseWindow == nullptr || _targetWindow == nullptr) {
        qDebug() << "Resizing without base or target";
        return false;
    }

    RECT rect;
    if (GetWindowRect(_targetWindow, &rect) == 0) {
        qDebug() << "Couldn't get target window rect";
        return false;
    }

    const auto width = rect.right - rect.left;
    const auto height = rect.bottom - rect.top;

    if (SetWindowPos(_baseWindow, nullptr, rect.left, rect.top, width, height,
                     SWP_SHOWWINDOW) == false) {
        qDebug() << "Couldn't move window to " << rect.left << " " << rect.top;
        return false;
    }

    auto baseWindow = _baseWindow;
    constexpr auto SHOW_WINDOW_DELAY_MS = 10;
    // show the base window afterwards
    QTimer::singleShot(SHOW_WINDOW_DELAY_MS, this,
                       [baseWindow]() { SetActiveWindow(baseWindow); });

    return true;
}
