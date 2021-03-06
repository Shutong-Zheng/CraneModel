#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    craneModel = new CraneModel;    //自定义的需要显示的图形
    ui->scrollArea->setWidget(craneModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

