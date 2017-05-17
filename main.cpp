#include <stdio.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "boost/filesystem.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace bfs = boost::filesystem;

void rectanglifyImage(cv::Mat& image, cv::Mat& templ, int r, int g, int b, std::vector<int>& locations, std::vector<int>& values);

int main(int argc, char** argv )
{
    //std::cout << argv[1] << " " << argv[2] << std::endl;
    bfs::path doc(argv[1]);
    bfs::path templates_dir(argv[2]);
    std::vector<std::string> templates;
    std::vector<int> locations;
    std::vector<int> values;

    //std::cout << "initialized vectors" << std::endl;

    bfs::directory_iterator end_itr; // default construction yields past-the-end
    for ( bfs::directory_iterator itr( templates_dir );
            itr != end_itr;
            ++itr )
    {
        std::string name = itr->path().string();
        //std::cout << name << std::endl;
        templates.push_back(name);
    }

    cv::Mat image;
    //typed
    image = cv::imread(doc.string());
    //std::cout << "Read base image" << std::endl;

    std::vector<cv::Mat> templ_imgs;
    for(int i = 0; i < templates.size(); i++) {
        templ_imgs.push_back(cv::imread(templates[i]));
    }
    //std::cout << "Read template images" << std::endl;


    for(int i = 0; i < templ_imgs.size(); i++) {
        int r = 50 * (i % 3);
        int g = 50 * (i % 2);
        int b = 50 * (i % 4);
        rectanglifyImage(image, templ_imgs.at(i), r, g, b, locations, values);
    }
    
    json j;
    for(int i = 0; i < locations.size(); i+=2) {
        json temp;
        temp["x"] = locations[i];
        temp["y"] = locations[i+1];
        temp["value"] = values[i/2];
        j.push_back(temp);
    }
    //j["values"] = values;

    std::cout << j << std::endl;

    //cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    //cv::imshow("Display Image", image);

    //cv::waitKey(0);

    return 0;
}

void rectanglifyImage(cv::Mat& image, cv::Mat& templ, int r, int g, int b, std::vector<int>& locations, std::vector<int>& values){
    cv::Mat result, norm_res, image_gr, templ_gr;

    int result_cols =  image_gr.cols - templ_gr.cols + 1;
    int result_rows = image_gr.rows - templ_gr.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );
    norm_res.create( result_rows, result_cols, CV_32FC1 );
    cv::cvtColor(image, image_gr, CV_BGR2GRAY);
    cv::cvtColor(templ, templ_gr, CV_BGR2GRAY);
    for(int i = 0; i < 2; i++) {
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
        locations.push_back(matchLoc.x);
        locations.push_back(matchLoc.y);
        values.push_back(minVal);
        cv::rectangle( image, matchLoc, cv::Point( matchLoc.x + templ_gr.cols , matchLoc.y + templ_gr.rows ), cv::Scalar(r,g,b), 1, 1, 0 );

        //std::cout << matchLoc.x << " " << matchLoc.y << std::endl;
        //std::cout << (minVal) << std::endl;
        //// std::cout << (image_gr.cols) << " " << (image_gr.rows) << std::endl;
        //// std::cout << result.at<float>(matchLoc.x, matchLoc.y) << std::endl;
        //// std::cout << (templ_gr.cols) << " " << (templ_gr.rows) << std::endl;
        for (int xx = matchLoc.x; xx < templ_gr.cols + matchLoc.x; xx++) {
            for(int yy = matchLoc.y; yy < templ_gr.rows + matchLoc.y; yy++) {
                image_gr.at<uchar>(yy,xx) = 255;
            }
        }
    }
    
}
