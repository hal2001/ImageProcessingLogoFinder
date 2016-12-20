#include "ImageProcessing.h"

void ImageProcessing::lighten(Mat &mat, int amount) {
    switch (mat.channels())  {
        case 1:
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    if (mat.at<uchar>(i, j) + amount > 255) {
                        mat.at<uchar>(i, j) = 255;
                    }
                    else if (mat.at<uchar>(i, j) + amount < 0) {
                        mat.at<uchar>(i, j) = 0;
                    }
                    else {
                        mat.at<uchar>(i, j) += amount;
                    }
                }
            break;
        case 3: {
            Mat_<Vec3b> mat3 = mat;
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    for (int k = 0; k < mat.channels(); ++k) {
                        if (mat3(i, j)[k] + amount > 255) {
                            mat3(i, j)[k] = 255;
                        } else if (mat3(i, j)[k] + amount < 0) {
                            mat3(i, j)[k] = 0;
                        } else {
                            mat3(i, j)[k] += amount;
                        }
                    }
                }
            break;
        }
        default:
            throw 1;
    }

}

void ImageProcessing::contrast(Mat &mat, float mult) {
    switch (mat.channels())  {
        case 1:
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    if (mat.at<uchar>(i, j) * mult > 255) {
                        mat.at<uchar>(i, j) = 255;
                    }
                    else if (mat.at<uchar>(i, j) * mult < 0) {
                        mat.at<uchar>(i, j) = 0;
                    }
                    else {
                        mat.at<uchar>(i, j) *= mult;
                    }
                }
            break;
        case 3: {
            Mat_<Vec3b> mat3 = mat;
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    for (int k = 0; k < mat.channels(); ++k) {
                        if (mat3(i, j)[k] * mult > 255) {
                            mat3(i, j)[k] = 255;
                        } else if (mat3(i, j)[k] * mult < 0) {
                            mat3(i, j)[k] = 0;
                        } else {
                            mat3(i, j)[k] *= mult;
                        }
                    }
                }
            break;
        }
        default:
            throw 1;
    }
}

// utilities for median filtering
struct vals {
    int mean;
    int R;
    int G;
    int B;
};

bool sorting(vals i, vals j) { return (i.mean<j.mean); }


void ImageProcessing::median_filter(Mat& mat, int w_size, int idx) {
    if (w_size > 1 && w_size < mat.cols && w_size < mat.rows &&
            idx > 0 && idx < w_size * w_size) {
        std::cout << "Applying median filter " << w_size << " x " << w_size
                  << " with idx = " << idx<< std::endl;
        switch (mat.channels()) {
            case 3: {
                Mat_<Vec3b> mat3 = mat;
                Mat_<Vec3b> mat3res = Mat(mat.rows, mat.cols, CV_8UC3);

                int offset = w_size / 2;

                for (int i = 0; i < mat.rows; ++i) {
                    for (int j = 0; j < mat.cols; ++j) {

                        vector<vals> med((unsigned long) (w_size * w_size));

                        int el = 0;
                        // iterate over pixels around me:
                        for (int k = 0; k < w_size; k++) {
                            for (int l = 0; l < w_size; l++) {

                                int c_row = i - offset + l;
                                int c_col = j - offset + k;

                                // in order not to go out of the borders
                                if ((c_row >= 0 && c_row < mat.rows) &&
                                    (c_col >= 0 && c_col < mat.cols)) {

                                    // mean:
                                    int pixVal = 0;
                                    for (int ch = 0; ch < mat.channels(); ch++) {
                                        pixVal += mat3(c_row, c_col)[ch];
                                    }
                                    med[el].mean = pixVal / 3;

                                    med[el].R = mat3(c_row, c_col)[0];
                                    med[el].G = mat3(c_row, c_col)[1];
                                    med[el].B = mat3(c_row, c_col)[2];

                                    el++;
                                }
                            }
                        }

                        // remove unused elements in a median vector
                        int unused = (int) (med.size() - el);
                        for (int n = unused; n > 0; --n) {
                            med.pop_back();
                        }

                        // sort
                        std::sort(std::begin(med), std::end(med), sorting);

                        // compensate for unused pixels
                        mat3res(i, j)[0] = (uchar) med[idx - unused].R;
                        mat3res(i, j)[1] = (uchar) med[idx - unused].G;
                        mat3res(i, j)[2] = (uchar) med[idx - unused].B;
                    }
                }
                mat = mat3res;
                break;
            }
            default:
                throw 1;
        }
    } else {
        throw 1;
    }
}

void ImageProcessing::binary(Mat& mat, int threshold){
    switch (mat.channels()) {
        case 1:
            for (int i = 1; i < mat.rows - 1; ++i) {
                for (int j = 1; j < mat.cols - 1; ++j) {
                    Vec3b &vec = mat.at<Vec3b>(i, j);
                    uchar I = mat.at<uchar>(i, j);
                    if (I > threshold) {
                        mat.at<uchar>(i, j) = 255;
                    } else {
                        mat.at<uchar>(i, j) = 0;
                    }
                }
            }
            break;
        case 3:
            for (int i = 1; i < mat.rows - 1; ++i) {
                for (int j = 1; j < mat.cols - 1; ++j) {
                    Vec3b& vec = mat.at<Vec3b>(i, j);
                    int I = (vec[0] + vec[1] + vec[2]) / 3;
                    if (I > threshold) {
                        vec[0] = vec[1] = vec[2] = 255;
                    } else {
                        vec[0] = vec[1] = vec[2] = 0;
                    }
                }
            }
            break;
        default:
            throw 1;
    }
}

float smallest(float x, float y, float z){
    return std::min({x, y, z});
}

float largest(float x, float y, float z){
    return std::max({x, y, z});
}

int smallest(int x, int y, int z){
    return std::min({x, y, z});
}

int largest(int x, int y, int z){
    return std::max({x, y, z});
}

// https://www.cs.rit.edu/~ncs/color/t_convert.html
Mat ImageProcessing::split_to_hs(Mat& mat) {
    std::cout << "Splitting input RGB values into HS(without V)..." << std::endl;
    Mat_<Vec3b> mat3 = mat;
    Mat_<Vec2f> mat_hs = Mat(mat.rows, mat.cols, CV_32FC2);
    // float matrix, two channels for H and S
    switch (mat.channels()) {
        case 3: {
            for (int i = 0; i < mat.rows; ++i) {
                for (int j = 0; j < mat.cols; ++j) {
                    // change RGB to 0..1
                    float R = (float) mat3(i, j)[0] / 255;
                    float G = (float) mat3(i, j)[1] / 255;
                    float B = (float) mat3(i, j)[2] / 255;
                    //std::cout << R << " " << G << " " << B << "\n";

                    float Cmax = largest(R, G, B);
                    float Cmin = smallest(R, G, B);
                    float delta = Cmax - Cmin;
                    float hue = 0;
                    if (Cmax == R) {
                        hue = (G - B) / delta;
                    } else if (Cmax == G) {
                        hue = (B - R) / delta + 2;
                    } else if (Cmax == B) {
                        hue = (R - G) / delta + 4;
                    }
                    hue *= 60;
                    if (hue < 0) {
                        hue += 360;
                    }

                    float sat = 0;
                    if (Cmax != 0) {
                        sat = delta / Cmax;
                    } else {
                        sat = 0;
                        hue = -1;
                    }
                    //std::cout << "H:" << hue << " S:" << sat << std::endl;

                    mat_hs(i, j)[0] = hue;
                    mat_hs(i, j)[1] = sat;
                }
            }
            return mat_hs;
        }
        default:
            throw 1;
    }
}

void ImageProcessing::resize(Mat &mat) {
    Size s;
    if (mat.rows > IMAGE_SIZE || mat.cols > IMAGE_SIZE) {
        if (mat.rows > IMAGE_SIZE) {
            s.width = IMAGE_SIZE;
            std::cout << "Changing width from " << mat.rows << " to " << IMAGE_SIZE << ".\n";
        }
        if (mat.cols > IMAGE_SIZE) {
            s.height = IMAGE_SIZE;
            std::cout << "Changing height to " << mat.cols << " to " << IMAGE_SIZE << ".\n";
        }
        cv::resize(mat, mat, s, 0, 0, CV_INTER_AREA);
    }
}

void ImageProcessing::info(Mat &mat) {
    std::cout << "Image: " << mat.rows << " x "
              << mat.cols << ", channels: " << mat.channels();
    unsigned long mean = 0;
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            mean += mat.at<uchar>(i,j);
        }
    }
    mean /= mat.rows * mat.cols;
    std::cout << ", mean pixel value: " << mean;
    std::cout << std::endl;
}

// http://stackoverflow.com/questions/8767166/passing-a-2d-array-to-a-c-function
int count_k(int (&filter)[3][3]) {
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; ++j) {
            k += filter[i][j];
        }
    }
    return k;
}

void filter3_image(Mat &mat, int k_sum, int (&filter)[3][3]) {
    switch (mat.channels()) {
        case 3: {
            Mat_<Vec3b> mat3 = mat;
            for (int i = 1; i < mat.rows - 1; ++i)
                for (int j = 1; j < mat.cols - 1; ++j) {
                    for (int k = 0; k < mat.channels(); ++k) {
                        //mat3(i, j)[k]
                        // apply mask for each channel:
                        uchar nSum = 0;
                        for (int l = 0; l < 3; ++l) {
                            for (int m = 0; m < 3; ++m) {
                                nSum += mat3(i - 1 + l, j - 1 + m)[k] * filter[l][m] * (float) 1 / k_sum;
                            }
                        }
                        mat3(i, j)[k] = nSum;
                    }
                }
            break;
        }
        default:
            throw 1;
    }
}

void ImageProcessing::filter3(Mat &mat, FilterTypes type) {
    std::cout << "Filtering with " << getTextForEnum(type) << " filter..."<< std::endl;
    switch (type) {
        case LOW_PASS: {
            int k = count_k(filter_lp);
            filter3_image(mat, k, filter_lp);
            break;
        }
        case GAUSS: {
            int k = count_k(filter_gauss);
            filter3_image(mat, k, filter_gauss);
            break;
        }
        default:
            throw 1;
    }

}

void ImageProcessing::show_histogram(Mat& mat) {
    Mat m1;
    m1 = Mat::zeros(360,255, CV_8UC3);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            float hue = mat.at<Vec2f>(i,j)[0];
            float sat = mat.at<Vec2f>(i,j)[1];
            uchar i_hue = (uchar)hue;
            uchar i_sat = (uchar)(sat * 255);
            m1.at<Vec2b>(i_sat ,i_hue)[0] = 255;
            m1.at<Vec2b>(i_sat, i_hue)[1] = 255;
            m1.at<Vec2b>(i_sat ,i_hue)[2] = 255;
        }
        //std::cout << "here2 " << std::endl;
    }
    std::cout << "here2 " << std::endl;
    //std::cout << "here: " << (int)m1.at<Vec2b>(0,0)[0];
    imshow( "H-S Histogram", m1);
    waitKey(-1);
}

void ImageProcessing::segment(Mat hs_mat, Mat& rgb_mat, double h_min, double h_max, double s_min, double s_max) {
    std::cout << "Segmenting HS image with " << h_min << " < HUE < " << h_max << ", " <<
            s_min << " < SAT < " << s_max << std::endl;
    Mat_<Vec2f> mat_hs = hs_mat;
    Mat_<Vec3f> mat_rgb = rgb_mat;
    for (int i = 0; i < mat_hs.rows; ++i) {
        for (int j = 0; j < mat_hs.cols; ++j) {
            double hue_val = (double)mat_hs(i, j)[0];
            double sat_val = (double)mat_hs(i, j)[1];
            if ((h_min * HUE_MAX < hue_val && hue_val < h_max * HUE_MAX) &&
                    (s_min < sat_val && sat_val < s_max)) {
                // ok
                mat_rgb(i,j)[0] = mat_rgb(i,j)[1] = mat_rgb(i,j)[2] = OBJ;
            } else {
                mat_rgb(i,j)[0] = mat_rgb(i,j)[1] = mat_rgb(i,j)[2] = BG;
            }
        }
    }
    rgb_mat = mat_rgb;
}

