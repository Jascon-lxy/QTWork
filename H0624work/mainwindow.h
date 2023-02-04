#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QToolButton>
#include <QLabel>
#include <QFrame>
#include <QSpinBox>
#include <QFileDialog>
#include <QPainter>
#include <QTranslator>
#include <QPdfWriter>
#include <QDebug>

#include <QWhatsThis>
#include <iostream>
#include <QtCore/qmath.h>
#include<QDebug>
#include<QImage>
#include<QTimer>
#include"gray.h"
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    String imageUrl;
    String outImage;

    QString QimageUrl;
    int er_value;

    Mat toMat(QImage image);
    QImage toQImage(Mat & mat);
    QImage LabelDisplayMat(Mat mat);
    QImage gamma(QImage image);
    QImage *SideSobeldiant(QImage image,double scale,int type);
    QImage gray(QImage image);
    QImage fuhe(QImage images);
    QImage *grayScaleImg(QImage img);
    QImage erzhi(QImage image,int value);

    QImage AdjustBrightness(QImage Img, int iBrightValue);
    QImage AdjustSaturation(QImage Img, int iSaturateValue);
    QImage AdjustContrast(QImage Img, int iContrastValue);

    QImage setRGB(QImage image,int value_r, int value_g, int value_b);

    void saveFile();
    QPixmap diejia(QPixmap p1, QPixmap p2);

private slots:
    void on_action_I_triggered();

    void on_gray_btn_clicked();

    void on_res_btn_clicked();

    void on_left_btn_clicked();

    void on_right_btn_clicked();

    void on_h_btn_clicked();

    void on_v_btn_clicked();

    void on_del_btn_clicked();

    void on_Fiter_btn_clicked();

    void on_Gamma_btn_clicked();

    void on_edge_btn_clicked();

    void on_horizontalSlider_liang_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_die_btn_clicked();

    void on_horizontalSlider_R_valueChanged(int value);

    void on_horizontalSlider_G_valueChanged(int value);

    void on_horizontalSlider_B_valueChanged(int value);

    void on_action_V_triggered();

    void ReadFrame();

    void on_e_btn_clicked();

    void Edge_frame();

    void on_hui_btn_clicked();

    void hui_frame();

    void on_er_btn_clicked();

    void er_frame();

    void on_res_btn_v_clicked();

    void on_action_S_triggered();

    void on_qian_btn_clicked();

    void on_action_C_triggered();

    void on_action_K_triggered();

private:
    Ui::MainWindow *ui;
    VideoCapture capture;
    QTimer *timer;
    QTimer *timer_e;
    QTimer *timer_er;
    QTimer *timer_hui;
    Mat frame;
};

#endif // MAINWINDOW_H
