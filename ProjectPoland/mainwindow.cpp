#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include "shapedetector.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Multinetwork = new qmneuralnetwork("C:/Qt/MNNP200_0.3.txt");
    Multinetworkch = new qmneuralnetwork("C:/Qt/MNNP350_0.3a.txt");
    //Multinetworkch = new qmneuralnetwork(784,350, 104, 26, 0.3,'a');
    //multitrain("C:/users/Саша/Desktop/Мусорнапроект/characters.csv",5);
    //Multinetworkch->saveWeighths();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QStringList fileName = QFileDialog::getOpenFileNames(this,"Choose files to read", "C://Qt/projecttrash","*.png");
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
    const string imgpath = ui->listWidget->currentItem()->text().toUtf8().constData();
    QPixmap pix(ui->listWidget->currentItem()->text());
    QImage image(pix.toImage());
    cv::Mat mat = cv::imread(imgpath);
    cv::medianBlur(mat,mat,5);
    cv::fastNlMeansDenoising(mat,mat,30,7,21);
    cv::Mat frame;
    cv::GaussianBlur(mat,frame,cv::Size(0,0),3);
    //cv::addWeighted(mat,3,frame,-0.5,0,frame);
    cv::addWeighted(mat,10,frame,-0.05,0,frame);
    cv::Mat cimage = mat;
    cv::Mat gray;
    if(cimage.channels()==3)
        cv::cvtColor(cimage,gray,cv::COLOR_BGR2GRAY);
    else
        gray = cimage.clone();
    cv::Mat blurred, thresh;
    cv::GaussianBlur(gray, blurred, Size(5, 5), 0.0);
    cv::threshold(blurred, thresh, 60, 255, cv::THRESH_BINARY);
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    ShapeDetector sd;
    vector<Point> c;
    for (size_t i = 0; i < contours.size(); i++)
    {
        c = contours[i];
        Rect crect = boundingRect(c);
        sd.detect(Mat(c));
        string shape = sd.get_shape_type();
        double area = cv::contourArea(c);
        if((shape=="square" || shape=="rectangle") && area<13600 && area>12700)
        {
        cv::Mat part = gray(Rect(crect.x+5,crect.y+5,crect.width-10,crect.height-10));
        cv::resize(part,part,cv::Size(112,112));
        //imshow("part",part);
        cv::imwrite("C:/Qt/part.png",part);
        QImage parted = cvMatToQImage(part);

        QVector <float> pixels;
        for(int i=0; i<112; i+=4)
        {
            for(int j=0; j<112; j+=4)
            {
                if(getImageData(parted,j,i)) pixels.push_back(1);
                else pixels.push_back(0);
            }
        }
        matrix ans = Multinetworkch->query(pixels);
        int nMyInt = ans.getmax()+97;
        char nMyChar = static_cast<char>(nMyInt);
        QChar a = char(nMyChar);
        qDebug()<<a<<endl;
        cv::drawContours(cimage, contours, i, Scalar(0, 255, 0), 6);
        }
    }
    imshow("Image", cimage);
    //waitKey(0);
    //cv::imwrite("C:/Qt/name.png",mat);
    pix = cvMatToQPixmap(mat);
    ui->label->setPixmap(pix.scaled(w,h, Qt::KeepAspectRatio));
    ui->plainTextEdit->document()->setPlainText(queryCurrentImageFirstRaw()+','+queryCurrentImageSecondRaw()+','+queryCurrentImageThirdRaw()+','+queryCurrentImageFourthRaw()+';');
}

bool MainWindow::getImageData(QImage A, int x, int y)
{
    bool pixel = false;
    for(int i=x; i<x+4; i++)
        for(int j=y; j<y+4; j++)
        {
            if(A.pixelColor(QPoint(i,j)).red()<140) pixel = true;
        }
    return pixel;
}

QString MainWindow::queryCurrentImageFirstRaw()
{
    QString word = "";
    QPixmap pmap(ui->listWidget->currentItem()->text());
    QImage img(pmap.toImage());
    QVector <float> pixels;
    int start = 840;
    int end = 2240;
    for(start; start<end; start+=140)
    {
        for(int i=224; i<336; i+=4)
        {
            int localend = start+112;
            for(int j=start; j<localend; j+=4)
            {
                if(getImageData(img,j,i)) pixels.push_back(1);
                else pixels.push_back(0);
            }
        }
        if(iszero(pixels)==false)
        {
        matrix ans = Multinetworkch->query(pixels);
        int nMyInt = ans.getmax()+97;
        char nMyChar = static_cast<char>(nMyInt);
        QChar a = char(nMyChar);
        word += a;
        qDebug()<<pixels;
        pixels.clear();
        }
    }
    return word;
}

QString MainWindow::queryCurrentImageSecondRaw()
{
    QString word = "";
    QPixmap pmap(ui->listWidget->currentItem()->text());
    QImage img(pmap.toImage());
    QVector <float> pixels;
    int start = 840;
    int end = 2240;
    for(start; start<end; start+=140)
    {
        for(int i=392; i<504; i+=4)
        {
            int localend = start+112;
            for(int j=start; j<localend; j+=4)
            {
                if(getImageData(img,j,i)) pixels.push_back(1);
                else pixels.push_back(0);
            }
        }
        if(iszero(pixels)==false)
        {
        matrix ans = Multinetworkch->query(pixels);
        int nMyInt = ans.getmax()+97;
        char nMyChar = static_cast<char>(nMyInt);
        QChar a = char(nMyChar);
        word += a;
        pixels.clear();
        }
    }
    return word;
}

QString MainWindow::queryCurrentImageThirdRaw()
{
    QString word = "";
    QPixmap pmap(ui->listWidget->currentItem()->text());
    QImage img(pmap.toImage());
    QVector <float> pixels;
    int start = 840;
    int end = 2240;
    for(start; start<end; start+=140)
    {
        for(int i=560; i<672; i+=4)
        {
            int localend = start+112;
            for(int j=start; j<localend; j+=4)
            {
                if(getImageData(img,j,i)) pixels.push_back(1);
                else pixels.push_back(0);
            }
        }
        if(iszero(pixels)==false)
        {
        matrix ans = Multinetwork->query(pixels);
        word += QString::number(ans.getmax());
        pixels.clear();
        }
    }
    return word;
}

QString MainWindow::queryCurrentImageFourthRaw()
{
    QString word = "";
    QPixmap pmap(ui->listWidget->currentItem()->text());
    QImage img(pmap.toImage());
    QVector <float> pixels;
    int start = 840;
    int end = 2240;
    for(start; start<end; start+=140)
    {
        for(int i=728; i<840; i+=4)
        {
            int localend = start+112;
            for(int j=start; j<localend; j+=4)
            {
                if(getImageData(img,j,i)) pixels.push_back(1);
                else pixels.push_back(0);
            }
        }
        if(iszero(pixels)==false)
        {
        matrix ans = Multinetwork->query(pixels);
        word += QString::number(ans.getmax());
        pixels.clear();
        }
    }
    return word;
}

bool MainWindow::iszero(QVector <float> a)
{
    for(int i=0; i<a.length(); i++)
    {
        if(a[i]==1) return false;
    }
    return true;
}

void MainWindow::imgtocsv()
{
    QFile File("characters.csv");
    if (File.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&File);
        QPixmap pmap(ui->listWidget->currentItem()->text());
        QImage img(pmap.toImage());
        QVector <float> pixels;
        for(int i = 0; i<728; i+=28)
        {
            for(int j = 0; j<504; j+=28)
            {
                pixels = getRectValues(img, i, j);
                pixels.push_front(i/28);
                QString tmp = "";
                for(int k = 0; k<pixels.length(); k++)
                {
                    tmp+=QString::number(pixels[k]);
                    tmp+=", ";
                }
                stream <<tmp<<Qt::endl;
                tmp.clear();
            }
        }
        File.close();
    }
}

QVector <float> MainWindow::getRectValues(QImage A, int x, int y)
{
    int raw = x+28;
    int col = y+28;
    QVector <float> pixvals;
    for(int i = x; i<raw; i++)
    {
        for(int j = y; j<col; j++)
        {
            pixvals.push_back(rgb2bw(A,j,i));
        }
    }
    return pixvals;
}

float MainWindow::rgb2bw(QImage A, int x, int y)
{
    return ((A.pixelColor(QPoint(x,y)).red() + A.pixelColor(QPoint(x,y)).green() + A.pixelColor(QPoint(x,y)).blue())/3);
}


/*void MainWindow::on_pushButton_4_clicked()
{
    imgtocsv();
}*/

void MainWindow::multitrain(QString Path,int times)
{
    QFile File(Path);
    int cnt = 0;
    QStringList PixList;
    for(int i=0; i<times; i++)
    {
    qDebug()<<cnt++;
    if(!File.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qCritical("Error %d : %s", File.error(),qPrintable(File.errorString()));
    }
    QString Number;
    while(!File.atEnd())
    {
        Number = File.readLine();
        PixList = Number.split(',',QString::SkipEmptyParts);
        QVector <float> targets(26,0);
        for(int i=0; i<targets.size(); i++)
            targets[i]=0.01;
        targets[PixList[0].toInt()]=0.99;
        PixList.pop_front();
        QVector <float> inputs;
        for(int i=0; i<PixList.length();i++)
            inputs.push_back(PixList[i].toFloat()/255*0.99+0.01);
        Multinetworkch->train(inputs,targets);
    }
    File.close();
    }
}


QImage MainWindow::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
        case CV_8UC4:
        {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_ARGB32);
            return image;
        }
        case CV_8UC3:
        {
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        case CV_8UC1:
        {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Grayscale8);
    #else
            static QVector<QRgb>  sColorTable;
            if (sColorTable.isEmpty())
            {
                sColorTable.resize(256 );

                for (int i = 0; i < 256; ++i )
                {
                    sColorTable[i] = qRgb(i, i, i );
                }
            }

            QImage image(inMat.data,
                         inMat.cols, inMat.rows,
                         static_cast<int>(inMat.step),
                         QImage::Format_Indexed8 );

            image.setColorTable(sColorTable );
    #endif
            return image;
        }
        default:
        {
            qWarning()<< "ASM::cvMatToQImage()- cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }
    }
    return QImage();
}

QPixmap MainWindow::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        qDebug()<<item->text()<<endl;
        string imgpath = item->text().toUtf8().constData();
        cv::Mat mat = cv::imread(imgpath);
        cv::medianBlur(mat,mat,5);
        cv::fastNlMeansDenoising(mat,mat,30,7,21);
        cv::Mat frame;
        cv::GaussianBlur(mat,frame,cv::Size(0,0),3);
        cv::addWeighted(mat,3,frame,-0.5,0,frame);
        cv::Mat cimage = mat;
        cv::Mat gray;
        if(cimage.channels()==3)
            cv::cvtColor(cimage,gray,cv::COLOR_BGR2GRAY);
        else
            gray = cimage.clone();
        cv::Mat blurred, thresh;
        cv::GaussianBlur(gray, blurred, Size(5, 5), 0.0);
        cv::threshold(blurred, thresh, 60, 255, cv::THRESH_BINARY);
        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;
        cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        ShapeDetector sd;
        vector<Point> c;
        QString word = "";
        for (size_t i = contours.size()-1; i > 0; i--)
        {
            c = contours[i];
            Rect crect = boundingRect(c);
            sd.detect(Mat(c));
            string shape = sd.get_shape_type();
            double area = cv::contourArea(c);
            if((shape=="square" || shape=="rectangle") && area<13600 && area>12700)
            {
                cv::Mat part = gray(Rect(crect.x+5,crect.y+5,crect.width-10,crect.height-10));
                cv::resize(part,part,cv::Size(112,112));
                QImage parted = cvMatToQImage(part);
                imshow("asd",part);
                QVector <float> pixels;
                qDebug()<<parted.size()<<"Size"<<endl;
                for(int i=0; i<112; i+=4)
                {
                    for(int j=0; j<112; j+=4)
                    {
                        if(getImageData(parted,j,i)) pixels.push_back(1);
                        else pixels.push_back(0);
                    }
                }
                if(!iszero(pixels))
                {
                    matrix ans = Multinetworkch->query(pixels);
                    int nMyInt = ans.getmax()+97;
                    char nMyChar = static_cast<char>(nMyInt);
                    QChar a = char(nMyChar);
                    word+=a;
                }
            }
        }
        qDebug()<<word<<endl;
    }
}
