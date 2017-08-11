#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;
using namespace cv;

inline void ClosingOperation(const Mat &In, Mat &Out, const Mat &Element);
inline void FindColor(const Mat &In, Mat &Out, const Scalar &Low, const Scalar &High);
void FindWhiteLines(const Mat &InputFrame);
inline void Initalize(Mat &Frame);
void FindTheGoal(const Mat &InputFrame);
void FindTheBall(const Mat &InputFrame);

int main()
{
    VideoCapture Capture("OrangeBall.avi");
    while (1)
    {
        Mat Frame;
        Capture >> Frame;

        if (Frame.empty())
        {
            break;
        }

        Initalize(Frame);   // Could I just work on the HSVFrame?

                            //// To find white lines.
                            //FindWhiteLines(Frame);
                            //FindTheGoal(Frame);
        FindTheBall(Frame);
        waitKey(50);
    }

    waitKey(0);
    return 0;
}

inline void ClosingOperation(const Mat &In, Mat &Out, const Mat &Element)
{
    dilate(In, Out, Element);
    erode(In, Out, Element);
}

inline void FindColor(const Mat &In, Mat &Out, const Scalar &Low, const Scalar &High)
{
    inRange(In, Low, High, Out);
}

inline void Initalize(Mat &Frame)
{
    cvtColor(Frame, Frame, COLOR_BGR2HSV);
}

void FindWhiteLines(const Mat &InputFrame)
{
    constexpr int SafeRange = numeric_limits<int>::max() - 2;
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
                NowX1 = PreX1 * 0.25 + Line[0] * 0.75;
                NowY1 = PreY1 * 0.25 + Line[1] * 0.75;
                NowX2 = PreX2 * 0.25 + Line[2] * 0.75;
                NowY2 = PreY2 * 0.25 + Line[3] * 0.75;

                line(InputFrame, Point(NowX1, NowY1), Point(NowX2, NowY2),
                    Scalar(0, 0, 255), 3, LINE_AA);

                // Update Pres.
                PreX1 = NowX1;
                PreX2 = NowX2;
                PreY1 = NowY1;
                PreY2 = NowY2;
            }
            else if (Count > SafeRange) // To avoid the overflow.
            {
                Count = 0;
            }
        }
    }
}

void FindTheGoal(const Mat &InputFrame)
{
    // We find the blue area first.
    auto LowBlue = Scalar(100, 43, 46);
    auto HighBlue = Scalar(124, 255, 255);
    Mat BlueFrame;
    FindColor(InputFrame, BlueFrame, LowBlue, HighBlue);

    // Then we improve the image quality.
    auto Element = getStructuringElement(MORPH_RECT, Size(13, 13));
    ClosingOperation(BlueFrame, BlueFrame, Element);

    // Find and draw the contour of the goal.
    vector<vector<Point>> Contours;
    findContours(BlueFrame, Contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    Mat Result(InputFrame.size(), CV_8U, Scalar(255));
    drawContours(Result, Contours, -1, Scalar(0), 3);

    // Calculte the area and find out the middle points of the goal.
    for (const auto &r : Contours)
    {
        Moments m = moments(r, true);    // True or false?
        double AreaOfContour = contourArea(r);

        // Filier the noise.
        if (AreaOfContour < 50)
        {
            continue;
        }

        cout << "(" << m.m10 / m.m00 << ", " << m.m01 / m.m00 << ")" << endl;   // Print middle points.
        cout << AreaOfContour << endl;  // Print the area of the contour.
    }

    imshow("Out", Result);
}

void FindTheBall(const Mat &InputFrame)
{
    Mat OrangeFrame;
    auto LowOrange = Scalar(5, 120, 107);                   // Note: H * 180, S * 255, V * 255
    auto HighOrange = Scalar(14, 236, 210);
    FindColor(InputFrame, OrangeFrame, LowOrange, HighOrange);

    // Better than ClosingOperation().
    GaussianBlur(OrangeFrame, OrangeFrame, Size(15, 15), 2, 2);

    vector<Vec3f> Circles;
    HoughCircles(OrangeFrame, Circles, HOUGH_GRADIENT, 1, 10, 500, 20, 15, 50); // Remained to be modified.

    if (!Circles.empty())
    {
        // Draw the circles.
        for (const auto &r : Circles)
        {
            Point Center((r[0]), r[1]);
            int Radius = r[2];
            circle(OrangeFrame, Center, 3, Scalar(0, 255, 0), -1);
            circle(OrangeFrame, Center, Radius, Scalar(155, 50, 255), 3);
        }
    }

    imshow("Out", OrangeFrame);
}