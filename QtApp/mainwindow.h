#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
    explicit MainWindow(QWidget* parent);

    virtual ~MainWindow();

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow* m_mainWindowmainWindow;
};

#endif // MAINWINDOW_H
