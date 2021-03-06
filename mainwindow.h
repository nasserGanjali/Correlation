#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTime>

#include <stdint.h>

#include "fcorr.h"

class FCorr;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void test();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
