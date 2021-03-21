// TP3-FPI.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
Mat sobelFilter(Mat frame);
int main(int argc, char** argv)
{
    
    int camera = 0;
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation. 
    int value = 50;
    int value2 = 0;
    namedWindow("Result Video", WINDOW_NORMAL);
    namedWindow("Original", WINDOW_GUI_NORMAL);
    createTrackbar("track1", "Result Video", &value, 255, NULL);
    if (!cap.open(camera))
        return 0;
    while(true)
    {
        Mat frame;
        Mat resultFrame;
       
        
        cap >> frame;
        resultFrame = frame;
        if (frame.empty()) break; // end of video stream
        resultFrame = sobelFilter(frame);
        imshow("Original", frame);
        imshow("Result Video", resultFrame);
        if (waitKey(1) == 27) break; // stop capturing by pressing ESC
        if (waitKey(1) == 118) {
            std::cout << "TESTANDO ";
        }
    }
    cap.release();  // release the VideoCapture object
    return 0;
}

Mat sobelFilter(Mat frame) {
        int ddepth = CV_16S;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Mat src_gray;
        Mat resultFrame;
        cvtColor(frame, src_gray, COLOR_BGR2GRAY);
        Sobel(src_gray, grad_x, ddepth, 1, 0, 3, 1, 0, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, 3, 1, 0, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, resultFrame, -1);
        return resultFrame;
}