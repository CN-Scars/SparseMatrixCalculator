#include "matrix.h"
#include "qdebug.h"

#include <QFile>
#include <QTextStream>

Matrix::Matrix(int row, int column)
{
    matrix.resize(row);
    for (int i = 0; i < row; ++i)
    {
        matrix[i].resize(column, 0);
    }
}

Matrix::~Matrix()
{
    clear();
}

Matrix Matrix::operator+(const Matrix &rhs) const   // 采用三元组进行稀疏矩阵的加法运算
{
    QVector<Triple> tripleA = toTriple();
    QVector<Triple> tripleB = rhs.toTriple();
    QVector<Triple> tripleC;
    int i = 0, j = 0;
    while (i < tripleA.size() && j < tripleB.size())
    {
        if (tripleA[i].row < tripleB[j].row)
        {
            tripleC.append(tripleA[i++]);
        }
        else if (tripleA[i].row > tripleB[j].row)
        {
            tripleC.append(tripleB[j++]);
        }
        else
        {
            if (tripleA[i].column < tripleB[j].column)
            {
                tripleC.append(tripleA[i++]);
            }
            else if (tripleA[i].column > tripleB[j].column)
            {
                tripleC.append(tripleB[j++]);
            }
            else
            {
                Triple t;
                t.row = tripleA[i].row;
                t.column = tripleA[i].column;
                t.value = tripleA[i].value + tripleB[j].value;
                tripleC.append(t);
                ++i;
                ++j;
            }
        }
    }
    while (i < tripleA.size())
    {
        tripleC.append(tripleA[i++]);
    }
    while (j < tripleB.size())
    {
        tripleC.append(tripleB[j++]);
    }

    Matrix result;
    result.matrix = getMatrixFromTriple(tripleC);
    return result;
}

Matrix Matrix::operator-(const Matrix &rhs) const
{
    QVector<Triple> negatedTripleB = rhs.toTriple();

    // 将 rhs 矩阵的每个元素取反
    for (Triple &triple : negatedTripleB)
    {
        triple.value = -triple.value;
    }

    // 创建一个新的 Matrix 实例来调用加法运算符
    Matrix negatedMatrix;
    negatedMatrix.matrix = getMatrixFromTriple(negatedTripleB);

    // 使用加法运算符来实现减法
    return *this + negatedMatrix;
}

Matrix Matrix::operator*(const Matrix &rhs) const
{
    QVector<Triple> tripleA = toTriple();
    QVector<Triple> tripleB = rhs.toTriple();
    QMap<QPair<double, double>, double> valueMap; // 用于存储乘积结果

    for (const auto &a : tripleA)
    {
        for (const auto &b : tripleB)
        {
            if (a.column == b.row)
            {
                QPair<double, double> pos(a.row, b.column); // 确定结果矩阵中的位置
                valueMap[pos] += a.value * b.value; // 累加到对应位置
            }
        }
    }

    QVector<Triple> tripleC;
    for (auto it = valueMap.begin(); it != valueMap.end(); ++it)
    {
        if (it.value() != 0)
        {
            Triple t;
            t.row = it.key().first;
            t.column = it.key().second;
            t.value = it.value();
            tripleC.append(t);
        }
    }

    Matrix result;
    result.matrix = getMatrixFromTriple(tripleC);
    return result;
}

QDebug operator<<(QDebug dbg, const Matrix &matrix)
{
    QVector<QVector<double>> m = matrix.getMatrix();
    for (const QVector<double> &row : m)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            dbg.nospace() << row[i];
            if (i != row.size() - 1)
            {
                dbg.nospace() << " ";
            }
        }
        dbg.nospace() << "\n";
    }
    return dbg.nospace();
}

void Matrix::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    matrix.clear();
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList list = line.split(" ");
        QVector<double> row;
        for (const QString &str : list)
        {
            row.append(str.toInt());
        }
        matrix.append(row);
    }
    triple = toTriple();
    file.close();
}

QVector<Triple> Matrix::toTriple() const
{
    QVector<Triple> triple;
    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix[i].size(); ++j)
        {
            if (matrix[i][j] != 0)
            {
                Triple t;
                t.row = i;
                t.column = j;
                t.value = matrix[i][j];
                triple.append(t);
            }
        }
    }
    return triple;
}

QVector<QVector<double> > Matrix::getMatrix() const
{
    return matrix;
}

QVector<QVector<double>> Matrix::getMatrixFromTriple(const QVector<Triple> &triple) const
{
    QVector<QVector<double>> matrix;
    for (const Triple &t : triple)
    {
        if (matrix.size() < t.row + 1)
        {
            matrix.resize(t.row + 1);
        }
        if (matrix[t.row].size() < t.column + 1)
        {
            matrix[t.row].resize(t.column + 1, 0);
        }
        matrix[t.row][t.column] = t.value;
    }
    return matrix;
}

void Matrix::clear()
{
    matrix.clear();
}
