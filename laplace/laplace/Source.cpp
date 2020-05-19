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
    Mat dst1, dst2, gaussian, abs_dst2;
    int ddepth = CV_16S, ksize = 1;
    double scale = 1, delta = 0;
    string imgnum = "19021";
    string filename = imgnum;
    filename.append(".jpg");
   
    const char* window_name = "OpenCV Edge Map";
    const char* window_name_2 = "Custom Edge Map";

    string fn_opencv = imgnum;
    fn_opencv.append("_opencv.jpg");
    string fn_custom = imgnum;
    fn_custom.append("_custom.jpg");

    short int custom_x;
    short int new_value, count_diff=0, count=0;

    Mat src1 = imread(samples::findFile(filename), IMREAD_GRAYSCALE);

    GaussianBlur(src1, gaussian, Size(3, 3), 0, 0);
    src1.copyTo(dst1, src1);
    src1.copyTo(dst2, src1);
    int width = src1.cols, height = src1.rows;

    vector<uint8_t> pixels(width * height);
    vector<int> newpixels(width * height);

    Laplacian(gaussian, dst2, ddepth, ksize, scale, delta, BORDER_ISOLATED);
    convertScaleAbs(dst2, abs_dst2);
    imshow(window_name, abs_dst2);

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
                //custom_x = - (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows + 1) * width + cols + 1] - (int)pixels[(rows + 1) * width + cols] + 8*(int)pixels[rows*width+cols];
                custom_x = - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (rows == 0 && cols == width - 1)
            {
                //custom_x = -(int)pixels[(rows + 1) * width + cols - 1] - (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (rows == 0)
            {
                //custom_x = - (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1] - (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows + 1) * width + cols + 1] - (int)pixels[(rows + 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (rows == height - 1 && cols == 0)
            {
                //custom_x = - (int)pixels[(rows - 1) * width + cols + 1] - (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows - 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows - 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (rows == height - 1 && cols == width - 1)
            {
                //custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows - 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[(rows - 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (rows == height - 1)
            {
                //custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows - 1) * width + cols + 1] - (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows - 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows - 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (cols == 0)
            {
                //custom_x = (int)pixels[(rows - 1) * width + cols + 1] + 2 * (int)pixels[(rows)*width + cols + 1] + (int)pixels[(rows + 1) * width + cols + 1] - (int)pixels[(rows - 1) * width + cols] + (int)pixels[(rows + 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else if (cols == width - 1)
            {
                //custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - 2 * (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1] - (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows + 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            else
            {
                //custom_x = -(int)pixels[(rows - 1) * width + cols - 1] - (int)pixels[(rows)*width + cols - 1] - (int)pixels[(rows + 1) * width + cols - 1] - (int)pixels[(rows - 1) * width + cols + 1] - (int)pixels[(rows)*width + cols + 1] - (int)pixels[(rows + 1) * width + cols + 1] - (int)pixels[(rows + 1) * width + cols] - (int)pixels[(rows - 1) * width + cols] + 8 * (int)pixels[rows * width + cols];
                custom_x = -(int)pixels[rows * width + cols - 1] - (int)pixels[rows * width + cols + 1] - (int)pixels[(rows - 1) * width + cols] - (int)pixels[(rows + 1) * width + cols] + 4 * (int)pixels[rows * width + cols];
            }

            if (abs(custom_x) > 255) custom_x = 255;

            new_value = abs(custom_x); //abs(custom_x) * 0.5 + abs(custom_y) * 0.5;

            newpixels[rows * width + cols] = new_value;

            dst1.at<uint8_t>(rows, cols) = new_value;

           /*if (new_value != abs_dst2.at<uint8_t>(rows, cols))
            {
                printf("x pozicija: %d y pozicija: %d\n", rows, cols);
                printf("OpenCV: %d\n", abs_dst2.at<uint8_t>(rows, cols));
                printf("Moj: %d\n", new_value);
                cout << endl;
                count_diff++;
            }
            count++;*/
        }
    }

    //printf("%d/%d razlicitih\n", count_diff, count);
    imshow(window_name_2, dst1);
    waitKey();

    return 0;
}
