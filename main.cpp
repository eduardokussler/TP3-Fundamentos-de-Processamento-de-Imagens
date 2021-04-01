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
#define ROTATE_LEFT 74// J
#define ROTATE_LEFT_MIN 106//j
#define ROTATE_RIGHT 75// K
#define ROTATE_RIGHT_MIN 107//k
#define FLIP_HORIZONTAL 72// H
#define FLIP_HORIZONTAL_MIN 104// h
#define FLIP_VERTICAL 73//I
#define FLIP_VERTICAL_MIN 105//i
#define CONTRAST 79//O
#define CONTRAST_MIN 111//o
#define RECORDING 84// T
#define RECORDING_MIN 116//t
using namespace cv;

// TODO
//Configure recording to function with grayscale operations

Mat sobelFilter(Mat frame);
Mat gaussFilter(Mat frame, int kernelSize);
Mat detectEdges(Mat frame);
Mat calcLuminance(Mat frame);
Mat resizeVideo(Mat frame);
Mat calcNegative(Mat frame);
Mat adjustBrightness(Mat frame, int value);
Mat rotateLeft(Mat frame);
Mat rotateRight(Mat frame); 
Mat flipHorizontally(Mat frame);
Mat flipVertically(Mat frame);
Mat adjustContrast(Mat frame, int value);
void displayFunctions();
void displayFrame(Mat frame, Mat resultFrame);

int main(int argc, char** argv)
{
    Mat frame;
    Mat resultFrame;
    int camera = 0;
    // The following variables indicates what features are active
    bool sobel          = false;
    bool gauss          = false;
    bool edges          = false;
    bool grayscale      = false;
    bool resize         = false;
    bool negative       = false;
    bool brightness     = false;
    bool rotateRightF   = false;
    bool rotateLeftF    = false;
    bool recording      = false;
    bool flipHorizontal = false;
    bool flipVertical   = false;
    bool contrast       = false;
    // -1 indicates that no key has been pressed 
    int keyPressed = -1;
    // Counts how many rotations of 90º - max 3
    int rotationsLeft = 0;
    int rotationsRight = 0;
    VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation. 
    
    int value = 3;
    namedWindow("Result Video", WINDOW_NORMAL);
    namedWindow("Original", WINDOW_NORMAL);
    createTrackbar("track1", "Original", &value, 255, NULL);
    if (!cap.open(camera))
        return 0;
    cap >> frame;
    VideoWriter videoOut;
    videoOut.open("./output.avi",VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, frame.size());
    displayFunctions();
    while(true)
    {
        cap >> frame;
        resultFrame = frame;
        if (frame.empty()) break; // end of video stream
        if(sobel) {
            resultFrame = sobelFilter(resultFrame);
        }
        if(gauss) {
            resultFrame = gaussFilter(resultFrame, value);
        }
        if(edges){
            resultFrame = detectEdges(resultFrame);
        }
        if(grayscale) {
            resultFrame = calcLuminance(resultFrame);
        }
        if(resize && !recording) {
            std::cout << "Before resize: " << resultFrame.size() << std::endl;
            resultFrame = resizeVideo(resultFrame);
            resizeWindow("Result Video", resultFrame.size()/2);
            std::cout << "After resize: " << resultFrame.size() << std::endl;
        }
        if(negative) {
            resultFrame = calcNegative(resultFrame);
        }
        if(brightness) {
            resultFrame = adjustBrightness(resultFrame, value);
        }
        if(rotateLeftF && !recording) {
            for (int i = 0; i < rotationsLeft; i++)
                resultFrame = rotateRight(resultFrame);
        }
        if(rotateRightF && !recording) {
            for (int i = 0; i < rotationsRight; i++)
                resultFrame = rotateRight(resultFrame);
        }
        if(contrast) {
            resultFrame = adjustContrast(resultFrame, value);
        }
        if(flipHorizontal) {
            resultFrame = flipHorizontally(resultFrame);
        }
        if(flipVertical) {
            resultFrame = flipVertically(resultFrame);
        }
        if(recording) {
            videoOut.write(resultFrame);
        }


        displayFrame(frame, resultFrame);
        keyPressed = waitKey(1);
        if(keyPressed == -1) {
            continue;
        } else if(keyPressed == ESC){
            break;
        } else if(keyPressed == DO_NOTHING || keyPressed == DO_NOTHING_MIN) {
            sobel          = false;
            gauss          = false;
            edges          = false;
            resize         = false;
            negative       = false;
            brightness     = false;
            grayscale      = false;
            rotateRightF   = false;
            rotateLeftF    = false;
            recording      = false;
            flipHorizontal = false;
            flipVertical   = false;
            contrast       = false;
        } else if(keyPressed == SOBEL || keyPressed == SOBEL_MIN) {
            sobel = !sobel;
        } else if(keyPressed == GAUSS || keyPressed == GAUSS_MIN) {
            gauss = !gauss;
        } else if(keyPressed == EDGES || keyPressed == EDGES_MIN) {
            edges = !edges;
        } else if(keyPressed == GRAYSCALE || keyPressed == GRAYSCALE_MIN) {
            grayscale = !grayscale;
        } else if((keyPressed == RESIZE || keyPressed == RESIZE_MIN) && !recording) {
            resize = !resize;
        } else if(keyPressed == NEGATIVE || keyPressed == NEGATIVE_MIN) {
            negative = !negative;
        } else if(keyPressed == BRIGHTNESS || keyPressed == BRIGHTNESS_MIN) {
            brightness = !brightness;
        } else if((keyPressed == ROTATE_LEFT || keyPressed == ROTATE_LEFT_MIN) && !recording) {
            if(rotationsLeft == 0) {
                rotateLeftF = true;
                rotationsLeft++;
            } else if(rotationsLeft < 3) {
                rotationsLeft++;
            } else {
                rotationsLeft = 0;
                rotateLeftF = false;
            }
        } else if((keyPressed == ROTATE_RIGHT || keyPressed == ROTATE_RIGHT_MIN) && !recording) {
            if(rotationsRight == 0) {
                rotateRightF = true;
                rotationsRight++;
            } else if(rotationsRight < 3) {
                rotationsRight++;
            } else {
                rotationsRight = 0;
                rotateRightF = false;
            }
        } else if(keyPressed == FLIP_HORIZONTAL || keyPressed == FLIP_HORIZONTAL_MIN) {
            flipHorizontal = !flipHorizontal;
        } else if(keyPressed == FLIP_VERTICAL || keyPressed == FLIP_VERTICAL_MIN) {
            flipVertical = !flipVertical;
        } else if(keyPressed == RECORDING || keyPressed == RECORDING_MIN) {
            recording = !recording;
            resize = false;
            rotateLeftF = false;
            rotateRightF = false;
        } else if(keyPressed == CONTRAST || keyPressed == CONTRAST_MIN) {
            contrast = !contrast;
        }
        
    }
    cap.release();  // release the VideoCapture object
    videoOut.release();
    destroyAllWindows();
    return 0;
}

void displayFunctions() {
    std::cout.clear();
    std::cout << "Clear filters: " << (char)DO_NOTHING << " or " << (char) DO_NOTHING_MIN << std::endl;
    std::cout << "Sobel filter: " << (char) SOBEL << " or " << (char) SOBEL_MIN << std::endl;
    std::cout << "Gauss filter: " << (char) GAUSS << " or " << (char) GAUSS_MIN << std::endl;
    std::cout << "Detect edges: " << (char) EDGES << " or " << (char) EDGES_MIN << std::endl;
    std::cout << "Grayscale: " << (char) GRAYSCALE << " or " << (char) GRAYSCALE_MIN << std::endl;
    std::cout << "Resize (1/2, both dimensions): " << (char) RESIZE << " or " << (char) RESIZE_MIN << std::endl;
    std::cout << "Negative: " << (char)NEGATIVE << " or " << (char) NEGATIVE_MIN << std::endl;
    std::cout << "Enhance brightness: " << (char) BRIGHTNESS << " or " << (char) BRIGHTNESS_MIN << std::endl;
    std::cout << "Rotate video 90º left: " << (char) ROTATE_LEFT << " or " << (char) ROTATE_LEFT_MIN << std::endl;
    std::cout << "Rotate video 90º right: " << (char) ROTATE_RIGHT << " or " << (char) ROTATE_RIGHT_MIN << std::endl;
    std::cout << "Flip video horizontally: " << (char)FLIP_HORIZONTAL << " or " << (char) FLIP_HORIZONTAL_MIN << std::endl;
    std::cout << "Flip video vertically: " << (char) FLIP_VERTICAL << " or " << (char) FLIP_VERTICAL_MIN << std::endl;
    std::cout << "Change contrast: " << (char) CONTRAST << " or " << (char) CONTRAST_MIN << std::endl;
    std::cout << "Toggle recording: " << (char) RECORDING << " or " << (char) RECORDING_MIN << std::endl;
    
    std::cout << "PRESS ESC TO EXIT" << std::endl;
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
    Mat resultFrame(frame.rows, frame.cols, frame.type());
    // std::cout << "ANTES cvtColor: " << resultFrame.channels() << std::endl;
    cvtColor(frame, resultFrame, COLOR_RGB2GRAY, 3);
    cvtColor(resultFrame, resultFrame, COLOR_GRAY2RGB, 3);
    // std::cout << "DEPOIS cvtColor: "<< resultFrame.channels() << std::endl;
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

Mat rotateLeft(Mat frame) {
    Mat resultFrame;
    rotate(frame, resultFrame, ROTATE_90_COUNTERCLOCKWISE);
    return resultFrame;
}

Mat rotateRight(Mat frame) {
    Mat resultFrame;
    rotate(frame, resultFrame, ROTATE_90_CLOCKWISE);
    return resultFrame;
}

Mat flipHorizontally(Mat frame) {
    Mat resultFrame;
    flip(frame, resultFrame, 1);
    return resultFrame;
}
Mat flipVertically(Mat frame) {
    Mat resultFrame;
    flip(frame, resultFrame, 0);
    return resultFrame;
}


Mat adjustContrast(Mat frame, int value) {
    Mat resultFrame;
    frame.convertTo(resultFrame, -1, (double)value/127, 0);
    return resultFrame;
}