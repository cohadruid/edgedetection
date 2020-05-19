#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdint.h>
#include <iostream>
#include <fstream>
#define PI 3.14159265
#define THRESHOLD_LOW 100
#define THRESHOLD_HIGH 200
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    Mat dst1, dst2, dst3, grad_x, grad_y, abs_grad_x, abs_grad_y, grad, gaussian, ocv_canny, custom_canny;
    int ddepth = CV_16S, ksize = 3, temp;
    double scale = 1, delta = 0, tangens, temp_dir, temp_dir_round;
    string imgnum = "85048";
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
    src1.copyTo(custom_canny, src1);
    int width = src1.cols, height = src1.rows;

    vector<uint8_t> pixels(width * height);
    vector<int> newpixels(width * height);
    vector<int> cannypixels(width * height);
    vector<double> directions(width * height);

    Sobel(gaussian, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_ISOLATED);
    Sobel(gaussian, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_ISOLATED);
    
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    Canny(gaussian, ocv_canny, 100, 200, 3, false);

    imshow(window_name, grad);
    imshow("OpenCV Canny", ocv_canny);

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

            
            temp_dir = atan((double)custom_y / (double)custom_x); //* 180 / 3.15159;
            

            if (temp_dir < -2.141421 || temp_dir >= 2.41421) temp_dir_round = 90;
            else if (temp_dir < 2.41421 && temp_dir >= 0.414214) temp_dir_round = 45;
            else if (temp_dir < 0.414214 && temp_dir >= -0.414214) temp_dir_round = 0;
            else temp_dir_round = -45;

            directions[rows * width + cols] = temp_dir_round;

            //printf("PosX: %d PosY: %d\n", rows, cols);
            /*printf("Gx = %d , Gy = %d\n", custom_x, custom_y);
            printf("Gy/Gx = %f\n", tangens);*/
            //printf("atan(Gy/Gx) = %f\n\n", temp_dir_round);
            //printf("Zaokruzeno = %.2f\n\n", temp_dir_round);

            if (abs(custom_x) > 255) custom_x = 255;
            if (abs(custom_y) > 255) custom_y = 255;

            new_value = abs(custom_x) * 0.5 + abs(custom_y) * 0.5;

            newpixels[rows * width + cols] = new_value;
            dst3.at<uint8_t>(rows, cols) = (uint8_t)new_value;
           
        }
    }

    for (int rows = 1; rows < height-1; rows++)
    {
        for (int cols = 1; cols < width-1; cols++)
        {
            temp_dir = directions[rows * width + cols];

            if (temp_dir == 0)
            {
                if (newpixels[rows * width + cols] < newpixels[rows * width + cols - 1] || newpixels[rows * width + cols] < newpixels[rows * width + cols + 1]) temp = 0;
                else temp = newpixels[rows * width + cols];
            }

            else if (temp_dir == 45)
            {
                if (newpixels[rows * width + cols] < newpixels[(rows - 1) * width + cols - 1] || newpixels[rows * width + cols] < newpixels[(rows + 1) * width + cols + 1]) temp = 0;
                else temp = newpixels[rows * width + cols];
            }

            else if (temp_dir == -45)
            {
                if (newpixels[rows * width + cols] < newpixels[(rows + 1) * width + cols - 1] || newpixels[rows * width + cols] < newpixels[(rows + 1) * width + cols - 1]) temp = 0;
                else temp = newpixels[rows * width + cols];
            }

            else if (temp_dir == 90)
            {
                if (newpixels[rows * width + cols] < newpixels[(rows - 1) * width + cols] || newpixels[rows * width + cols] < newpixels[(rows + 1) * width + cols + 1]) temp = 0;
                else temp = newpixels[rows * width + cols];
            }

            

            custom_canny.at<uint8_t>(rows, cols) = temp;
            cannypixels[rows * width + cols] = temp;
        } 
    }

    imshow("thinned edges", custom_canny);

    for (int rows = 1; rows < height-1; rows++)
    {
        for (int cols = 1; cols < width-1; cols++)
        {
            temp = custom_canny.at<uint8_t>(rows, cols);

            if (temp > THRESHOLD_HIGH) temp = 255;
            else if (temp < THRESHOLD_LOW) temp = 0;

            else
            {
                
            }
            cannypixels[rows * width + cols] = temp;
            //custom_canny.at<uint8_t>(rows, cols) = temp;
        }
    }
    //imshow(window_name_2, dst3);
    imshow("thresholding", custom_canny);
    waitKey();

    return 0;
}
