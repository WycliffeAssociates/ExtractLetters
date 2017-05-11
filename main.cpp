#include <stdio.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "boost/filesystem.hpp"

const int em2 = 1024;

int main(int argc, char** argv )
{
    // if ( argc != 2 )
    // {
    //     printf("usage: DisplayImage.out <Image_Path>\n");
    //     return -1;
    // }

    cv::Mat image, image_gr;
    cv::Mat result, norm_res;
    cv::Mat templ, templ_gr;
    image = cv::imread( "Hello.png");
    templ = cv::imread("template.png");
    cv::cvtColor(image, image_gr, CV_BGR2GRAY);
    cv::cvtColor(templ, templ_gr, CV_BGR2GRAY);

    int result_cols =  image_gr.cols - templ_gr.cols + 1;
    int result_rows = image_gr.rows - templ_gr.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );
    norm_res.create( result_rows, result_cols, CV_32FC1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    // cv::matchTemplate(image_gr, templ_gr, result, CV_TM_SQDIFF );
    // cv::normalize( result, norm_res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );

    int x_off = image_gr.cols - result.cols;
    int y_off = image_gr.rows - result.rows;

    for(int i = 0; i < 4; i++) {
        cv::matchTemplate(image_gr, templ_gr, result, CV_TM_SQDIFF );
        cv::normalize( result, norm_res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        //cv::normalize( result, norm_res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        double minVal; double maxVal; 
        cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;

        cv::minMaxLoc( norm_res, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        matchLoc = minLoc; 

        cv::rectangle( image, matchLoc, cv::Point( matchLoc.x + templ_gr.cols , matchLoc.y + templ_gr.rows ), cv::Scalar::all(0), 1, 1, 0 );

        std::cout << matchLoc.x << " " << matchLoc.y << std::endl;
        std::cout << (image_gr.cols) << " " << (image_gr.rows) << std::endl;
        std::cout << result.at<float>(matchLoc.x, matchLoc.y) << std::endl;
        std::cout << (templ_gr.cols) << " " << (templ_gr.rows) << std::endl;
        for (int xx = matchLoc.x; xx < templ_gr.cols + matchLoc.x; xx++) {
            for(int yy = matchLoc.y; yy < templ_gr.rows + matchLoc.y; yy++) {
                image_gr.at<uchar>(yy,xx) = 255;
            }
            std::cout << xx << std::endl;
        }

         //       image_gr.at<float>(64,128, 0) = 0;

    }
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", image);

    // cv::namedWindow("Template Image", cv::WINDOW_AUTOSIZE );
    // cv::imshow("Template Image", templ_gr);

    // cv::namedWindow("Template Image", cv::WINDOW_AUTOSIZE );
    // cv::imshow("Template Image", result);

    cv::waitKey(0);

    return 0;
}
