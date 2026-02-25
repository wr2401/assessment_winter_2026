#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

class BulletDetector {
private:
    Mat cameraMatrix;
    Mat distCoeffs;
    const float BULLET = 17.0;  // mm
    
public:
    BulletDetector() {
        initCameraParams();
    }
    
    void initCameraParams() {
        // 初始化相机参数
        cameraMatrix = (Mat_<double>(3,3) << 
            1284.5474, 0, 651.7508,
            0, 1284.2364, 526.5638,
            0, 0, 1);
            
        distCoeffs = (Mat_<double>(1,5) << 
            -0.37358, 0.17992, -0.00039, -0.00071, 0);
    }
    
    float calculateDistance(float radius, Point2f center) {
        float fx = cameraMatrix.at<double>(0,0);
        return (BULLET * fx) / (radius * 2);
    }

    vector<Point2f> detectBullets(Mat& image, Mat& result, int x) {
        vector<Point2f> centers;
        result = image.clone();
        /*
        // 1. 颜色空间转换
        Mat hsv;
        cvtColor(image, hsv, COLOR_BGR2HSV);
        
        // 2. 颜色阈值分割
        Scalar lowerGreen(35, 43, 46);
        Scalar upperGreen(77, 255, 255);
        Mat mask;
        inRange(hsv, lowerGreen, upperGreen, mask);
        */
       Mat mask = Mat::zeros(image.size(), CV_8U);
    
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.cols; j++) {
                Vec3b pixel = image.at<Vec3b>(i, j);
                uchar b = pixel[0];
                uchar g = pixel[1];
                uchar r = pixel[2];
            
                // 判断条件（可以根据实际情况调整）
                if (g > r && g > b &&          // 绿色最强
                    g > 50 &&                   // 不要太暗
                    (g - r) > 20 && (g - b) > 20) { // 明显比红蓝强
                    mask.at<uchar>(i, j) = 255;
                }
            }
        }

        // 3. 形态学去噪
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7,7));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        morphologyEx(mask, mask, MORPH_CLOSE, kernel);
        /*
        string resultPath1 = "imgs/mask" + to_string(x) + ".jpg";
        imwrite(resultPath1, mask);
        cout << "结果已保存到: " << resultPath1 << endl;
        */
        /*
        // 4. 查找轮廓
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        // 5. 分析每个轮廓
        for (size_t i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            if (area < 100) continue;
            
            Point2f center;
            float radius;
            minEnclosingCircle(contours[i], center, radius);
            
            double perimeter = arcLength(contours[i], true);
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);
            
            if (circularity > 0.7 && circularity < 1.2) {
                centers.push_back(center);
                
                // 绘制结果
                circle(result, center, radius, Scalar(0,255,0), 2);
                circle(result, center, 3, Scalar(0,0,255), -1);
                
                float dist = calculateDistance(radius, center);
                string text = format("%.0fmm", dist);
                putText(result, text, 
                       Point(center.x - 30, center.y - radius - 10),
                       FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1);
            }
        }
        */
        Mat dist;
        distanceTransform(mask, dist, DIST_L2, 3);
        
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        cout << "找到 " << contours.size() << " 个轮廓" << endl;
        
        for (auto& contour : contours) {
            double area = contourArea(contour);
            if (area < 50) continue;
            
            // 获取轮廓的外接矩形
            Rect rect = boundingRect(contour);
            
            // 在这个轮廓的范围内找所有局部最大值
            vector<pair<Point, float>> peaks;  // 点和距离值
            
            for (int i = rect.y + 1; i < rect.y + rect.height - 1; i++) {
                for (int j = rect.x + 1; j < rect.x + rect.width - 1; j++) {
                    if (i < 0 || i >= dist.rows || j < 0 || j >= dist.cols) continue;
                    if (mask.at<uchar>(i, j) == 0) continue;
                    
                    float val = dist.at<float>(i, j);
                    if (val < 10) continue;  // 半径太小不要
                    
                    // 检查是否是局部最大值（3x3窗口）
                    int isPeak = 0;
                    for (int di = -1; di <= 1; di++) {
                        for (int dj = -1; dj <= 1; dj++) {
                            if (di == 0 && dj == 0) continue;
                            if (dist.at<float>(i + di, j + dj) >= val) {
                                isPeak ++;
                            }
                        }
                    }
                    
                    if (isPeak==0) {
                        peaks.push_back({Point(j, i), val});
                    }
                }
            }
            
            // 如果没有找到局部最大值，就用全局最大值
            if (peaks.empty()) {
                float max_val = 0;
                Point max_pt;
                for (int i = rect.y; i < rect.y + rect.height; i++) {
                    for (int j = rect.x; j < rect.x + rect.width; j++) {
                        if (mask.at<uchar>(i, j) == 0) continue;
                        float val = dist.at<float>(i, j);
                        if (val > max_val) {
                            max_val = val;
                            max_pt = Point(j, i);
                        }
                    }
                }
                if (max_val > 10) {
                    peaks.push_back({max_pt, max_val});
                }
            }
            
            // 按距离值从大到小排序
            sort(peaks.begin(), peaks.end(), 
                [](auto& a, auto& b) { return a.second > b.second; });
            
            cout << "轮廓面积: " << area << ", 找到 " << peaks.size() << " 个峰值" << endl;
            
            if (peaks.size() == 1) {
                // 单球
                Point2f center(peaks[0].first.x, peaks[0].first.y);
                float radius = peaks[0].second;
                
                centers.push_back(center);
                
                circle(result, center, radius, Scalar(0,255,0), 2);
                //circle(result, center, 2, Scalar(0,0,255), -1);
                
                float distance = calculateDistance(radius, center);
                string text = format("%.0fmm", distance);
                putText(result, text, 
                    Point(center.x - 30, center.y - radius - 10),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1);
            }
            else {
                // 粘连，去掉太近的
                float min_distance_between = 30.0;  // 两个球心的最小距离
                
                for (size_t i = 0; i < peaks.size(); i++) {
                    Point2f pi(peaks[i].first.x, peaks[i].first.y);
                    float ri = peaks[i].second;
                    
                    // 检查是否离已选的太近
                    bool too_close = false;
                    for (auto& existing : centers) {
                        float d = norm(pi - existing);
                        if (d < min_distance_between) {
                            too_close = true;
                            break;
                        }
                    }
                    
                    if (!too_close && ri > 5) {
                        centers.push_back(pi);
                        
                        circle(result, pi, ri, Scalar(0,255,0), 2);
                        //circle(result, pi, 2, Scalar(0,0,255), -1);
                        
                        float distance = calculateDistance(ri, pi);
                        string text = format("%.0fmm", distance);
                        putText(result, text, 
                            Point(pi.x - 30, pi.y - ri - 10),
                            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1);
                    }
                }
            }
        }
        /*
        // 调试：保存距离图并标出峰值
        Mat dist_display;
        normalize(dist, dist_display, 0, 255, NORM_MINMAX);
        dist_display.convertTo(dist_display, CV_8U);
        cvtColor(dist_display, dist_display, COLOR_GRAY2BGR);
        
        for (auto& c : centers) {
            circle(dist_display, c, 3, Scalar(0,0,255), -1);
        }
        */
        /*
        string peaksPath = "imgs/peaks_result" + to_string(x) + ".jpg";
        imwrite(peaksPath, dist_display);
        */
        cout << "总共检测到 " << centers.size() << " 个弹丸" << endl;
        return centers;
    }
};

int main() {
    // 1. 创建检测器对象
    BulletDetector detector;
    
    for(int i = 1; i <= 2; i++) {
        // 2. 读取图像
        string imagePath = "imgs/test" + to_string(i) + ".jpg";
        Mat image = imread(imagePath);
    
        if (image.empty()) {
            cout << "错误：无法加载图像！" << endl;
            cout << "请确保：1. 文件存在 2. 路径正确" << endl;
            return -1;
        }

        // 3. 执行检测
        Mat result;
        vector<Point2f> bullets = detector.detectBullets(image, result,i);
    
        // 4. 输出结果
        cout << "检测到 " << bullets.size() << " 个小弹丸" << endl;
        for (size_t j = 0; j < bullets.size(); j++) {
            cout << "弹丸 " << j+1 << ": 位置(" 
                 << bullets[j].x << ", " << bullets[j].y << ")" << endl;
        }

        // 5. 保存结果
        string resultPath = "imgs/result" + to_string(i) + ".jpg";
        imwrite(resultPath, result);
        cout << "结果已保存到: " << resultPath << endl;
    }
    return 0;
}
