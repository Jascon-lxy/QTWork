#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QPixmap>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include"opencv2/imgproc/types_c.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<QDebug>
#include<QMatrix>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include"dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadFrame()));
    timer_e=new QTimer(this);
    connect(timer_e,SIGNAL(timeout()),this,SLOT(Edge_frame()));
    timer_hui=new QTimer(this);
    connect(timer_hui,SIGNAL(timeout()),this,SLOT(hui_frame()));
    timer_er=new QTimer(this);
    connect(timer_er,SIGNAL(timeout()),this,SLOT(er_frame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_I_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("文件对话框"),
                                                                "D:", tr("图片文件(*png *jpg)"));
    QString s = fileNames[0];
    QimageUrl = s;
    imageUrl=s.toStdString();
    QPixmap image;
    image.load(s);
    //image = image.scaled(ui->image_label->width(), ui->image_label->height(), Qt::KeepAspectRatio);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(image);
    ui->image_label->show();
}

void MainWindow::on_gray_btn_clicked()
{
    outImage = "D:\gray.png";
    Gray gray;
    gray.setOutName(outImage);
    gray.setImageName(imageUrl);
    gray.grayImage();

    QString imageurlin = QString::fromStdString(outImage);
    QPixmap image;
    image.load(imageurlin);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(image);
    ui->image_label->show();

    QFile::remove(imageurlin);
}

Mat MainWindow::toMat(QImage image)
{
    cv::Mat mat;
        qDebug() << image.format();
        switch(image.format())
        {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        }
        return mat;
}

QImage MainWindow::toQImage(Mat &mat)
{
    if(mat.type() == CV_8UC1)
        {
            QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
            // Set the color table (used to translate colour indexes to qRgb values)
            image.setColorCount(256);
            for(int i = 0; i < 256; i++)
            {
                image.setColor(i, qRgb(i, i, i));
            }
            // Copy input Mat
            uchar *pSrc = mat.data;
            for(int row = 0; row < mat.rows; row ++)
            {
                uchar *pDest = image.scanLine(row);
                memcpy(pDest, pSrc, mat.cols);
                pSrc += mat.step;
            }
            return image;
        }
        // 8-bits unsigned, NO. OF CHANNELS = 3
        else if(mat.type() == CV_8UC3)
        {
            // Copy input Mat
            const uchar *pSrc = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        else if(mat.type() == CV_8UC4)
        {
            qDebug() << "CV_8UC4";
            // Copy input Mat
            const uchar *pSrc = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return image.copy();
        }
        else
        {
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            return QImage();
        }
}
QImage MainWindow::LabelDisplayMat(Mat mat)
{
    Mat Rgb;
    QImage Img;
    if (mat.channels() == 3)//RGB Img
    {
        cv::cvtColor(mat, Rgb, CV_BGR2RGB);
        Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
    }
    else//Gray Img
    {
        Img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
    }
   return Img;
}

void MainWindow::on_res_btn_clicked()
{
    ui->horizontalSlider_liang->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
    ui->horizontalSlider_4->setValue(0);

    ui->label_liang->setText(QString::number(0));
    ui->label_bao->setText(QString::number(0));
    ui->label_dui->setText(QString::number(0));
    ui->label_er->setText(QString::number(0));

    QString imageurlin = QString::fromStdString(imageUrl);
    QPixmap image;
    image.load(imageurlin);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(image);
    ui->image_label->show();
}

void MainWindow::on_left_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
        //current_img->save(filename);
    QImage image  = current_img->toImage();
    QMatrix matrix;
    matrix.rotate(-90.0);
    image = image.transformed(matrix,Qt::FastTransformation);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(QPixmap::fromImage(image));
    ui->image_label->show();
}

void MainWindow::on_right_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
        //current_img->save(filename);
    QImage image  = current_img->toImage();
    QMatrix matrix;
    matrix.rotate(90.0);
    image = image.transformed(matrix,Qt::FastTransformation);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(QPixmap::fromImage(image));
    ui->image_label->show();
}

void MainWindow::on_h_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
        //current_img->save(filename);
    QImage image  = current_img->toImage();
    image = image.mirrored(true, false);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(QPixmap::fromImage(image));
    ui->image_label->show();
}

void MainWindow::on_v_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
        //current_img->save(filename);
    QImage image  = current_img->toImage();
    image = image.mirrored(false, true);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(QPixmap::fromImage(image));
    ui->image_label->show();
}

void MainWindow::on_del_btn_clicked()
{
    imageUrl = "";
    ui->image_label->clear();
}

void MainWindow::on_Fiter_btn_clicked()
{
    Gray me;
    outImage = "D:\gray.png";
    me.setImageName(imageUrl);
    me.setOutName(outImage);
    me.MeanImage();

    QString imageurlin = QString::fromStdString(outImage);
    QPixmap image;
    image.load(imageurlin);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(image);
    ui->image_label->show();

    QFile::remove(imageurlin);

}

QImage MainWindow::gamma(QImage image){
    double d=1.2;
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

void MainWindow::on_Gamma_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
        //current_img->save(filename);
    QImage img = current_img->toImage();
    QImage image=gamma(img);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(QPixmap::fromImage(image));
    ui->image_label->setAlignment(Qt::AlignCenter);
    ui->image_label->show();
}

QImage* MainWindow:: SideSobeldiant(QImage image,double scale,int type)
{
    QImage* newImage = new QImage(image.width(),image.height(),QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    QColor color4;
    QColor color5;
    QColor color6;
    QColor color7;
    QColor color8;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 1; y < image.height() - 1; y++)
    {
        for(int x = 1; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));

            color1=   QColor ( image.pixel(x-1,y-1));
            color2 =   QColor ( image.pixel(x,y-1));
            color3 =   QColor ( image.pixel(x+1,y));

            color4 =   QColor ( image.pixel(x-1,y));
            color5 =   QColor ( image.pixel(x+1,y));

            color6 =   QColor ( image.pixel(x-1,y+1));
            color7=   QColor ( image.pixel(x,y+1));
            color8 =   QColor ( image.pixel(x+1,y+1));
            r = abs(color1.red() + color2.red() * 2 + color3.red() - color6.red() - color7.red() * 2 - color8.red());
            g = abs(color1.green() + color2.green() * 2 + color3.green() - color6.green() - color7.green() * 2 - color8.green());
            b = abs(color1.blue() + color2.blue() * 2 + color3.blue() - color6.blue() - color7.blue() * 2 - color8.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() + color4.red() * 2 + color6.red() - color3.red() - color5.red() * 2 - color8.red());
            g1= abs(color1.green() + color4.green() * 2 + color6.green() - color3.green() - color5.green() * 2 - color8.green());
            b1 = abs(color1.blue() + color4.blue() * 2 + color6.blue() - color3.blue() - color5.blue() * 2 - color8.blue());
            rgb1 = r1 + g1 + b1;
           if(type == 0)
           {
               if (rgb > rgb1)
                   a = rgb;
               else
                   a = rgb1;
           }
           else if(type == 1)
           {
            a = (rgb + rgb1)/2;
           }
            a = a * scale;
            a = a>255?255:a;

            newImage->setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}

void MainWindow::on_edge_btn_clicked()
{
    const QPixmap *current_img = ui->image_label->pixmap();
    QImage img = current_img->toImage();
    QImage* calImage = SideSobeldiant(img,0.3,1);
    QPixmap pix = QPixmap::fromImage(*calImage,Qt::AutoColor);
    ui->image_label->clear();
    ui->image_label->setScaledContents(true);
    ui->image_label->setPixmap(pix);
    ui->image_label->show();
}

void MainWindow::on_horizontalSlider_liang_valueChanged(int value)
{
    ui->label_liang->setText(QString::number(value));

    QImage img;
    img.load(QimageUrl);

    ui->image_label->setPixmap(QPixmap::fromImage(AdjustBrightness(img,QString::number(value).toInt())));
}

//改变亮度
QImage MainWindow::AdjustBrightness(QImage Img, int iBrightValue)
{
    int red, green, blue;
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    for (int i = 0; i < pixels; ++i)
    {
        red= qRed(data[i])+ iBrightValue;
        red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
        green= qGreen(data[i]) + iBrightValue;
        green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
        blue= qBlue(data[i]) + iBrightValue;
        blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }

    return Img;
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_bao->setText(QString::number(value));

    QImage img;
    img.load(QimageUrl);

    ui->image_label->setPixmap(QPixmap::fromImage(AdjustSaturation(img,QString::number(value).toInt())));
}
QImage MainWindow::AdjustSaturation(QImage Img, int iSaturateValue)
{
    int red, green, blue, nRed, nGreen, nBlue;
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    float Increment = iSaturateValue/100.0;

    float delta = 0;
    float minVal, maxVal;
    float L, S;
    float alpha;

    for (int i = 0; i < pixels; ++i)
    {
        nRed = qRed(data[i]);
        nGreen = qGreen(data[i]);
        nBlue = qBlue(data[i]);

        minVal = std::min(std::min(nRed, nGreen), nBlue);
        maxVal = std::max(std::max(nRed, nGreen), nBlue);
        delta = (maxVal - minVal) / 255.0;
        L = 0.5*(maxVal + minVal) / 255.0;
        S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

        if (Increment > 0)
        {
            alpha = std::max(S, 1 - Increment);
            alpha = 1.0 / alpha - 1;
            red = nRed + (nRed - L*255.0)*alpha;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - L*255.0)*alpha;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - L*255.0)*alpha;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }
        else
        {
            alpha = Increment;
            red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
        }

        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }

    return Img;
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_dui->setText(QString::number(value));

    QImage img;
    img.load(QimageUrl);

    ui->image_label->setPixmap(QPixmap::fromImage(AdjustContrast(img,QString::number(value).toInt())));
}

//调制对比度
QImage MainWindow::AdjustContrast(QImage Img, int iContrastValue)
{
    int pixels = Img.width() * Img.height();
    unsigned int *data = (unsigned int *)Img.bits();

    int red, green, blue, nRed, nGreen, nBlue;

    if (iContrastValue > 0 && iContrastValue < 100)
    {
        float param = 1 / (1 - iContrastValue / 100.0) - 1;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * param;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * param;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * param;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    else
    {
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * iContrastValue / 100.0;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * iContrastValue / 100.0;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * iContrastValue / 100.0;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }

    return Img;
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_er->setText(QString::number(value));
    QImage erzhiImage;
    erzhiImage.load(QimageUrl);
    QImage images=gray(erzhiImage);
    int height=images.height();
    int width=images.width();
            int bt;
            QColor oldColor;
            for (int i = 0; i < height; ++i)
            {
                for(int j=0;j<width;++j){
                    oldColor = QColor(images.pixel(j,i));
                    bt = oldColor.red();
                if(bt<value){
                    bt=0;
                }else{
                    bt=255;
                }
                images.setPixel(j,i, qRgb(bt, bt, bt));

                }
            }

    ui->image_label->setPixmap(QPixmap::fromImage(images));
    ui->image_label->setAlignment(Qt::AlignCenter);
}
//二值灰度化
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

        for(int y = 0; y < newImage.height(); y++)
        {
            for(int x = 0; x < newImage.width(); x++)
            {
                oldColor = QColor(image.pixel(x,y));
                int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                newImage.setPixel(x, y, qRgb(average, average, average));
            }
        }
        return newImage;
}

void MainWindow::on_die_btn_clicked()
{
    QImage images;
    images.load(QimageUrl);
    QImage image2 =fuhe(images);

    ui->image_label->setPixmap(QPixmap::fromImage(image2));
    ui->image_label->setAlignment(Qt::AlignCenter);
}

//原图+边缘滤波复合调用函数
QImage MainWindow::fuhe(QImage images){
    QImage image2 =images.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < images.height() - 1; y++)
    {
        for(int x = 0; x < images.width() - 1; x++)
        {
            color0 =   QColor ( images.pixel(x,y));
            color1 =   QColor ( images.pixel(x + 1,y));
            color2 =   QColor ( images.pixel(x,y + 1));
            color3 =   QColor ( images.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            image2.setPixel(x,y,qRgb(a,a,a));
        }
    }
QImage image;
image.load(QimageUrl);
int red, green, blue;
int red2,green2,blue2;
    int pixels = image.width() * image.height();

    unsigned int *data = (unsigned int *)image.bits();
    unsigned int *data2 = (unsigned int *)image2.bits();

    for (int i = 0; i < pixels; ++i)
    {
        red= qRed(data[i]);
        red2=qRed(data2[i])*0.5+red*0.5;
        red2 = (red2 < 0x00) ? 0x00 : (red2 > 0xff) ? 0xff : red2;

        green= qGreen(data[i]);
        green2= qGreen(data2[i])*0.5+green*0.5;
        green2 = (green2 < 0x00) ? 0x00 : (green2 > 0xff) ? 0xff : green2;

        blue= qBlue(data[i]);
        blue2= qBlue(data2[i])*0.5+blue*0.5;
        blue2 =  (blue2  < 0x00) ? 0x00 : (blue2  > 0xff) ? 0xff : blue2 ;

        data2[i] = qRgba(red2, green2, blue2, qAlpha(data2[i]));
    }
    return image2;
}

QImage MainWindow::setRGB(QImage image,int value_r, int value_g, int value_b){
    int r,g,b;
    QColor oldColor;
    int height=image.height();
    int width=image.width();
    for (int i = 0; i < height; ++i)
    {
        for(int j=0;j<width;++j){
            oldColor = QColor(image.pixel(j,i));
            r=oldColor.red()+value_r;
            if(r>255)
                r=255;
            g=oldColor.green()+value_g;
            if(g>255)
                g=255;
            b=oldColor.blue()+value_b;
            if(b>255)
                b=255;
            image.setPixel(j,i, qRgb(r, g, b));
        }
    }
    return image;
}

void MainWindow::on_horizontalSlider_R_valueChanged(int value)
{
    QImage img;
    img.load(QimageUrl);
    int value_r=value;
    int value_g=ui->horizontalSlider_G->value();
    int value_b=ui->horizontalSlider_B->value();
    QImage Image=setRGB(img,value_r,value_g,value_b);
    ui->image_label->setPixmap(QPixmap::fromImage(Image));
    ui->image_label->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_horizontalSlider_G_valueChanged(int value)
{
    QImage img;
    img.load(QimageUrl);
    int value_r=ui->horizontalSlider_R->value();
    int value_g=value;
    int value_b=ui->horizontalSlider_B->value();
    QImage Image=setRGB(img,value_r,value_g,value_b);
    ui->image_label->setPixmap(QPixmap::fromImage(Image));
    ui->image_label->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_horizontalSlider_B_valueChanged(int value)
{
    QImage img;
    img.load(QimageUrl);
    int value_r=ui->horizontalSlider_R->value();
    int value_g=ui->horizontalSlider_G->value();
    int value_b=value;
    QImage Image=setRGB(img,value_r,value_g,value_b);
    ui->image_label->setPixmap(QPixmap::fromImage(Image));
    ui->image_label->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_action_V_triggered()
{
    timer_e->stop();
    timer_er->stop();
    timer_hui->stop();
    capture.release();//释放图像
    capture.open(0);
    timer->start(25);
}
void MainWindow::ReadFrame()
{
    //获取图像帧
    capture>>frame;
    /*
    //将抓取到的帧,转换为QImage格式,QImage::Format_RGB888使用24位RGB格式（8-8-8）存储图像
    //此时没有使用rgbSwapped()交换所有像素的红色和蓝色分量的值，底色偏蓝
    QImage image = QImage((const uchar*)frame.data,frame.cols, frame.rows,QImage::Format_RGB888);
    //将图片显示到label上
    ui->label->setPixmap(QPixmap::fromImage(image));
    */

    //将视频显示到label上
    QImage image = QImage((const uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
    ui->video_label->clear();
    ui->video_label->setScaledContents(true);
    ui->video_label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_e_btn_clicked()
{
    timer->stop();
    timer_er->stop();
    timer_hui->stop();
    capture.release();
    capture.open(0);
    timer_e->start(25);
}

void MainWindow::Edge_frame()
{
   capture>>frame;
   QImage image = QImage((const uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
   QImage*calImage = SideSobeldiant(image,0.3,1);
   ui->video_label->clear();
   ui->video_label->setScaledContents(true);
   ui->video_label->setPixmap(QPixmap::fromImage(*calImage));
}

void MainWindow::on_hui_btn_clicked()
{
    timer->stop();
    timer_e->stop();
    timer_er->stop();
    capture.release();
    capture.open(0);
    timer_hui->start(25);
}

void MainWindow::hui_frame()
{
   capture>>frame;
   QImage image = QImage((const uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
   QImage*calImage = grayScaleImg(image);
   ui->video_label->clear();
   ui->video_label->setScaledContents(true);
   ui->video_label->setPixmap(QPixmap::fromImage(*calImage));
}
QImage* MainWindow::grayScaleImg(QImage img)
{
    int nWidth = img.width();
    int nHeight = img.height();

    QImage* grayImg = new QImage(nWidth, nHeight, QImage::Format_ARGB32);
    for (int x = 0; x < nWidth; ++x)
    {
        for (int y = 0; y < nHeight; ++y)
        {
            QColor color;
            color = QColor(img.pixel(x,y));
            int grayVal = (color.red()+color.green()+color.blue())/3;
            grayImg->setPixel(x, y, QColor(grayVal, grayVal, grayVal).rgb());
        }
    }

    return grayImg;
}

void MainWindow::on_er_btn_clicked()
{
    er_value = ui->er_spinBox->value();
    timer->stop();
    timer_e->stop();
    timer_hui->stop();
    capture.release();
    capture.open(0);
    timer_er->start(25);
}
void MainWindow::er_frame()
{
   capture>>frame;
   QImage image = QImage((const uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
   QImage images=erzhi(image,er_value);
   ui->video_label->clear();
   ui->video_label->setScaledContents(true);
   ui->video_label->setPixmap(QPixmap::fromImage(images));
}
QImage MainWindow::erzhi(QImage image,int value)
{
    QImage images=gray(image);
    int height=images.height();
    int width=images.width();
    int bt;
    QColor oldColor;
    for (int i = 0; i < height; ++i)
    {
        for(int j=0;j<width;++j){
            oldColor = QColor(images.pixel(j,i));
            bt = oldColor.red();
            if(bt<value){
                bt=0;
            }else{
                bt=255;
            }
            images.setPixel(j,i, qRgb(bt, bt, bt));

        }
    }

    return images;
}

void MainWindow::on_res_btn_v_clicked()
{
    timer_e->stop();
    timer_er->stop();
    timer_hui->stop();
    capture.release();//释放图像
    capture.open(0);
    timer->start(25);
}

void MainWindow::on_action_S_triggered()
{
    QFileInfo file("D:\\num.png");
    bool isfile = file.isFile();
    if(isfile){
       saveFile();
    }else{
        QString filename = QFileDialog::getSaveFileName(this,
                        tr("Save Image"),
                        "",
                        tr("*.png;; *.jpg;;"));
        const QPixmap *current_img = ui->image_label->pixmap();
        current_img->save(filename);
    }
}
void MainWindow::saveFile()
{
    //qDebug()<<"函数到了"<<endl;
    const QPixmap *current_img = ui->image_label->pixmap();
    //qDebug()<<current_img<<endl;
    current_img->save("D:\\1.jpg");
    //qDebug()<<"开始找图片了"<<endl;
    QPixmap im1,im2;
    im1.load("D:\\num.png");
    qDebug()<<"没找到"<<endl;
    im2.load("D:\\1.jpg");
    im2 = diejia(im2,im1);
    QString filename = QFileDialog::getSaveFileName(this,
                tr("Save Image"),
                "",
                tr("*.png;; *.jpg;;"));

    //current_img->save("D:\\1.jpg");
    im2.save(filename);
    QFile::remove("D:\\num.png");
    QFile::remove("D:\\1.jpg");
}
QPixmap MainWindow::diejia(QPixmap p1, QPixmap p2)
{
    QPainter painter(&p1);
    painter.drawPixmap(0,0,p2.width(),p2.height(), p2);
    return p1;
}

void MainWindow::on_qian_btn_clicked()
{
    Dialog *q = new Dialog();
    q->show();
}

void MainWindow::on_action_C_triggered()
{
    QTranslator translator;
    translator.load(":/qm/My_zh_CN.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void MainWindow::on_action_K_triggered()
{
    QTranslator translator;
    translator.load(":/qm/my_zh_EN.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}
