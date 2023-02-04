#include "dialog.h"
#include "ui_dialog.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSize>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setMouseTracking(true);
    this->press = false;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);         //创建画板，画板附着在对象this上，也就是Widget窗口
    QPen mypen;                     //创建画笔
    mypen.setWidth(15);             //点的大小
    mypen.setColor(Qt::black);      //点的颜色
    painter.setPen(mypen);          //画板设置画笔

    int i = 0;                      //把每一次鼠标移动的地方进行画点
    while(point_list.size()!=0 && i != point_list.size())
    {
        painter.drawPoint(point_list.at(i));
        i++;
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        this->press = true;
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(this->press)     //只有鼠标被按下的时候才记录坐标
        {
            point = event->pos();       //记录坐标
            point_list.append(point);   //记录每一次移动的坐标
            update();                   //更新，让画笔画画
        }

    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        this->press = false;
    }
}

void Dialog::on_Ok_btn_clicked()
{
    ui->Ok_btn->hide();
    ui->Cal_btn->hide();
    QPixmap pixmap = QPixmap::grabWidget(this);     //把Widget窗口内容转为QPixmap
    QSize picSize(100,100);                           //设置修改的大小为28*28
    QPixmap ret = pixmap.scaled(picSize);           //重新修改QPixmap的大小
    //m.setflag(true);
    ret.save("D:\\num.png"); //保存QPixmap为png图片
    this->close();
}

void Dialog::on_Cal_btn_clicked()
{
    point_list.clear();
    update();
}

