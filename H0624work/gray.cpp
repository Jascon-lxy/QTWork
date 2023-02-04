#include "gray.h"

#include<QDebug>
using namespace cv;
Gray::Gray()
{
    this->ImageName="D:\\Homework\\QtWork\\image\\jjy.jpg";
    this->OutName="";
}
void Gray::setImageName(String a)
{
    this->ImageName=a;
}
String Gray::getOutNamne()
{
    return this->OutName;
}
void Gray::setOutName(String a)
{
    this->OutName=a;
}

void path(String &s){
    String::size_type pos = 0;
    while ((pos = s.find('\\', pos)) != String::npos){
        s.insert(pos, "\\");
        pos = pos + 2;
    }
}

void Gray::grayImage()
{
    Mat image;
    image = imread(this->ImageName,1);
    int rows = image.rows;
    int cols = image.cols;
    int channel = image.channels();

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(channel == 3){
                int b = image.at<Vec3b>(i,j)[0];
                int g = image.at<Vec3b>(i,j)[1];
                int r = image.at<Vec3b>(i,j)[2];

                image.at<Vec3b>(i,j)[0] =0.30*r + 0.59*g +0.11*b;
                image.at<Vec3b>(i,j)[1] =0.30*r + 0.59*g +0.11*b;
                image.at<Vec3b>(i,j)[2] =0.30*r + 0.59*g +0.11*b;
            }
        }
    }
    path(this->OutName);
//    QString s =QString::fromStdString(this->OutName);
//    qDebug()<<s<<endl;
    imwrite(this->OutName,image);
}
void salt(Mat &image, int num) {
    if (!image.data) return;//防止传入空图
    int i, j;
    srand(time(NULL));
    for (int x = 0; x < num; ++x) {
        i = rand() % image.rows;
        j = rand() % image.cols;
        image.at<Vec3b>(i, j)[0] = 255;
        image.at<Vec3b>(i, j)[1] = 255;
        image.at<Vec3b>(i, j)[2] = 255;
    }
}
void Gray::MeanImage()
{
    Mat src;
    src = imread(this->ImageName,1);
    salt(src,3000);
    for (int i = 1; i<src.rows; ++i)
    {
        for (int j = 1; j < src.cols; ++j)
        {
            if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols)
            {
                    src.at<Vec3b>(i, j)[0] = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i - 1, j - 1)[0] + src.at<Vec3b>(i - 1, j)[0] + src.at<Vec3b>(i, j - 1)[0] +
                                        src.at<Vec3b>(i - 1, j + 1)[0] + src.at<Vec3b>(i + 1, j - 1)[0] + src.at<Vec3b>(i + 1, j + 1)[0] + src.at<Vec3b>(i, j + 1)[0] +
                                        src.at<Vec3b>(i + 1, j)[0]) / 9;
                    src.at<Vec3b>(i, j)[1] = (src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i - 1, j - 1)[1] + src.at<Vec3b>(i - 1, j)[1] + src.at<Vec3b>(i, j - 1)[1] +
                                        src.at<Vec3b>(i - 1, j + 1)[1] + src.at<Vec3b>(i + 1, j - 1)[1] + src.at<Vec3b>(i + 1, j + 1)[1] + src.at<Vec3b>(i, j + 1)[1] +
                                        src.at<Vec3b>(i + 1, j)[1]) / 9;
                    src.at<Vec3b>(i, j)[2] = (src.at<Vec3b>(i, j)[2] + src.at<Vec3b>(i - 1, j - 1)[2] + src.at<Vec3b>(i - 1, j)[2] + src.at<Vec3b>(i, j - 1)[2] +
                                        src.at<Vec3b>(i - 1, j + 1)[2] + src.at<Vec3b>(i + 1, j - 1)[2] + src.at<Vec3b>(i + 1, j + 1)[2] + src.at<Vec3b>(i, j + 1)[2] +
                                        src.at<Vec3b>(i + 1, j)[2]) / 9;
                }
            }
        }
       // path(this->OutName);
    //    QString s =QString::fromStdString(this->OutName);
    //    qDebug()<<s<<endl;
        imwrite(this->OutName,src);
}
Mat Gray::Mean(Mat &mat)
{
    Mat src;
    src = mat;
    salt(src,3000);
    for (int i = 1; i<src.rows; ++i)
    {
        for (int j = 1; j < src.cols; ++j)
        {
            if ((i - 1 >= 0) && (j - 1) >= 0 && (i + 1)<src.rows && (j + 1)<src.cols)
            {
                    src.at<Vec3b>(i, j)[0] = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i - 1, j - 1)[0] + src.at<Vec3b>(i - 1, j)[0] + src.at<Vec3b>(i, j - 1)[0] +
                                        src.at<Vec3b>(i - 1, j + 1)[0] + src.at<Vec3b>(i + 1, j - 1)[0] + src.at<Vec3b>(i + 1, j + 1)[0] + src.at<Vec3b>(i, j + 1)[0] +
                                        src.at<Vec3b>(i + 1, j)[0]) / 9;
                    src.at<Vec3b>(i, j)[1] = (src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i - 1, j - 1)[1] + src.at<Vec3b>(i - 1, j)[1] + src.at<Vec3b>(i, j - 1)[1] +
                                        src.at<Vec3b>(i - 1, j + 1)[1] + src.at<Vec3b>(i + 1, j - 1)[1] + src.at<Vec3b>(i + 1, j + 1)[1] + src.at<Vec3b>(i, j + 1)[1] +
                                        src.at<Vec3b>(i + 1, j)[1]) / 9;
                    src.at<Vec3b>(i, j)[2] = (src.at<Vec3b>(i, j)[2] + src.at<Vec3b>(i - 1, j - 1)[2] + src.at<Vec3b>(i - 1, j)[2] + src.at<Vec3b>(i, j - 1)[2] +
                                        src.at<Vec3b>(i - 1, j + 1)[2] + src.at<Vec3b>(i + 1, j - 1)[2] + src.at<Vec3b>(i + 1, j + 1)[2] + src.at<Vec3b>(i, j + 1)[2] +
                                        src.at<Vec3b>(i + 1, j)[2]) / 9;
                }
            }
        }
    return src;
}
