#include "ImageMoments.h"

double ImageMoments::get_moment(cv::Mat& mat, int number) {
    double mom;
    reset_moments();
    calc_moments(mat);
    switch (number) {
        case 1:
            mom = (M20 + M02) / pow(m00, 2);
            break;
        case 3:
            mom = (pow(M30 - 3 * M12, 2) + pow(3 * M21 - M03, 2)) / pow(m00, 5);
            break;
        case 7:
            mom = (M20 * M02 - pow(M11, 2)) / pow(m00, 4);
            break;
        default:
            throw 1;
    }
    return mom;
}

void ImageMoments::calc_moments(cv::Mat &mat) {
    Mat_<Vec3b> mat3 = mat;
    for (int x = 0; x < mat.rows; ++x) { // i = y
        for (int y = 0; y < mat.cols; ++y) { // j = x

            int val = mat3(x, y)[0] == ImageProcessing::OBJ ? 1 : 0;

            m00 += val;
            m01 += y * val;
            m10 += x * val;
            m11 += x * y * val;

            m02 += y * y * val;
            m20 += x * x * val;
            m12 += x * y * y * val;
            m21 += x * x * y * val;

            m03 += y * y * y * val;
            m30 += x * x * x * val;
        }
    }
    //std::cout << m00 << ", " << m01 << ", " << m10 << std::endl;

    m1000 = m10 / m00; // image center I, x
    m0100 = m01 / m00; // image center J, y

    //std::cout << m1000 << ", " << m0100 << std::endl;

    M00 = m00;
    M01 = 0.;
    M10 - 0.;
    M11 = m11 - m10 * m01 / m00;

    M20 = m20 - m10 * m10 / m00;
    M02 = m02 - m01 * m10 / m00;

    M21 = m21 - 2 * m11 * m1000 - m20 * m0100 + 2 * m01 * m1000 * m1000;
    M12 = m12 - 2 * m11 * m0100 - m02 * m1000 + 2 * m10 * m0100 * m0100;

    M30 = m30 - 3 * m20 * m1000 + 2 * m10 * m1000 * m1000;
    M03 = m03 - 3 * m02 * m0100 + 2 * m01 * m0100 * m0100;
}

void ImageMoments::reset_moments() {
    m00 = 0.;
    m10 = 0.;
    m01 = 0.;
    m11 = 0.;

    m20 = 0.;
    m02 = 0.;
    m21 = 0.;
    m12 = 0.;

    m03 = 0.;
    m30 = 0.;

    M00 = 0.;
    M10 = 0.;
    M01 = 0.;
    M11 = 0.;

    M20 = 0.;
    M02 = 0.;
    M21 = 0.;
    M12 = 0.;

    M30 = 0.;
    M03 = 0.;

    m1000 = 0.;
    m0100 = 0.;
}
