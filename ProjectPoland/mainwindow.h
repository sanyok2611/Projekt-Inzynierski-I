#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qneuralnetwork.h"
#include "qmneuralnetwork.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    bool getImageData(QImage A, int x, int y);

    //void queryCurrentImageFirstRectangle();
    QString queryCurrentImageFirstRaw();
    QString queryCurrentImageSecondRaw();
    QString queryCurrentImageThirdRaw();
    QString queryCurrentImageFourthRaw();
    void imgtocsv();
    float rgb2bw(QImage A, int x, int y);
    QVector <float> getRectValues(QImage A, int x, int y);
    //void on_pushButton_4_clicked();
    void multitrain(QString Path,int times);
    bool iszero(QVector <float> a);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    QImage cvMatToQImage(const cv::Mat &inMat);


    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    qmneuralnetwork *Multinetwork{};
    qmneuralnetwork *Multinetworkch{};

};
#endif // MAINWINDOW_H
