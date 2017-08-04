#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Point PrevPt(-1, -1);
Mat SrcImage, DstImage, MaskImage;

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

        Mat_<Vec3b> TempDstImage;
        cvtColor(DstImage, TempDstImage, COLOR_BGR2HSV);

        vector<int> H, S, V;
        for (const auto &r : TempDstImage)
        {
            if (r[0] != 0 && r[1] != 0 && r[2] != 0)
            {
                // Store HSVs.
                H.push_back(r[0]);
                S.push_back(r[1]);
                V.push_back(r[2]);
            }
        }

        // Sort and find LowHSV and HighHSV.
        sort(H.begin(), H.end());
        sort(S.begin(), S.end());
        sort(V.begin(), V.end());
        cout << "LowHSV: (" << H[0] << ", " << S[0] << ", " << V[0] << ")" << endl;
        cout << "HighHSV: (" << H[H.size() - 1] << ", " << S[S.size() - 1] << ", " << V[V.size() - 1] << ")" << endl;
    }

}

int main()
{
    SrcImage = imread("2.jpg");

    // Set the mask template white. 
    MaskImage.create(SrcImage.rows, SrcImage.cols, CV_8U);
    MaskImage.setTo(Scalar(255));
    imshow("SrcImage", SrcImage);

    setMouseCallback("SrcImage", OnMouseHandle, nullptr);

    waitKey(0);
    return 0;
}