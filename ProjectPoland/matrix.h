#ifndef MATRIX_H
#define MATRIX_H
#include <QVector>

class matrix
{
public:
    matrix(int raws, int cols);
    matrix();
    matrix(QVector <float> &inputs);
    void fill(int raws, int cols);
    QVector <QVector <float>> Matrix;
    void show(); //відобразити матрицю в дебаг
    matrix dot(matrix A); //матричне множення
    matrix transpose();  //транспонування матриці
    matrix mltpl(matrix A); //поелементне множення
    matrix minus(matrix A);
    matrix numbmltpl(float number);
    matrix add(matrix A);
    matrix oneminus();
    matrix numbadd(float number);
    int getmax();

};

#endif // MATRIX_H
