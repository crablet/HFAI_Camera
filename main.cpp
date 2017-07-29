#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

inline void ClosingOperation(Mat &In, Mat &Out, const Mat &Element);
inline void FindColor(Mat &In, Mat &Out, const Scalar &Low, const Scalar &High);
void FindWhiteLines(Mat &InputFrame);

int main()
{
    VideoCapture Capture("in.mp4");
    while (1)
    {
        Mat Frame, HSVFrame;
        Capture >> Frame;

        if (Frame.empty())
        {
            break;
        }

        cvtColor(Frame, HSVFrame, COLOR_BGR2HSV);
        auto LowWhite = Scalar(0, 0, 178);        // 0, 0, 178
        auto HighWhite = Scalar(180, 77, 255);    // 180, 77, 255

        // FindWhiteArea
        Mat WhiteFrame;
        FindColor(HSVFrame, WhiteFrame, LowWhite, HighWhite);

        // Closing Operation
        auto Element = getStructuringElement(MORPH_RECT, Size(13, 13));
        ClosingOperation(WhiteFrame, WhiteFrame, Element);

        // To find white lines.
        FindWhiteLines(WhiteFrame);

        imshow("Out", WhiteFrame);
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

void FindWhiteLines(Mat &InputFrame)
{
    vector<Vec4i> WhiteLines;
    HoughLinesP(InputFrame, WhiteLines, 1, CV_PI / 180, 120, 125, 30);

    unsigned Count = 0;
    double PreX1 = .0, PreX2 = .0;
    double PreY1 = .0, PreY2 = .0;
    double NowX1 = .0, NowX2 = .0;
    double NowY1 = .0, NowY2 = .0;

    // It is dangerous to vist a empty vector.
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