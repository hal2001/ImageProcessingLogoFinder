#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

enum FilterTypes {
    LOW_PASS,
    GAUSS
};

static const char* EnumFilters[] = {
        "low-pass",
        "3x3 gaussian"
};

static const char* getTextForEnum(int enumVal) {
    return EnumFilters[enumVal];
}

static int filter_lp[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
};

static int filter_gauss[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
};

/*
 * A class for storing all processing operations for images
 */
class ImageProcessing {

public:
    const static int IMAGE_SIZE = 1000;
    const static int HUE_MAX = 360;

    const static int OBJ = 255;
    const static int BG = 0;

    // resize
    static void resize(Mat& mat);

    // utilities
    static void info(Mat& mat);

    // add constant value to all pixels
    static void lighten(Mat& mat, int amount);

    // change contrast:
    static void contrast(Mat& mat, float mult);

    /* median filtering
     * - specify the width and height of a window by 'w_size' value
     * - pick value from median vector at 'idx'
     */
    static void median_filter(Mat& mat, int w_size, int idx);

    // 3x3 filtering; do not include edges
    static void filter3(Mat &mat, FilterTypes type);

    static Mat split_to_hs(Mat& mat);

    // binarization over specific threshold
    static void binary(Mat &mat, int threshold);

    static void show_histogram(Mat &mat);

    static void segment(Mat hs_mat, Mat& rgb_mat, double h_min, double h_max, double s_min, double s_max);

    static void segment_2(Mat hs_mat, Mat& rgb_mat, double h_min, double h_max, double s_min, double s_max);
};
