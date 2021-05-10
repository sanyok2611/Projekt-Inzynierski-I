#ifndef QNEURALNETWORK_H
#define QNEURALNETWORK_H

#include <QObject>
#include "matrix.h"
#include <QFile>
class qneuralnetwork : public QObject
{
    Q_OBJECT
public:
    explicit qneuralnetwork(QObject *parent = nullptr);
    qneuralnetwork();
    qneuralnetwork(int inputnodes, int hiddennodes, int outputnodes, float learningrate);
    qneuralnetwork(const QString &filePath);
    void train(QVector <float> &inputs_list, QVector <float> &targets_list);
    matrix query(QVector <float> &inputs_list);
    matrix selfActivation(matrix &x);
    void saveWeighths();
    int inodes;
    int hnodes;
    int onodes;
    matrix mwih;
    matrix mwho;
    float lr;

signals:
    void whatnumbersignal(int what);
public slots:
};

#endif // QNEURALNETWORK_H
