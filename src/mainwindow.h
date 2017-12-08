#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "field.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Field field;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void updateValueLi();
    void updateValueAStar();

};


#endif // MAINWINDOW_H
