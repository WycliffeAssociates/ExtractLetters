#include <stdio.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "boost/filesystem.hpp"

const int em2 = 1024;

void rectanglifyImage(cv::Mat& img, cv::Mat& templ, int r, int g, int b);

int main(int argc, char** argv )
{
    // if ( argc != 2 )
    // {
    //     printf("usage: DisplayImage.out <Image_Path>\n");
    //     return -1;
    // }

    cv::Mat image;
    cv::Mat templ, templ_o, templ_e, templ_h;
    
    //typed
    image = cv::imread( "Hello.png");
    templ = cv::imread("template_l.png");
    templ_e = cv::imread("template_e.png");
    templ_o = cv::imread("template_o.png");
    templ_h = cv::imread("template_h.png");


    cv::Mat h_image_s, h_image_l;
    cv::Mat h_templ, h_templ_o, h_templ_e, h_templ_h;
    //hand
    h_image_s = cv::imread( "hello_small.jpg");
    h_image_l = cv::imread( "hello_large.jpg");
    h_templ = cv::imread("h_template_l.jpg");
    h_templ_e = cv::imread("h_template_e.jpg");
    h_templ_o = cv::imread("h_template_o.jpg");
    h_templ_h = cv::imread("h_template_h.jpg");

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    rectanglifyImage(image, templ, 255, 0, 0);
    rectanglifyImage(image, templ_e, 0, 255, 0);
    rectanglifyImage(image, templ_o, 0, 0, 255);
    rectanglifyImage(image, templ_h, 0, 0, 0);

    rectanglifyImage(h_image_s, h_templ, 255, 0, 0);
    rectanglifyImage(h_image_s, h_templ_e, 0, 255, 0);
    rectanglifyImage(h_image_s, h_templ_o, 0, 0, 255);
    rectanglifyImage(h_image_s, h_templ_h, 0, 0, 0);

    rectanglifyImage(h_image_l, h_templ, 255, 0, 0);
    rectanglifyImage(h_image_l, h_templ_e, 0, 255, 0);
    rectanglifyImage(h_image_l, h_templ_o, 0, 0, 255);
    rectanglifyImage(h_image_l, h_templ_h, 0, 0, 0);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", image);

    cv::namedWindow("Handwritten Image S", cv::WINDOW_AUTOSIZE );
    cv::imshow("Handwritten Image S", h_image_s);

    cv::namedWindow("Handwritten Image L", cv::WINDOW_AUTOSIZE );
    cv::imshow("Handwritten Image L", h_image_l);

    // cv::namedWindow("Template Image", cv::WINDOW_AUTOSIZE );
    // cv::imshow("Template Image", templ_gr);

    // cv::namedWindow("Template Image", cv::WINDOW_AUTOSIZE );
    // cv::imshow("Template Image", result);

    cv::waitKey(0);

    return 0;
}

void rectanglifyImage(cv::Mat& image, cv::Mat& templ, int r, int g, int b){
    cv::Mat result, norm_res, image_gr, templ_gr;

    int result_cols =  image_gr.cols - templ_gr.cols + 1;
    int result_rows = image_gr.rows - templ_gr.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );
    norm_res.create( result_rows, result_cols, CV_32FC1 );
    cv::cvtColor(image, image_gr, CV_BGR2GRAY);
    cv::cvtColor(templ, templ_gr, CV_BGR2GRAY);
    for(int i = 0; i < 1; i++) {
        cv::matchTemplate(image_gr, templ_gr, result, CV_TM_SQDIFF );
         cv::normalize( result, norm_res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        // cv::normalize( result, norm_res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
        norm_res = cv::Mat(result);
        double minVal; double maxVal; 
        cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;

        cv::minMaxLoc( norm_res, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        matchLoc = minLoc; 
        if(minVal > 1000000) {
            break;
        }
        cv::rectangle( image, matchLoc, cv::Point( matchLoc.x + templ_gr.cols , matchLoc.y + templ_gr.rows ), cv::Scalar(r,g,b), 1, 1, 0 );

        std::cout << matchLoc.x << " " << matchLoc.y << std::endl;
        std::cout << (minVal) << std::endl;
        // std::cout << (image_gr.cols) << " " << (image_gr.rows) << std::endl;
        // std::cout << result.at<float>(matchLoc.x, matchLoc.y) << std::endl;
        // std::cout << (templ_gr.cols) << " " << (templ_gr.rows) << std::endl;
        for (int xx = matchLoc.x; xx < templ_gr.cols + matchLoc.x; xx++) {
            for(int yy = matchLoc.y; yy < templ_gr.rows + matchLoc.y; yy++) {
                image_gr.at<uchar>(yy,xx) = 255;
            }
        }
    }
    
}
