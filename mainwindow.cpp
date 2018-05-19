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

    ui->nonScalableResult->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_nonScalableButton_clicked()
{
    ui->nonScalableList->clear();
    ui->nonScalableResultList->clear();
    int elCount = ui->nonScalableSpinBox->value();
    int list[elCount];
    for (int i = 0; i < elCount; i++) {
        list[i] = rand() % 32 + 1;
        ui->nonScalableList->addItem(QString::fromStdString(std::to_string(list[i])));
    }


    int y[elCount];

#pragma omp parallel num_threads(elCount)
    {

#pragma omp for ordered
        for(int i = 0; i < elCount; i++ )
        {
#pragma omp ordered
            {
                y[i] = list[i];
            }
        }

        for(int j = 0; j < std::log(elCount); j++){
#pragma omp for ordered
            for(int i = std::pow(2,j); i < elCount; i++ )
            {
#pragma omp ordered
                {
                    y[i] = y[i] + list[i - (int)std::pow(2,j)];
                }
            }

#pragma omp for ordered
            for(int i = std::pow(2,j); i < elCount; i++ )
            {
#pragma omp ordered
                {
                    list[i] = y[i];
                }
            }
        }

    }

    for (int i = 0; i < elCount; i++) {
        ui->nonScalableResultList->addItem(QString::fromStdString(std::to_string(y[i])));
    }
    auto result = QString("Result: ");
    result.append(QString::fromStdString(std::to_string(y[elCount - 1])));

    ui->nonScalableResult->show();
    ui->nonScalableResult->setText(result);

}


void MainWindow::on_scalableButton_clicked()
{
    ui->scalableList->clear();
    ui->scalableResultList->clear();
    int elCount = ui->scalableSpinBox->value();
    int tCount = ui->scalableSpinBox_2->value();

    int list[elCount];
    for (int i = 0; i < elCount; i++) {
        list[i] = rand() % 10 + 1;
        ui->scalableList->addItem(QString::fromStdString(std::to_string(list[i])));
    }


    int S[elCount];
    int Z[tCount];
#pragma omp parallel num_threads(tCount)
    {

#pragma omp for ordered
        for(int i = 0; i < tCount; i++ )
        {
#pragma omp ordered
            {
                int from = std::floor(elCount / tCount) * (i);
                int to = i + 1 == tCount
                        ? elCount
                        : std::floor(elCount / tCount) * (i + 1);
                qDebug("Running on p %d, taking from %d to %d" , i, from, to);
                Z[i] = list[from];
                for(int j = from + 1; j < to; j++){
                    Z[i] += list[j];
                }
                qDebug("Local sum is %d" , Z[i]);
            }

        }
    }
}
