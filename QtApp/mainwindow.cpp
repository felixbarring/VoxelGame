#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_mainWindow(new Ui::MainWindow) {
  m_mainWindow->setupUi(this);
}

MainWindow::~MainWindow() {
  delete m_mainWindow;
}

void MainWindow::on_actionExit_triggered() {
  this->close();
}
