#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;

Point PrevPt(-1, -1);
Mat SrcImage, MaskImage, DstImage;

void OnMouseHandle(int event, int x, int y, int flags, void*)
{
    if (!SrcImage.data)  // When there is no picture.
        return;

    if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))  // Left button up and not drags with the left button.  
    {
        PrevPt = Point(-1, -1);
    }
    else if (event == EVENT_LBUTTONDOWN)  // Press the left button.
    {
        PrevPt = Point(x, y);
    }
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))  // Mouse moves and drags with the left button.
    {
        Point NowPt = Point(x, y);
        if (PrevPt.x < 0)
        {
            PrevPt = NowPt;
        }
        line(MaskImage, PrevPt, NowPt, Scalar(0), 2, 8, 0);   // Line on the mask template.
        line(SrcImage, PrevPt, NowPt, Scalar::all(255), 2, 8, 0); // Line on the source picture. 
        PrevPt = NowPt;
        imshow("SrcImage", SrcImage);
    }

    if (event == EVENT_RBUTTONUP)
    {
        floodFill(MaskImage, Point(x, y), Scalar(0)); // Fill the mask template.
                                                      // So where you click will result in contrary results.
        SrcImage.copyTo(DstImage, MaskImage);                   
        imshow("DstImage", DstImage);
    }

}

int main()
{
    SrcImage = imread("1.jpg");

    // Set the mask template white. 
    MaskImage.create(SrcImage.rows, SrcImage.cols, CV_8U);
    MaskImage.setTo(Scalar(255));
    imshow("SrcImage", SrcImage);

    setMouseCallback("SrcImage", OnMouseHandle, nullptr);

    waitKey(0);
    return 0;
}
