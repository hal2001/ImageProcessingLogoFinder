#pragma once

#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "ImageProcessing.h"

using namespace cv;

class ImageMoments {

private:

    double m00 = 0.;
    double m10 = 0.;
    double m01 = 0.;
    double m11 = 0.;
    double m20 = 0.;
    double m02 = 0.;

    double m21 = 0.;
    double m12 = 0.;

    double m03 = 0.;
    double m30 = 0.;

    double M00 = 0.;
    double M10 = 0.;
    double M01 = 0.;
    double M11 = 0.;
    double M20 = 0.;
    double M02 = 0.;

    double M21 = 0.;
    double M12 = 0.;
    double M30 = 0.;
    double M03 = 0.;

    double m1000 = 0.;
    double m0100 = 0.;

    void reset_moments();

    void calc_moments(Mat& mat);

public:

    ImageMoments(Mat& mat);

    ImageMoments(string logo_path);

    // gets "M" moment (M1, M3, M7,...)
    double get_moment(int mom);

    double get_classification(double x1, double y1, double z1, double x2, double y2, double z2);
};

