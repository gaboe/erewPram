#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <list>
#include <QDebug>
#include <omp.h>

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
    ui->resultList->clear();
    int elCount = ui->nonScalableSpinBox->value();
    int list[elCount];
    for (int i = 0; i < elCount; i++) {
        list[i] = rand() % 32 + 1;
        ui->nonScalableList->addItem(QString::fromStdString(std::to_string(list[i])));
    }


    int y[elCount];


    omp_set_nested(2);

#pragma omp parallel num_threads(elCount)
    {
#pragma omp for ordered
        for(int i = 0; i < elCount; i++ )
        {
#pragma omp ordered
            {
                qDebug("Zapis hodnoty %d do policka %d na vlakne ID: %d\n", list[i], i, omp_get_thread_num());
                y[i] = list[i];
            }
        }


    }

    for (int i = 0; i < elCount; i++) {
        ui->resultList->addItem(QString::fromStdString(std::to_string(y[i])));
    }



}

