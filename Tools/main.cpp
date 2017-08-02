#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void OnMouse(int event, int x, int y, int flags, void *param);
bool Flag = true;
int X = 0, Y = 0;
vector<Point> Points;

int main()
{
    Mat Image = imread("1.jpg");
    namedWindow("In");
    setMouseCallback("In", OnMouse);
    imshow("In", Image);
    while (1)
    {
        if (waitKey(0))
            break;
    }
    for (size_t i = 0; i < Points.size(); ++i)
    {
        //cout << Points[i].x << " " << Points[i].y << endl;
        if (i > 0 && Points[i].x == Points[0].x && Points[i].y == Points[0].y)
            cout << "Get!" << endl;
    }
    waitKey(0);
    return 0;
}

void OnMouse(int event, int x, int y, int flags, void *param)
{
    if (flags == EVENT_FLAG_LBUTTON)
        Points.push_back(Point(x, y));
    //if (!Points.empty() && Points[0] == Points[Points.size() - 1])
    //    return;
}