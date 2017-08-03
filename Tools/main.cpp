//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <vector>
//
//using namespace cv;
//using namespace std;
//
//Mat SrcImage, MaskImage;
//Point PrevPt(-1, -1);
//vector<Point> Points;
//
//static void OnMouseHandle(int event, int x, int y, int flags, void*);
//
//int main()
//{
//    SrcImage = imread("1.jpg");
//    imshow("In", SrcImage);
//
//    Mat TempImage, GrayImage;
//    SrcImage.copyTo(TempImage);
//
//    cvtColor(SrcImage, MaskImage, COLOR_BGR2GRAY);
//    cvtColor(MaskImage, GrayImage, COLOR_GRAY2BGR);
//
//    MaskImage = Scalar::all(0);
//
//    setMouseCallback("In", OnMouseHandle, nullptr);
//
//    while (1)
//    {
//        if (waitKey(0))
//            break;
//    }
//    waitKey(0);
//    return 0;
//}
//
//static void OnMouseHandle(int event, int x, int y, int flags, void*)
//{
//    if (x < 0 || x >= SrcImage.cols || y < 0 || y >= SrcImage.rows)
//        return;
//    if (event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
//    {
//        PrevPt = Point(-1, -1);
//        Points.clear();
//    }
//    else if (event == EVENT_LBUTTONDOWN)
//    {
//        PrevPt = Point(x, y);
//    }
//    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
//    {
//        Point Now(x, y);
//        if (PrevPt.x < 0)
//        {
//            PrevPt = Now;
//        }
//        Points.push_back(Point(x, y));
//        line(MaskImage, PrevPt, Now, Scalar::all(255), 5);
//        line(SrcImage, PrevPt, Now, Scalar::all(255), 5);
//        PrevPt = Now;
//        imshow("In", SrcImage);
//        for (size_t i = 0; i < Points.size(); ++i)
//        {
//            if (i > 0 && i < Points.size() - 2 && Points[i] == Point(x, y))
//            {
//                cout << Points[i] << " " << Point(x, y) << endl;
//                return; // Do something.
//            }
//        }
//    }
//}

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

#include <opencv2/opencv.hpp>  


using namespace std;
using namespace cv;



Point prev_pt(-1, -1);
Mat img;

Mat img_mask;
Mat dst;


void on_mouse(int event, int x, int y, int flags, void*)
{
    if (!img.data)
        return;
    if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))  //判断事件为松开鼠标左键或者不是左拖拽  
    {
        prev_pt = Point(-1, -1);
    }
    else if (event == CV_EVENT_LBUTTONDOWN)  //判断为按下左键  
    {
        prev_pt = Point(x, y);
    }
    else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))  //判断移动鼠标并且左拖拽  
    {
        Point pt = Point(x, y);
        if (prev_pt.x < 0)
        {
            prev_pt = pt;
        }
        line(img_mask, prev_pt, pt, Scalar(0), 2, 8, 0); //模板上划线  
        line(img, prev_pt, pt, Scalar::all(255), 2, 8, 0);          //原图上划线  
        prev_pt = pt;
        imshow("image", img);





    }
    if (event == CV_EVENT_RBUTTONUP)
    {

        floodFill(img_mask, Point(x, y), Scalar(0));//填充抠图模板  
                                                    /*imshow("img_mask", img_mask);*/
        img.copyTo(dst, img_mask);
        imshow("dst", dst);


    }

}



int main()
{

    Mat image = imread("1.jpg");
    image.copyTo(img);

    //将模板设置成白色  
    img_mask.create(img.rows, img.cols, CV_8U);
    img_mask.setTo(Scalar(255));
    //显示原图  
    imshow("image", img);


    ////显示模板原图  
    //imshow("watershed transform", img_mask);  

    //鼠标回调函数  
    setMouseCallback("image", on_mouse, 0);



    waitKey(0);
    return 0;
}
