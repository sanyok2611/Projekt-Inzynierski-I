#include "matrix.h"
#include <QDebug>
#include <cmath>

float randomFl()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX)-0.5;
}
matrix::matrix(int raws, int cols)
{
    QVector <float> tempVector;
    for(int i = 0; i<raws; i++)
    {
        for(int j=0; j<cols; j++)
        {
            tempVector.push_back(randomFl());
        }
        this->Matrix.push_back(tempVector);
        tempVector.clear();
    }
}

matrix::matrix(QVector <float> &input)
{
    this->Matrix.push_back(input);
}

void matrix::fill(int raws, int cols)
{
    QVector <float> tempVector;
    for(int i = 0; i<raws; i++)
    {
        for(int j=0; j<cols; j++)
        {
            tempVector.push_back(randomFl());
        }
        this->Matrix.push_back(tempVector);
        tempVector.clear();
    }
}
matrix::matrix()
{
}

void matrix::show()
{
    for(QVector <float> i: this->Matrix)
    qDebug()<<i;

}

matrix matrix::dot(matrix A)
{
    matrix C(this->Matrix.size(), A.Matrix[0].size());
    for(int i=0; i<this->Matrix.size(); i++)
        for(int j=0; j<A.Matrix[0].size(); j++)
        {
            C.Matrix[i][j]=0;
            for(int k=0; k<this->Matrix[0].size();k++)
                C.Matrix[i][j]+=this->Matrix[i][k]*A.Matrix[k][j];
        }
    return C;
}

matrix matrix::transpose()
{
    matrix C(this->Matrix[0].size(), this->Matrix.size());
    for(int i=0; i<this->Matrix[0].size(); i++)
        for(int j=0; j<this->Matrix.size(); j++)
            C.Matrix[i][j]=this->Matrix[j][i];
    return C;
}

matrix matrix::mltpl(matrix A)
{
    matrix C(A.Matrix.size(), A.Matrix[0].size());
    for(int i=0; i<A.Matrix.size(); i++)
        for(int j=0; j<A.Matrix[0].size(); j++)
            C.Matrix[i][j]=this->Matrix[i][j]*A.Matrix[i][j];
    return C;
}

matrix matrix::minus(matrix A)
{
    matrix C(A.Matrix.size(), A.Matrix[0].size());
    for(int i=0; i<A.Matrix.size(); i++)
        for(int j=0; j<A.Matrix[0].size(); j++)
            C.Matrix[i][j]=this->Matrix[i][j]-A.Matrix[i][j];
    return C;
}

matrix matrix::numbmltpl(float number)
{
    matrix C(this->Matrix.size(), this->Matrix[0].size());
    for(int i=0; i<this->Matrix.size(); i++)
        for(int j=0; j<this->Matrix[0].size(); j++)
            C.Matrix[i][j]=this->Matrix[i][j]*number;
    return C;
}

matrix matrix::add(matrix A)
{
    matrix C(A.Matrix.size(), A.Matrix[0].size());
    for(int i=0; i<A.Matrix.size(); i++)
        for(int j=0; j<A.Matrix[0].size(); j++)
            C.Matrix[i][j]=this->Matrix[i][j]+A.Matrix[i][j];
    return C;
}

matrix matrix::oneminus()
{
    matrix C(this->Matrix.size(), this->Matrix[0].size());
    for(int i=0; i<this->Matrix.size(); i++)
        for(int j=0; j<this->Matrix[0].size(); j++)
            C.Matrix[i][j]=1-this->Matrix[i][j];
    return C;
}

matrix matrix::numbadd(float number)
{
    matrix C(this->Matrix.size(), this->Matrix[0].size());
    for(int i=0; i<this->Matrix.size(); i++)
        for(int j=0; j<this->Matrix[0].size(); j++)
            C.Matrix[i][j]=this->Matrix[i][j]+number;
    return C;
}

int matrix::getmax()
{
    float maxi = this->Matrix[0][0];
    int index=0;
    for(int i=1; i<this->Matrix.size(); i++)
        if(this->Matrix[i][0]>maxi)
        {
            index=i;
            maxi=this->Matrix[i][0];
        }

    return index;
}
