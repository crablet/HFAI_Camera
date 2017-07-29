#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void FindColor(Mat&, Mat&, const Scalar&, const Scalar&);

int main()
{
    VideoCapture Capture("in.mp4");
    while (1)
    {
        Mat Frame, HSVFrame;
        Capture >> Frame;
        cvtColor(Frame, HSVFrame, COLOR_BGR2HSV);
        auto LowBlue = Scalar(100, 43, 46);
        auto HighBlue = Scalar(124, 255, 255);
        Mat BlueFrame;
        FindColor(HSVFrame, BlueFrame, LowBlue, HighBlue);
        imshow("原始图", Frame);
        waitKey(30);
    }
    return 0;
}

void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High)
{
    inRange(In, Low, High, Out);
    imshow("处理图", Out);
}