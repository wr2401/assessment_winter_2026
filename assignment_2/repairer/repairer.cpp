#include "opencv2/opencv.hpp"
#include "iostream"
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

Mat denoise_color_noise(const Mat& src, int kernel_size = 3) {
    Mat result = src.clone();
    
    for (int i = kernel_size/2; i < src.rows - kernel_size/2; i++) {
        for (int j = kernel_size/2; j < src.cols - kernel_size/2; j++) {
            
            vector<Vec3b> neighbors;
            for (int di = -kernel_size/2; di <= kernel_size/2; di++) {
                for (int dj = -kernel_size/2; dj <= kernel_size/2; dj++) {
                    neighbors.push_back(src.at<Vec3b>(i+di, j+dj));
                }
            }
            
            vector<uchar> b_vals, g_vals, r_vals;
            for (auto& n : neighbors) {
                b_vals.push_back(n[0]);
                g_vals.push_back(n[1]);
                r_vals.push_back(n[2]);
            }
            
            sort(b_vals.begin(), b_vals.end());
            sort(g_vals.begin(), g_vals.end());
            sort(r_vals.begin(), r_vals.end());
            
            result.at<Vec3b>(i, j)[0] = b_vals[b_vals.size()/2];
            result.at<Vec3b>(i, j)[1] = g_vals[g_vals.size()/2];
            result.at<Vec3b>(i, j)[2] = r_vals[r_vals.size()/2];
        }
    }
    
    return result;
}

int main(){
    Mat img = imread("imgs/noisy.jpg");
    Mat denoise;
    
    //fastNlMeansDenoisingColored(img, denoise, 8, 8, 7, 21); 

    denoise = denoise_color_noise(img, 3);

    imwrite("imgs/denoise3.jpg", denoise);
    return 0;
}