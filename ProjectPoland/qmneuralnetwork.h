#ifndef QMNEURALNETWORK_H
#define QMNEURALNETWORK_H

#include <QObject>
#include "matrix.h"
#include <QFile>
class qmneuralnetwork : public QObject
{
    Q_OBJECT
public:
    explicit qmneuralnetwork(QObject *parent = nullptr);
    qmneuralnetwork();
    qmneuralnetwork(int inputnodes, int hiddennodes, int outputnodes, int ooutputnodes, float learningrate, char tp);
    qmneuralnetwork(const QString &filePath);
    void train(QVector <float> &inputs_list, QVector <float> &targets_list);
    matrix query(QVector <float> &inputs_list);
    matrix selfActivation(matrix &x);
    void saveWeighths();
    int inodes;
    int hnodes;
    int onodes;
    int oonodes;
    char type;
    matrix mwih;
    matrix mwho;
    matrix mwoo;
    float lr;

signals:
    void whatnumbersignal(int what);
public slots:
};

#endif // QMNEURALNETWORK_H
