#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

int xGradient(Mat image, int x, int y)
{
    return image.at<uint8_t>(y - 1, x - 1) +
        2 * image.at<uint8_t>(y, x - 1) +
        image.at<uint8_t>(y + 1, x - 1) -
        image.at<uint8_t>(y - 1, x + 1) -
        2 * image.at<uint8_t>(y, x + 1) -
        image.at<uint8_t>(y + 1, x + 1);
}

// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction

int yGradient(Mat image, int x, int y)
{
    return image.at<uint8_t>(y - 1, x - 1) +
        2 * image.at<uint8_t>(y - 1, x) +
        image.at<uint8_t>(y - 1, x + 1) -
        image.at<uint8_t>(y + 1, x - 1) -
        2 * image.at<uint8_t>(y + 1, x) -
        image.at<uint8_t>(y + 1, x + 1);
}

int main(int argc, char** argv)
{
    //ofstream file255, filenorm;
    //file255.open("file255.txt", ios_base::app);
    //filenorm.open("filenorm.txt", ios_base::app);
    Mat custom255, custom_norm, custom, diff255, diff_norm, grad_x, grad_y, abs_grad_x, abs_grad_y, grad, gaussian, diff_clean, nekisobel;
    int ddepth = /*CV_16U;*/ CV_32F;
    string imgnum = "385039";
    string filename = imgnum;
    filename.append(".jpg");
   
    const char* window_name = "OpenCV Edge Map";
    const char* window_name_2 = "Custom Edge Map";


    int custom_x, custom_y, temp, posx, posy, tempDiff;
    int new_value, max = 0;

    int gx, gy, sum;

    float factor;

    Mat src1 = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    src1.copyTo(custom255, src1);
    src1.copyTo(custom_norm, src1);
    src1.copyTo(custom, src1);
    src1.copyTo(diff255, src1);
    src1.copyTo(diff_norm, src1);
    src1.copyTo(diff_clean, src1);
    src1.copyTo(nekisobel, src1);
    int width = src1.cols, height = src1.rows;

    vector<uint8_t> pixels(width * height);
    vector<int> newpixels(width * height);

    Sobel(src1, grad_y, ddepth, 0, 1, 3, 1, 0, BORDER_ISOLATED);
    Sobel(src1, grad_x, ddepth, 1, 0, 3, 1, 0, BORDER_ISOLATED);

    //convertScaleAbs(grad_x, abs_grad_x);
    //convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    imshow(window_name, grad);
    //imshow("Original", src1);

    GaussianBlur(src1, gaussian, Size(3,3), 0);

    for (int rows = 0; rows < height; rows++)
    {
        for (int cols = 0; cols < width; cols++)
        {
            pixels[rows * width + cols] = gaussian.at<uint8_t>(rows, cols);
        }
    }

    for (int rows = 1; rows < height-1; rows++)
    {
        for (int cols = 1; cols < width-1; cols++)
        {
            gx = xGradient(src1, cols, rows);
            gy = yGradient(src1, cols, rows);
            sum = abs(gx) + abs(gy);
            sum = sum > 255 ? 255 : sum;
            sum = sum < 0 ? 0 : sum;
            nekisobel.at<uint8_t>(rows, cols) = sum;
        }
    }

    for (int rows = 1; rows < height - 1; rows++)
    {
        for (int cols = 1; cols < width - 1; cols++)
        {
            custom_x = (int)pixels[(rows - 1) * width + cols - 1] + 2 * (int)pixels[(rows)*width + cols - 1] + (int)pixels[(rows + 1) * width + cols - 1] - (int)pixels[(rows - 1) * width + cols + 1] - 2 * (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows + 1) * width + cols + 1];
            custom_y = (int)pixels[(rows - 1) * width + cols - 1] + 2 * (int)pixels[(rows - 1) * width + cols] + (int)pixels[(rows - 1) * width + cols + 1] - (int)pixels[(rows + 1) * width + cols - 1] - 2 * (int)pixels[(rows + 1) * width + cols] - (int)pixels[(rows + 1) * width + cols + 1];
            //new_value = sqrt(custom_x * custom_x + custom_y * custom_y);
            new_value = abs(custom_x) * 0.5 + abs(custom_y) * 0.5;

            newpixels[rows * width + cols] = new_value;
            tempDiff = abs(new_value - (int)grad.at<uint8_t>(rows, cols));
            diff_clean.at<uint8_t>(rows, cols) = (uint8_t)tempDiff;
            custom.at<uint8_t>(rows, cols) = (uint8_t)new_value;
            if (new_value > max)
            {
                posx = rows;
                posy = cols;
                max = new_value;
            }
            if (new_value > 255) new_value = 255;

            tempDiff = abs(new_value - (int)nekisobel.at<uint8_t>(rows, cols));
            custom255.at<uint8_t>(rows, cols) = (uint8_t)new_value;
            diff255.at<uint8_t>(rows, cols) = (uint8_t)tempDiff;

           // file255 << tempDiff << " ";
        }

       // file255 << endl;
    }

    factor = 255.0 / max;
    printf("max = %d @ x = %d , y = %d\n", max, posx, posy);
    //cout << factor << endl;

    for (int rows = 1; rows < height - 1; rows++)
    {
        for (int cols = 1; cols < width - 1; cols++)
        {
            temp = newpixels[rows * width + cols] * factor;
            custom_norm.at<uint8_t>(rows, cols) = (uint8_t)temp;

            tempDiff = abs(temp - (int)grad.at<uint8_t>(rows, cols));
            diff_norm.at<uint8_t>(rows, cols) = (uint8_t)tempDiff;


           // filenorm << tempDiff << " ";
        }

        //filenorm << endl;
    }


    imshow("Custom Clean", custom);
    imshow("neki sobel", nekisobel);
    imshow("Opencv Custom Clean Difference", diff_clean);
    imshow("Custom 255", custom255);
    imshow("Custom Normal", custom_norm);
    imshow("Opencv Custom255 Difference", diff255);
    imshow("Opencv CustomNorm Difference", diff_norm);

    waitKey();
    return 0;
}
