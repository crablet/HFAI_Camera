#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat SrcImage, MaskImage;
Point PrevPt(-1, -1);

static void OnMouseHandle(int event, int x, int y, int flags, void*);

int main()
{
    SrcImage = imread("1.jpg");
    imshow("In", SrcImage);

    Mat TempImage, GrayImage;
    SrcImage.copyTo(TempImage);

    cvtColor(SrcImage, MaskImage, COLOR_BGR2GRAY);
    cvtColor(MaskImage, GrayImage, COLOR_GRAY2BGR);

    MaskImage = Scalar::all(0);

    setMouseCallback("In", OnMouseHandle, nullptr);

    while (1)
    {
        if (waitKey(0))
            break;
    }
    waitKey(0);
    return 0;
}

static void OnMouseHandle(int event, int x, int y, int flags, void*)
{
    if (x < 0 || x >= SrcImage.cols || y < 0 || y >= SrcImage.rows)
        return;
    if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
    {
        PrevPt = Point(-1, -1);
    }
    else if (event == EVENT_LBUTTONDOWN)
    {
        PrevPt = Point(x, y);
    }
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
    {
        Point Now(x, y);
        if (PrevPt.x < 0)
        {
            PrevPt = Now;
        }
        line(MaskImage, PrevPt, Now, Scalar::all(255), 5);
        line(SrcImage, PrevPt, Now, Scalar::all(255), 5);
        PrevPt = Now;
        imshow("In", SrcImage);
    }
}

//void OnMouseHandle(int event, int x, int y, int flags, void *param)
//{
//    Mat &Image = *(Mat*)param;
//    switch (event)
//    {
//    case EVENT_LBUTTONDOWN:
//        Flag = true;
//        break;
//    case EVENT_LBUTTONUP:
//        Flag = false;
//        //Points.clear();
//        break;
//    case EVENT_MOUSEMOVE:
//        if (Flag)
//        {
//            Points.push_back(Point(x, y));
//            cout << x << " " << y << endl;
//            circle(Image, Point(x, y), 2, Scalar(0, 255, 0), -1);
//            for (size_t i = 0; i < Points.size() - 2; ++i)
//            {
//                if (Points[i] == Point(x, y))
//                {
//                    cout << "Get!" << endl;
//                    break;
//                }
//            }
//        }
//        break;
//    default:
//        break;
//    }
//}
