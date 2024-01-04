#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>
#include <QString>

struct Triple   // 三元组
{
    // 行号、列号、值
    int row, column;
    double value;
};

class Matrix
{
public:
    Matrix(int row = 0, int column = 0);
    ~Matrix();

    friend QDebug operator<<(QDebug dbg, const Matrix &matrix); // 重载输出运算符

    // 运算符重载
    Matrix operator+(const Matrix &rhs) const;
    Matrix operator-(const Matrix &rhs) const;
    Matrix operator*(const Matrix &rhs) const;

    void loadFromFile(const QString &fileName); // 从文件中读取矩阵
    QVector<Triple> toTriple() const; // 转换为三元组
    QVector<Triple> getTriple() const; // 获取三元组
    QVector<QVector<double>> getMatrix() const; // 获取矩阵
    QVector<QVector<double>> getMatrixFromTriple(const QVector<Triple> &triple, const int &rows, const int &columns) const; // 从三元组中获取矩阵

    void clear(); // 清空矩阵

private:
    QVector<QVector<double>> matrix; // 矩阵
    QVector<Triple> triple; // 三元组
};

#endif // MATRIX_H
