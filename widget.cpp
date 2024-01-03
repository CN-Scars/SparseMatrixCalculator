#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->execute_Button->setEnabled(false);
    ui->save_Button->setEnabled(false);

    connect(ui->operationMode_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateExecuteButtonState(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateExecuteButtonState(int index)
{
    bool matricesNotEmpty = !matrixA.getMatrix().isEmpty() and !matrixB.getMatrix().isEmpty();
    ui->execute_Button->setEnabled(matricesNotEmpty and index != 0);
}

void Widget::on_readA_Button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开矩阵文件"), ".", tr("文本文件(*.txt)"));
    if (fileName.isEmpty())
    {
        return;
    }

    matrixA.loadFromFile(fileName);
    ui->matrixA_textBrowser->clear();
    QVector<QVector<double>> matrix = matrixA.getMatrix();
    for (const QVector<double> &row : matrix)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            ui->matrixA_textBrowser->insertPlainText(QString::number(row[i]));
            if (i != row.size() - 1)
            {
                ui->matrixA_textBrowser->insertPlainText(" ");
            }
        }
        ui->matrixA_textBrowser->insertPlainText("\n");
    }

    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("加载矩阵A成功！");
}

void Widget::on_clearA_Button_clicked()
{
    matrixA.clear();
    ui->matrixA_textBrowser->clear();
    ui->save_Button->setEnabled(false);
    result.clear();
    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("清除矩阵A成功！");
}

void Widget::on_readB_Button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开矩阵文件"), ".", tr("文本文件(*.txt)"));
    if (fileName.isEmpty())
    {
        return;
    }

    matrixB.loadFromFile(fileName);
    ui->matrixB_textBrowser->clear();
    QVector<QVector<double>> matrix = matrixB.getMatrix();
    for (const QVector<double> &row : matrix)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            ui->matrixB_textBrowser->insertPlainText(QString::number(row[i]));
            if (i != row.size() - 1)
            {
                ui->matrixB_textBrowser->insertPlainText(" ");
            }
        }
        ui->matrixB_textBrowser->insertPlainText("\n");
    }

    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("加载矩阵B成功！");
}

void Widget::on_clearB_Button_clicked()
{
    matrixB.clear();
    ui->matrixB_textBrowser->clear();
    ui->save_Button->setEnabled(false);
    result.clear();
    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("清除矩阵B成功！");
}

void Widget::on_execute_Button_clicked()
{
    // 根据操作模式进行运算
    int index = ui->operationMode_comboBox->currentIndex();

    switch (index)
    {
    case 1:
        // 判断能否相加
        if (matrixA.getMatrix().size() != matrixB.getMatrix().size() or matrixA.getMatrix()[0].size() != matrixB.getMatrix()[0].size())
        {
            ui->status_label->setText("矩阵A和矩阵B不能相加！");
            return;
        }
        result = matrixA + matrixB;
        break;
    case 2:
        // 判断能否相减
        if (matrixA.getMatrix().size() != matrixB.getMatrix().size() or matrixA.getMatrix()[0].size() != matrixB.getMatrix()[0].size())
        {
            ui->status_label->setText("矩阵A和矩阵B不能相减！");
            return;
        }
        result = matrixA - matrixB;
        break;
    case 3:
        // 判断能否相乘
        if (matrixA.getMatrix()[0].size() != matrixB.getMatrix().size())
        {
            ui->status_label->setText("矩阵A和矩阵B不能相乘！");
            return;
        }
        result = matrixA * matrixB;
        break;
    default:
        break;
    }
    showResult(result);
    ui->save_Button->setEnabled(true);
    ui->status_label->setText("计算完成！");
}

void Widget::showResult(const Matrix &matrix)
{
    ui->result_textBrowser->clear();
    QVector<QVector<double>> m = matrix.getMatrix();
    for (const QVector<double> &row : m)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            ui->result_textBrowser->insertPlainText(QString::number(row[i]));
            if (i != row.size() - 1)
            {
                ui->result_textBrowser->insertPlainText(" ");
            }
        }
        ui->result_textBrowser->insertPlainText("\n");
    }
}

void Widget::on_save_Button_clicked()
{
    // 把result矩阵保存到txt中
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存结果"), ".", tr("文本文件(*.txt)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    QVector<QVector<double>> m = result.getMatrix();
    for (const QVector<double> &row : m)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            out << row[i];
            if (i != row.size() - 1)
            {
                out << " ";
            }
        }
        out << "\n";
    }
    file.close();
    ui->status_label->setText("保存结果成功！");
}
