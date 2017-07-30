#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element);
inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High);
void FindWhiteLines(Mat &InputFrame);
inline void Initalize(Mat &Frame);

int main()
{
    VideoCapture Capture("in.mp4");
    while (1)
    {
        Mat Frame;
        Capture >> Frame;

        if (Frame.empty())
        {
            break;
        }

        Initalize(Frame);

        // To find white lines.
        FindWhiteLines(Frame);

        imshow("In", Frame);
        waitKey(5);
    }
    waitKey(0);
    return 0;
}

inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element)
{
    dilate(In, Out, Element);
    erode(In, Out, Element);
}

inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High)
{
    inRange(In, Low, High, Out);
}

inline void Initalize(Mat &Frame)
{
    cvtColor(Frame, Frame, COLOR_BGR2HSV);
}

void FindWhiteLines(Mat &InputFrame)
{
    const auto LowWhite = Scalar(0, 0, 178);        // 0, 0, 178
    const auto HighWhite = Scalar(180, 77, 255);    // 180, 77, 255

    // FindWhiteArea
    Mat WhiteFrame;
    FindColor(InputFrame, WhiteFrame, LowWhite, HighWhite);

    // Closing Operation
    auto Element = getStructuringElement(MORPH_RECT, Size(13, 13));
    ClosingOperation(WhiteFrame, WhiteFrame, Element);


    vector<Vec4i> WhiteLines;
    HoughLinesP(WhiteFrame, WhiteLines, 1, CV_PI / 180, 120, 125, 30);

    unsigned Count = 0;
    double PreX1 = .0, PreX2 = .0;
    double PreY1 = .0, PreY2 = .0;
    double NowX1 = .0, NowX2 = .0;
    double NowY1 = .0, NowY2 = .0;

    // It is dangerous to visit an empty vector.
    if (!WhiteLines.empty())
    {
        PreX1 = WhiteLines[0][0], PreX2 = WhiteLines[0][2];
        PreY1 = WhiteLines[0][1], PreY2 = WhiteLines[0][3];
    }

    for (const auto &Line : WhiteLines)
    {
        if (abs(Line[0] - Line[2]) * 0.2 <  abs(Line[1] - Line[3]))
        {
            ++Count;
            if (Count % 3 == 0) // In order not to print too much lines on the screen.
            {
                // To aviod the sudden change. But the weighting remains to be modified.
                NowX1 = PreX1 * 0.2 + Line[0] * 0.8;
                NowY1 = PreY1 * 0.2 + Line[1] * 0.8;
                NowX2 = PreX2 * 0.2 + Line[2] * 0.8;
                NowY2 = PreY2 * 0.2 + Line[3] * 0.8;

                line(InputFrame, Point(NowX1, NowY1), Point(NowX2, NowY2),
                    Scalar(124, 255, 255), 3, LINE_AA);

                // Update Pres.
                PreX1 = NowX1;
                PreX2 = NowX2;
                PreY1 = NowY1;
                PreY2 = NowY2;
            }
            else if (Count > 30000) // To avoid the overflow.
            {
                Count = 0;
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


//#include <opencv2/opencv.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
//inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High);
//inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element);
//
//int main()
//{
//    VideoCapture Capture("in.mp4");
//    while (1)
//    {
//        Mat Frame, HSVFrame;
//        Capture >> Frame;
//
//        if (Frame.empty())
//        {
//            break;
//        }
//
//        cvtColor(Frame, HSVFrame, COLOR_BGR2HSV);
//
//        auto LowBlue = Scalar(100, 43, 46);
//        auto HighBlue = Scalar(124, 255, 255);
//        Mat BlueFrame;
//        FindColor(HSVFrame, BlueFrame, LowBlue, HighBlue);
//
//        auto Element = getStructuringElement(MORPH_RECT, Size(13, 13));
//        ClosingOperation(BlueFrame, BlueFrame, Element);
//
//        vector<vector<Point>> Contours;
//        findContours(BlueFrame, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//        Mat Result(Frame.size(), CV_8U, Scalar(255));
//        drawContours(Result, Contours, -1, Scalar(0), 3);
//
//        for (const auto &r : Contours)
//        {
//            Moments m = moments(r, true);    // True or false?
//            double AreaOfContour = contourArea(r);
//            if (AreaOfContour < 50)
//            {
//                continue;
//            }
//            cout << "(" << m.m10 / m.m00 << ", " << m.m01 / m.m00 << ")" << endl;   // Print middle points.
//            cout << AreaOfContour << endl;  // Print the area of the contour.
//        }
//
//        imshow("Out", Result);
//        imshow("In", Frame);
//        waitKey(5);
//    }
//    waitKey(0);
//    return 0;
//}
//
//
//
//inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element)
//{
//    dilate(In, Out, Element);
//    erode(In, Out, Element);
//}