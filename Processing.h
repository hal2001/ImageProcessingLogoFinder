#pragma once
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Image.h"

using namespace std;
using namespace cv;

/*
 * A class for managing sets of images
 */
class Processing {

private:

    string input_dir;
    vector<Image*> images;

public:

    static int image_count;

    Processing();

    Processing(const string input_dir);
    virtual ~Processing();

    void read_image(string img_name);


    // access cv Mat object by the index
    Mat& operator[](size_t idx) { return images.at(idx)->mat; }

};