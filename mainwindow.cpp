#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <list>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_nonScalableButton_clicked()
{
    ui->nonScalableList->clear();
    std::list<int> list;
    for (int i = 0; i < ui->nonScalableSpinBox->value(); i++) {
        list.push_back(rand() % 32);
    }
    list.sort();

    foreach (auto item, list) {
        ui->nonScalableList->addItem(QString::fromStdString(std::to_string(item)));
    }
}

