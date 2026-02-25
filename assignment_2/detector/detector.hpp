#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

class BulletDetector {
private:
    Mat cameraMatrix;
    Mat distCoeffs;
    const float BULLET_RADIUS = 17.0f;
    
public:
    BulletDetector();
    void initCameraParams();
    float calculateDistance(float imageRadius, const Point2f& center);
    vector<Point2f> detectBullets(const Mat& image, Mat& resultImage);
};

#endif
