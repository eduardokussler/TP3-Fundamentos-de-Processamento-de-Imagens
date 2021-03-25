#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

// define the threshold to use in the Canny function
#define THRESHOLD_CANNY 15
// Define the caracters to activate the functions
#define ESC 27
#define DO_NOTHING 67 // C
#define DO_NOTHING_MIN 99// c
#define SOBEL 83 // S
#define SOBEL_MIN 115 // s
#define GAUSS 66 // B
#define GAUSS_MIN 98 // b
#define EDGES 69 // E
#define EDGES_MIN 101 // e
#define GRAYSCALE 71 // G
#define GRAYSCALE_MIN 103 // g
#define RESIZE 90 // Z
#define RESIZE_MIN 122 // z
#define NEGATIVE 78// N
#define NEGATIVE_MIN 110// n
#define BRIGHTNESS 82 // R
#define BRIGHTNESS_MIN 114 // r
using namespace cv;


Mat sobelFilter(Mat frame);
Mat gaussFilter(Mat frame, int kernelSize);
Mat detectEdges(Mat frame);
Mat calcLuminance(Mat frame);
Mat resizeVideo(Mat frame);
Mat calcNegative(Mat frame);
Mat adjustBrightness(Mat frame, int value);
void displayFunctions();
void displayFrame(Mat frame, Mat resultFrame);

int main(int argc, char** argv)
{
    Mat frame;
    Mat resultFrame;
    int camera = 0;
    // Mode indicates which function is active at a given moment
    int mode = DO_NOTHING;
    // -1 indicates that no key has been pressed 
    int keyPressed = -1;
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation. 
    int value = 3;
    namedWindow("Result Video", WINDOW_NORMAL);
    namedWindow("Original", WINDOW_GUI_NORMAL);
    createTrackbar("track1", "Result Video", &value, 255, NULL);
    if (!cap.open(camera))
        return 0;
    displayFunctions();
    while(true)
    {
        cap >> frame;
        resultFrame = frame;
        if (frame.empty()) break; // end of video stream
        switch (mode)
        {
        case DO_NOTHING:
            resultFrame = frame;
            break;
        case SOBEL:
            resultFrame = sobelFilter(frame);
            break;
        case GAUSS:
            resultFrame = gaussFilter(frame, value);
            break;
        case EDGES:
            resultFrame = detectEdges(frame);
            break;
        case GRAYSCALE:
            resultFrame = calcLuminance(frame);
            break;
        case RESIZE:
            resultFrame = resizeVideo(frame);
            break;
        case NEGATIVE:
            resultFrame = calcNegative(frame);
            break;
        case BRIGHTNESS:
            resultFrame = adjustBrightness(frame, value);
            break;
        default:
            continue;
        }
        displayFrame(frame, resultFrame);
        keyPressed = waitKey(1);
        if(keyPressed == -1) {
            continue;
        } else if(keyPressed == ESC){
            break;
        } else if(keyPressed == DO_NOTHING || keyPressed == DO_NOTHING_MIN) {
            mode = DO_NOTHING;
        } else if(keyPressed == SOBEL || keyPressed == SOBEL_MIN) {
            mode = SOBEL;
        } else if(keyPressed == GAUSS || keyPressed == GAUSS_MIN) {
            mode = GAUSS;
        } else if(keyPressed == EDGES || keyPressed == EDGES_MIN) {
            mode = EDGES;
        } else if(keyPressed == GRAYSCALE || keyPressed == GRAYSCALE_MIN) {
            mode = GRAYSCALE;
        } else if(keyPressed == RESIZE || keyPressed == RESIZE_MIN) {
            mode = RESIZE;
        } else if(keyPressed == NEGATIVE || keyPressed == NEGATIVE_MIN) {
            mode = NEGATIVE;
        } else if(keyPressed == BRIGHTNESS || keyPressed == BRIGHTNESS_MIN) {
            mode = BRIGHTNESS;
        }
        
    }
    cap.release();  // release the VideoCapture object
    return 0;
}

void displayFunctions() {

}

void displayFrame(Mat frame, Mat resultFrame) {
    imshow("Original", frame);
    imshow("Result Video", resultFrame);
}

Mat sobelFilter(Mat frame) {
        int ddepth = CV_16S;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Mat src_gray;
        Mat resultFrame;
        src_gray = calcLuminance(frame);
        Sobel(src_gray, grad_x, ddepth, 1, 0, 3, 1, 0, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, 3, 1, 0, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, resultFrame, -1);
        return resultFrame;
}


Mat gaussFilter(Mat frame, int kernelSize) {
    Mat resultFrame;
    // Kernel size cannot be an even number
    kernelSize = (kernelSize % 2 == 0)?kernelSize+1:kernelSize;
    GaussianBlur(frame, resultFrame, Size(kernelSize, kernelSize), 0, 0, BORDER_DEFAULT);
    return resultFrame;
}

Mat calcLuminance(Mat frame) {
    Mat resultFrame;
    cvtColor(frame, resultFrame, COLOR_BGR2GRAY);
    return resultFrame;
}

Mat detectEdges(Mat frame) {
    frame = calcLuminance(frame);
    Mat resultFrame;
    frame = gaussFilter(frame, 9);
    Canny(frame, resultFrame, 1, THRESHOLD_CANNY, 3, false);
    return resultFrame;
}

Mat resizeVideo(Mat frame) {
    Mat resultFrame;
    resize(frame, resultFrame, Size(frame.cols/2, frame.rows/2));
    return resultFrame;
}

Mat calcNegative(Mat frame) {
    Mat temp = Mat(frame.rows, frame.cols, frame.type(),Scalar(1,1,1))*255;
    Mat resultFrame;
    subtract(temp, frame, resultFrame);
    
    return resultFrame;
}

Mat adjustBrightness(Mat frame, int value) {
    Mat resultFrame;
    frame.convertTo(resultFrame, -1, 1, value);
    return resultFrame;
}