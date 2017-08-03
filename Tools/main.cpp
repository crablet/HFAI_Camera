#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

vector<Point> Points;
bool Flag = false;

void OnMouseHandle(int event, int x, int y, int flags, void *param);

int main()
{
    Mat SrcImage = imread("1.jpg");
    Mat TempImage;
    SrcImage.copyTo(TempImage);

    namedWindow("In");
    setMouseCallback("In", OnMouseHandle, &SrcImage);

    while (1)
    {
        SrcImage.copyTo(TempImage);
        imshow("In", TempImage);
        if (waitKey(0))
            break;
    }
    waitKey(0);
    return 0;
}

void OnMouseHandle(int event, int x, int y, int flags, void *param)
{
    Mat &Image = *(Mat*)param;
    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        Flag = true;
        break;
    case EVENT_LBUTTONUP:
        Flag = false;
        Points.clear();
        break;
    case EVENT_MOUSEMOVE:
        if (Flag)
        {
            Points.push_back(Point(x, y));
            cout << x << " " << y << endl;
            for (size_t i = 0; i < Points.size() - 2; ++i)
            {
                if (Points[i] == Point(x, y))
                {
                    cout << "Get!" << endl;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}