#include <iostream>
#include "Processing.h"
#include "ImageProcessing.h"

using namespace std;

const string INPUT_DIR = "input_data";

int main() {


    Processing p(INPUT_DIR);
    for (int i = 0; i < 4; ++i) {
        p.read_image(to_string(i + 1) + ".jpg");
        ImageProcessing::info(p[i]);
        ImageProcessing::resize(p[i]);
        Mat mat = ImageProcessing::split_to_hs(p[i]);
        //printf( "hue: %6.4lf\n", mat.at<Vec2f>(5,5)[0]);
        //printf( "sat: %6.4lf\n", mat.at<Vec2f>(5,5)[1]);
        ImageProcessing::segment(mat,p[i],0.5,0.7,0.5,1);
        imshow(to_string(i),p[i]);
    }
    cout << "Images count: " << p.image_count << endl;


//    int img = 1;
//    Processing p;
//    p.read_image("input_data/4.jpg");
//    ImageProcessing::info(p[0]);
//    ImageProcessing::resize(p[0]);
////    //ImageProcessing::filter3(p[0],GAUSS);
//    Mat mat = ImageProcessing::split_to_hs(p[0]);
////    //printf( "hue: %6.4lf\n", mat.at<Vec2f>(5,5)[0]);
////    //printf( "sat: %6.4lf\n", mat.at<Vec2f>(5,5)[1]);
//    ImageProcessing::segment(mat,p[0],0.5,0.7,0.5,1);
//    imshow(to_string(img),p[0]);

    //ImageProcessing::show_histogram(mat);


    //ImageProcessing::lighten(p[0],-100);
    //ImageProcessing::contrast(p[0],0.1);

    //ImageProcessing::median_filter(p[0],3,8);

    /*
    vector<Mat> channels = ImageProcessing::split_to_hsv(p[0]);
    string s1("channel[0] - H");
    imshow(s1,channels[0]);
    */

    //ImageProcessing::binary(p[0],100);


    waitKey(-1);
    return 0;
}