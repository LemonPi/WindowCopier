#ifndef WINDOW_COPIER_H
#define WINDOW_COPIER_H

#include <QMainWindow>
#include "Windows.h"

namespace Ui {
class WindowCopier;
}

#include "Windows.h"

class WindowCopier : public QMainWindow {
    Q_OBJECT

  public:
    enum class State {
        INACTIVE,
        LOOKING_FOR_BASE_WINDOW,
        LOOKING_FOR_TARGET_WINDOW,
        NUM_STATES
    };

    explicit WindowCopier(QWidget* parent = 0);
    ~WindowCopier();

    State getState() const;
    void changeState(State state);
    bool setBaseWindow(HWND baseWindow);
    bool setTargetWindow(HWND targetWindow);
    bool resizeBaseToTarget();

  private slots:
    void on_startButton_clicked();

  private:
    Ui::WindowCopier* ui;
    State _appState;

    HWND _baseWindow;
    HWND _targetWindow;
};

#endif // WINDOW_COPIER_H
