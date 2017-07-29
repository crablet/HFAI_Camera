#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High);
inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element);

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

        auto Element = getStructuringElement(MORPH_RECT, Size(13, 13));
        ClosingOperation(BlueFrame, BlueFrame, Element);

        vector<vector<Point>> Contours;
        findContours(BlueFrame, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        Mat Result(Frame.size(), CV_8U, Scalar(255));
        drawContours(Result, Contours, -1, Scalar(0), 3);

        for (const auto &r : Contours)
        {
            Moments m = moments(r, true);    // True or false?
            double AreaOfContour = contourArea(r);
            if (AreaOfContour < 50)
            {
                continue;
            }
            cout << "(" << m.m10 / m.m00 << ", " << m.m01 / m.m00 << ")" << endl;   // Print middle points.
            cout << AreaOfContour << endl;  // Print the area of the contour.
        }

        imshow("Out", Result);
        imshow("In", Frame);
        waitKey(30);
    }
    return 0;
}

inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High)
{
    inRange(In, Low, High, Out);
}

inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element)
{
    dilate(In, Out, Element);
    erode(In, Out, Element);
}