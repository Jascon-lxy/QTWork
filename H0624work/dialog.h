#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QMenu>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void on_Ok_btn_clicked();

    void on_Cal_btn_clicked();

private:
    Ui::Dialog *ui;
    bool press;
    QPoint point;
    QList<QPoint> point_list;
};

#endif // DIALOG_H
