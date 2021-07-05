#include "qmneuralnetwork.h"
#include <QDebug>
#include <cmath>

qmneuralnetwork::qmneuralnetwork(QObject *parent) : QObject(parent)
{

}

qmneuralnetwork::qmneuralnetwork(int inputnodes, int hiddennodes, int outputnodes, int ooutputnodes, float learningrate, char tp)
{
    this->type=tp;
    this->hnodes=hiddennodes;
    this->onodes=outputnodes;
    this->inodes=inputnodes;
    this->oonodes=ooutputnodes;
    this->lr=learningrate;
    this->mwih.fill(hiddennodes,inputnodes);
    this->mwho.fill(outputnodes,hiddennodes);
    this->mwoo.fill(ooutputnodes,outputnodes);
}

qmneuralnetwork::qmneuralnetwork(const QString &filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    int inputnodes = file.readLine().toInt();
    this->inodes=inputnodes;
    int hiddennodes = file.readLine().toInt();
    this->hnodes=hiddennodes;
    int outputnodes = file.readLine().toInt();
    this->onodes=outputnodes;
    int ooutputnodes = file.readLine().toInt();
    this->oonodes=ooutputnodes;
    float learningrate = file.readLine().toFloat();
    this->lr=learningrate;
    QString Weighths = file.readLine();
    QStringList Line = Weighths.split(',',QString::SkipEmptyParts);
    for(int i=0; i<hiddennodes; i++)
    {
        QVector <float> tmp;
        for(int j=0; j<inputnodes; j++)
        {
            tmp.push_back(Line[j+i*inputnodes].toFloat());
        }
        this->mwih.Matrix.push_back(tmp);
        tmp.clear();
    }
    Weighths = file.readLine();
    Line = Weighths.split(',',QString::SkipEmptyParts);
    for(int i=0; i<outputnodes; i++)
    {
        QVector <float> tmp;
        for(int j=0; j<hiddennodes; j++)
        {
            tmp.push_back(Line[j+i*hiddennodes].toFloat());
        }
        this->mwho.Matrix.push_back(tmp);
        tmp.clear();
    }
    Weighths = file.readLine();
    Line = Weighths.split(',',QString::SkipEmptyParts);
    for(int i=0; i<ooutputnodes; i++)
    {
        QVector <float> tmp;
        for(int j=0; j<outputnodes; j++)
        {
            tmp.push_back(Line[j+i*outputnodes].toFloat());
        }
        this->mwoo.Matrix.push_back(tmp);
        tmp.clear();
    }
    file.close();
}

void qmneuralnetwork::train(QVector <float> &inputs_list, QVector <float> &targets_list)
{
    matrix inputs(inputs_list);
    inputs=inputs.transpose();
    matrix targets(targets_list);
    targets=targets.transpose();
    matrix hidden_outputs = this->mwih.dot(inputs);
    hidden_outputs=this->selfActivation(hidden_outputs);
    matrix final_outputs=this->mwho.dot(hidden_outputs);
    final_outputs=this->selfActivation(final_outputs);
    matrix o_outputs=this->mwoo.dot(final_outputs);
    o_outputs=this->selfActivation(o_outputs);

    matrix output_errors=targets.minus(o_outputs);
    matrix final_errors=this->mwoo.transpose().dot(output_errors);
    matrix hidden_errors=this->mwho.transpose().dot(final_errors);

    this->mwoo=this->mwoo.add(output_errors.mltpl(o_outputs).mltpl(o_outputs.oneminus()).dot(final_outputs.transpose()).numbmltpl(this->lr));
    this->mwho=this->mwho.add(final_errors.mltpl(final_outputs).mltpl(final_outputs.oneminus()).dot(hidden_outputs.transpose()).numbmltpl(this->lr));
    this->mwih=this->mwih.add(hidden_errors.mltpl(hidden_outputs).mltpl(hidden_outputs.oneminus()).dot(inputs.transpose()).numbmltpl(this->lr));
}

matrix qmneuralnetwork::selfActivation(matrix &x)
{
    matrix C;
    QVector <float> tmp;
    for(int i=0; i<x.Matrix.size(); i++)
    {
        tmp.push_back(1/(1+exp(x.Matrix[i][0]*(-1))));
        C.Matrix.push_back(tmp);
        tmp.clear();
    }
    return C;
}

void qmneuralnetwork::saveWeighths()
{
    QString Name = "MNNP"+QString::number(this->hnodes)+"_"+QString::number(this->lr)+this->type+".txt";
    QFile File("C:/Qt/"+Name);
    if (File.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&File);
        stream << this->inodes <<Qt::endl;
        stream << this->hnodes <<Qt::endl;
        stream << this->onodes <<Qt::endl;
        stream << this->oonodes<<Qt::endl;
        stream << this->lr <<Qt::endl;
        for(int i=0; i<this->hnodes; i++)
        {
            for(int j=0; j<this->inodes; j++)
                stream << this->mwih.Matrix[i][j]<<",";
        }
        stream << Qt::endl;
        for(int i=0; i<this->onodes; i++)
        {
            for(int j=0; j<this->hnodes; j++)
                stream << this->mwho.Matrix[i][j]<<",";
        }
        stream << Qt::endl;
        for(int i=0; i<this->oonodes; i++)
        {
            for(int j=0; j<this->onodes; j++)
                stream << this->mwoo.Matrix[i][j]<<",";
        }
        File.close();
    }
}



matrix qmneuralnetwork::query(QVector <float> &inputs_list)
{
    matrix inputs(inputs_list);
    inputs=inputs.transpose();
    inputs = this->mwih.dot(inputs);
    inputs=this->selfActivation(inputs);
    inputs=this->mwho.dot(inputs);
    inputs=this->selfActivation(inputs);
    inputs=this->mwoo.dot(inputs);
    inputs=this->selfActivation(inputs);
    //inputs.show();
    return inputs;
}
