#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this,"Choose files to read", "C://");
    QMessageBox::information(this,"...", fileName[0]);
    foreach(QString str, fileName)
    {
        ui->listWidget->addItem(str);
    }
}
