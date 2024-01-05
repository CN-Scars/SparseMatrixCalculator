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
    ui->matrixA_textBrowser->setWordWrapMode(QTextOption::NoWrap);  // 设置文本不自动换行
    ui->matrixB_textBrowser->setWordWrapMode(QTextOption::NoWrap);  // 设置文本不自动换行
    ui->result_textBrowser->setWordWrapMode(QTextOption::NoWrap);   // 设置文本不自动换行
    ui->progressBar->setValue(0);

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

    // 显示进度条并设置初始值
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0); // 当最大值设置为0时，进度条显示为不确定模式
    ui->progressBar->setVisible(true);

    ui->status_label->setText("正在加载矩阵A...");
    matrixA.loadFromFile(fileName);

    ui->matrixA_textBrowser->clear();
    QVector<QVector<double>> matrix = matrixA.getMatrix();

    // 设置进度条的最大值
    ui->progressBar->setMaximum(matrix.size());
    ui->progressBar->setValue(0); // 初始进度设置为0
    ui->progressBar->setVisible(true);

    for (int rowIndex = 0; rowIndex < matrix.size(); ++rowIndex)
    {
        const QVector<double> &row = matrix[rowIndex];
        QString line;
        for (int i = 0; i < row.size(); ++i)
        {
            QString number = QString::number(row[i], 'f', 10); // 格式化为10位小数
            QString paddedNumber = QString("%1").arg(number, 20, ' '); // 20字符宽，使用空格填充
            line += paddedNumber;
        }
        ui->matrixA_textBrowser->append(line);

        // 更新进度条的当前值
        ui->progressBar->setValue(rowIndex + 1);
    }

    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("加载矩阵A成功！");

    // 输出矩阵A
    // qDebug() << "矩阵A：\n" << matrixA;
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

    // 显示进度条并设置初始值
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0); // 进度条显示为不确定模式
    ui->progressBar->setVisible(true);

    ui->status_label->setText("正在加载矩阵B...");
    matrixB.loadFromFile(fileName);

    ui->matrixB_textBrowser->clear();
    QVector<QVector<double>> matrix = matrixB.getMatrix();

    // 设置进度条的最大值
    ui->progressBar->setMaximum(matrix.size());
    ui->progressBar->setValue(0); // 初始进度设置为0

    for (int rowIndex = 0; rowIndex < matrix.size(); ++rowIndex)
    {
        const QVector<double> &row = matrix[rowIndex];
        QString line;
        for (int i = 0; i < row.size(); ++i)
        {
            QString number = QString::number(row[i], 'f', 10); // 格式化为10位小数
            QString paddedNumber = QString("%1").arg(number, 20, ' '); // 20字符宽，使用空格填充
            line += paddedNumber;
        }
        ui->matrixB_textBrowser->append(line);

        // 更新进度条的当前值
        ui->progressBar->setValue(rowIndex + 1);
    }

    updateExecuteButtonState(ui->operationMode_comboBox->currentIndex());
    ui->status_label->setText("加载矩阵B成功！");

    // 输出矩阵B
    // qDebug() << "矩阵B：\n" << matrixB;
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
    ui->status_label->setText("正在计算...");

    // 根据操作模式进行运算
    int index = ui->operationMode_comboBox->currentIndex();

    // 初始化进度条
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

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
            ui->status_label->setText("矩阵A和矩阵B不能相乘！矩阵A：" + QString::number(matrixA.getMatrix().size()) + "x" + QString::number(matrixA.getMatrix()[0].size()) + "，矩阵B：" + QString::number(matrixB.getMatrix().size()) + "x" + QString::number(matrixB.getMatrix()[0].size()));
            return;
        }
        result = matrixA * matrixB;
        break;
    default:
        break;
    }

    // 显示结果
    showResult(result);

    // 更新进度条为100%并隐藏
    ui->progressBar->setValue(100);

    // 允许保存操作
    ui->save_Button->setEnabled(true);

    // 更新状态栏
    ui->status_label->setText("计算完成！");

    // 输出结果
    // qDebug() << "计算结果：\n" << result;
}

void Widget::showResult(const Matrix &matrix)
{
    ui->result_textBrowser->clear();
    QVector<QVector<double>> m = matrix.getMatrix();
    for (const QVector<double> &row : m)
    {
        QString line;
        for (int i = 0; i < row.size(); ++i)
        {
            QString number = QString::number(row[i], 'f', 10); // 格式化为两位小数
            QString paddedNumber = QString("%1").arg(number, 20, ' '); // 10字符宽，使用空格填充
            line += paddedNumber;
        }
        ui->result_textBrowser->append(line);
    }

    // qDebug() << "计算结果：\n" << matrix;
}

void Widget::on_save_Button_clicked()
{
    // 把result矩阵保存到txt中
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存结果"), ".", tr("文本文件(*.txt)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    ui->status_label->setText("正在保存结果...");

    QTextStream out(&file);
    QVector<QVector<double>> m = result.getMatrix();

    // 显示进度条并设置初始值和最大值
    ui->progressBar->setVisible(true);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(m.size());

    for (int rowIndex = 0; rowIndex < m.size(); ++rowIndex)
    {
        const QVector<double> &row = m[rowIndex];

        for (int i = 0; i < row.size(); ++i)
        {
            QString number = QString::number(row[i], 'f', 10); // 格式化为10位小数
            if (i == 0)
            {
                out << number;
                continue;
            }
            QString paddedNumber = QString("%1").arg(number, 20, ' '); // 20字符宽，使用空格填充
            out << paddedNumber;
        }
        out << "\n";

        // 更新进度条的当前值
        ui->progressBar->setValue(rowIndex + 1);
    }

    file.close();

    ui->status_label->setText("保存结果成功！");
}

void Widget::on_clearResult_pushButton_clicked()
{
    result.clear();
    ui->result_textBrowser->clear();
    ui->save_Button->setEnabled(false);
    ui->status_label->setText("清除结果成功！");
}
