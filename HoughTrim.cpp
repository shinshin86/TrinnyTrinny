#include<iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


class cut_point{
public:
    int x1;
    int y1;
    int x2;
    int y2;
    cut_point(){
        x1 = 0;
        y1 = 0;
        x2 = 0;
        y2 = 0;
    }
    
    void set_point(int arg_x1, int arg_y1, int arg_x2, int arg_y2){
        x1 = arg_x1;
        y1 = arg_y1;
        x2 = arg_x2;
        y2 = arg_y2;
        
    }
    
    void get_point(){
        printf("%s","GET POINTS \n");
        printf("%s%d%s","x1 : ",x1,"\n");
        printf("%s%d%s","y1 : ",y1,"\n");
        printf("%s%d%s","x2 : ",x2,"\n");
        printf("%s%d%s","y2 : ",y2,"\n");
    }
};

int main(int argc,char *argv[])
{
    int i;
    printf("Argument count =  %d\n", argc);
    if(argc != 2){
        printf("%s","Please in an one argument.\nProcessing is a quit...");
        exit(1);
    }
    for (i = 0; i < argc; i++) {
        printf("%d argument = %s\n", i, argv[i]);
    }
    // Target image a argument of first.
    cv::Mat src_img = cv::imread(argv[1], 1);
    if(!src_img.data) return -1;

    cv::Mat dst_img, work_img;
    dst_img = src_img.clone();
    cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
    cv::Canny(work_img, work_img, 50, 200, 3);
    int count;

    // use to trimming.
    cut_point c_points1;
    cut_point c_points2;
    
    
    // Hough transform
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(work_img, lines, 1, CV_PI/180, 300, 300, 1);
    
    std::vector<cv::Vec4i>::iterator it = lines.begin();
    for(; it!=lines.end(); ++it) {
        cv::Vec4i l = *it;
        
        // DEBUG - Line color is RED,
        // cv::line(dst_img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 2, CV_AA);

        
        // cut poinst1
        if(l[1] == l[3]){
            if((l[0]>=100 && l[0]<= 600) && l[2] >= (l[0]+700)){
                if((l[1] >= 300)&&(l[1] <= 1000)){
                    // printf("%s%d%s","DEBUG!!!!! : ",l[0],"\n");
                    c_points1.set_point(l[0],l[1],l[2],l[3]);
                }
            }
        }
        // cut poinst2
        if(l[1] == l[3]){
            if((l[0]>=100 && l[0]<= 600) && l[2] >= (l[0]+700)){
                if((l[1] >= 900)&&(l[1] <= 2500)){
                    // printf("%s%d%s","DEBUG!!!!! : ",l[0],"\n");
                    c_points2.set_point(l[0],l[1],l[2],l[3]);
                }
            }
        }
        count++;
    }
    
    if((c_points1.x1 != 0)&&(c_points1.y1!=0)&&(c_points1.x2 != 0)&&(c_points1.y2!=0)){
        printf("%s","PROCESSING...\n");
        cv::Mat cut_img(dst_img,cv::Rect(c_points1.x1,c_points1.y1,(c_points1.x2 - c_points1.x1),(c_points2.x2 - c_points2.x1)));
        imwrite(argv[1],cut_img);
    }
    // debug print
    c_points1.get_point();
    c_points2.get_point();
}
