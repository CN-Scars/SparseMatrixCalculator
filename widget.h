 #ifndef WIDGET_H
#define WIDGET_H

#include "matrix.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_readA_Button_clicked(); // 读取矩阵A

    void on_clearA_Button_clicked();    // 清空矩阵A

    void on_readB_Button_clicked(); // 读取矩阵B

    void on_clearB_Button_clicked();    // 清空矩阵B

    void updateExecuteButtonState(int index);  // 更新按钮状态

    void on_execute_Button_clicked();

    void on_save_Button_clicked();

private:
    Matrix matrixA; // 矩阵A
    Matrix matrixB; // 矩阵B
    Matrix result;  // 结果矩阵
    Ui::Widget *ui;

    void showResult(const Matrix &matrix);  // 显示结果
};
#endif // WIDGET_H
