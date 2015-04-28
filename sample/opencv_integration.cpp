#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "videoreader.h"

const float resizeImage = 0.5f;


int main(int argc, char* argv[]) {
    VideoReader vid;
    bool success = vid.open(argv[1]);

    int frameCount = vid.frameCount();
    for (int i = 0; i < std::min(100, frameCount); ++i) {
        cv::Mat image;
        vid.read(image, i);
        cv::Mat image2;
        if (resizeImage != 1)
            cv::resize(image, image2, cv::Size(), resizeImage, resizeImage);
        else
            image2 = image;
        cv::imshow("Video", image2);
        cv::waitKey(10);
    }

    for (int i = 0; i < std::min(100, frameCount); ++i) {
        cv::Mat image;
        vid.read(image, i);
        cv::Mat image2;
        if (resizeImage != 1)
            cv::resize(image, image2, cv::Size(), resizeImage, resizeImage);
        else
            image2 = image;
        cv::imshow("Video", image2);
        cv::waitKey(10);
    }
    return 0;
}