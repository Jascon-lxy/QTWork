#ifndef GRAY_H
#define GRAY_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<QDebug>
using namespace cv;

class Gray
{
public:
    Gray();
    void grayImage();
    void setImageName(String a);
    String getOutNamne();
    void setOutName(String a);

    void MeanImage();
    Mat Mean(Mat & mat);
private:
    String ImageName;
    String OutName;
};

#endif // GRAY_H
