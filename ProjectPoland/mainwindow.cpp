#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

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
    //QMessageBox::information(this,"...", fileName[0]);
    foreach(QString str, fileName)
    {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/rec/ImageIcon.png"),str);
        ui->listWidget->addItem(item);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int w = ui->label->width();
    int h = ui->label->height();
    QPixmap pix(ui->listWidget->currentItem()->text());
    ui->label->setPixmap(pix.scaled(w,h, Qt::KeepAspectRatio));
}
