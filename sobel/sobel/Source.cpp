#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    Mat dst1, dst2, dst3, grad_x, grad_y, abs_grad_x, abs_grad_y, grad, gaussian;
    int ddepth = CV_16S, ksize = 3;
    double scale = 1, delta = 0;
    string imgnum = "37073";
    string filename = imgnum;
    filename.append(".jpg");
    const char* window_name = "OpenCV Edge Map";
    const char* window_name_2 = "Custom Edge Map";

    string fn_opencv = imgnum;
    fn_opencv.append("_opencv.jpg");
    string fn_custom = imgnum;
    fn_custom.append("_custom.jpg");

    short int custom_x, custom_y;
    short int new_value;

    Mat src1 = imread(samples::findFile(filename), IMREAD_GRAYSCALE);

    GaussianBlur(src1, gaussian, Size(3, 3), 0, 0);
    src1.copyTo(dst1, src1);
    src1.copyTo(dst2, src1);
    src1.copyTo(dst3, src1);
    int width = src1.cols, height = src1.rows;

    vector<uint8_t> pixels(width * height);
    vector<int> newpixels(width * height);
    vector<double> directions(width * height);

    Sobel(gaussian, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_ISOLATED);
    Sobel(gaussian, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_ISOLATED);

    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    imshow(window_name, grad);

    for (int rows = 0; rows < height; rows++)
    {
        for (int cols = 0; cols < width; cols++)
        {
            pixels[rows * width + cols] = gaussian.at<uint8_t>(rows, cols);

        }

    }

    for (int rows = 0; rows < height; rows++)
    {
        for (int cols = 0; cols < width; cols++)
        {
            if (rows == 0 && cols == 0)
            {
                custom_x = 2 * (int)pixels[(rows)*width + cols + 1] + (int)pixels[(rows + 1) * width + cols + 1];


                custom_y = 2 * (int)pixels[(rows + 1) * width + cols] + (int)pixels[(rows + 1) * width + cols + 1];


            }

            else if (rows == 0 && cols == width - 1)
            {
                custom_x = -(int)pixels[(rows + 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1];


                custom_y = 2 * (int)pixels[(rows + 1) * width + cols] + (int)pixels[(rows + 1) * width + cols - 1];


            }

            else if (rows == 0)
            {
                custom_x = -2 * (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1] + 2 * (int)pixels[(rows)*width + cols + 1] + (int)pixels[(rows + 1) * width + cols + 1];


                custom_y = (int)pixels[(rows + 1) * width + cols - 1] + 2 * (int)pixels[(rows + 1) * width + cols] + (int)pixels[(rows + 1) * width + cols + 1];


            }

            else if (rows == height - 1 && cols == 0)
            {
                custom_x = (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows)*width + cols + 1];


                custom_y = -2 * (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows - 1) * width + cols + 1];


            }

            else if (rows == height - 1 && cols == width - 1)
            {
                custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1];


                custom_y = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows - 1) * width + cols];


            }

            else if (rows == height - 1)
            {
                custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1] + (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows)*width + cols + 1];


                custom_y = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows - 1) * width + cols + 1];


            }

            else if (cols == 0)
            {
                custom_x = (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows)*width + cols + 1] + (int)pixels[(rows + 1) * width + cols + 1];


                custom_y = -2 * (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows + 1) * width + cols] + (int)pixels[(rows + 1) * width + cols + 1];


            }

            else if (cols == width - 1)
            {
                custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1];


                custom_y = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows - 1) * width + cols] + (int)pixels[(rows + 1) * width + cols - 1] + 2 * (int)pixels[(rows + 1) * width + cols];


            }

            else
            {
                custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1] + (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows)*width + cols + 1] + (int)pixels[(rows + 1) * width + cols + 1];


                custom_y = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows - 1) * width + cols + 1] + (int)pixels[(rows + 1) * width + cols - 1] + 2 * (int)pixels[(rows + 1) * width + cols] + (int)pixels[(rows + 1) * width + cols + 1];


            }


            if (abs(custom_x) > 255) custom_x = 255;
            if (abs(custom_y) > 255) custom_y = 255;

            new_value = abs(custom_x) * 0.5 + abs(custom_y) * 0.5;

            newpixels[rows * width + cols] = new_value;
            dst3.at<uint8_t>(rows, cols) = (uint8_t)new_value;

        }
    }
    imshow(window_name_2, dst3);
    waitKey();

    return 0;
}
