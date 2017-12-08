#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(400, 100, field.width() + 200, field.height() + 50);
    connect( ui->buttonLiSearch, SIGNAL(clicked(bool)), ui->widget, SLOT(pathFindingLi()));
    connect( ui->buttonAStarSearch, SIGNAL(clicked(bool)), ui->widget, SLOT(pathFindingAStar()));
    connect(ui->buttonClearPath, SIGNAL(clicked(bool)), ui->widget, SLOT(clearPath()));
    connect(ui->buttonClearWalls, SIGNAL(clicked(bool)), ui->widget, SLOT(clearWalls()));
    connect(ui->buttonClearAll, SIGNAL(clicked(bool)), ui->widget, SLOT(clearAll()));
    connect(ui->buttonLiSearch, SIGNAL(clicked(bool)), SLOT(updateValueLi()));
    connect(ui->buttonAStarSearch, SIGNAL(clicked(bool)), SLOT(updateValueAStar()));
//    connect( ui->widget, SIGNAL(signalLi()), SLOT(updateValueLi()));
//    connect( ui->widget, SIGNAL(signalAStar()), SLOT(updateValueAStar()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateValueLi()
{
    ui->labelLi->setText(QString::number(field.getLentghLi()));
}

void MainWindow::updateValueAStar()
{
    ui->labelAStar->setText(QString::number(field.getLentghAStar()));
}
