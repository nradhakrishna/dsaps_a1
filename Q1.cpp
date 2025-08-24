#include <opencv2/opencv.hpp>
#include <iostream>
#include<math.h>
using namespace cv;
using namespace std;

void calculate_energy(const Mat& image, Mat &energy, int rows, int cols){ 
    
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            Vec3b up=image.at<Vec3b>(max(i-1,0),j);
            Vec3b down=image.at<Vec3b>(min(i+1,rows-1),j);
            Vec3b left=image.at<Vec3b>(i,max(j-1,0));
            Vec3b right=image.at<Vec3b>(i,min(j+1,cols-1));
            double dx=pow((right[0]-left[0]),2)+pow((right[1]-left[1]),2)+pow((right[2]-left[2]),2);
            double dy=pow((down[0]-up[0]),2)+pow((down[1]-up[1]),2)+pow((down[2]-up[2]),2);
            energy.at<double>(i,j)=sqrt(dx+dy);
        }

    }
    return;
    
}
int main() {
    Mat image = imread("sample2.jpeg");
    if (image.empty()) {
        cout << "Could not read the image" << endl;
        return 1;
    }
    int rows=image.rows;
    int cols=image.cols;
    int target_rows, target_cols;
    printf("Original image size: %d x %d\n", rows, cols);
    printf("Enter target rows and cols: ");
    scanf("%d %d", &target_rows, &target_cols);
    printf("Resizing to: %d x %d\n", target_rows, target_cols);
    int vertical_seams=cols-target_cols;
    int horizontal_seams=rows-target_rows;
    for(int i=0;i<vertical_seams;i++){
        printf("Removing vertical seam %d/%d\r", i+1, vertical_seams);
        Mat energy(rows, cols, CV_64F);
        calculate_energy(image, energy, rows, cols);
        double dp[cols];
        for(int j=0;j<cols;j++){
            dp[j]=1e308;
        }
        for(int j=0;j<rows-1;j++){
            int t_i=0;
            double val=0;
            int x=0;
            while(x<cols){
                x++;
                val+=energy.at<double>(j,t_i);
                int next_i;
                double next_val=1e308;
                if(next_val>energy.at<double>(j+1,t_i)){
                    next_val=energy.at<double>(j+1,t_i);
                    next_i=t_i;
                }
                if(next_val>energy.at<double>(j+1,min(t_i+1,cols-1))){
                    next_val=energy.at<double>(j+1,min(t_i+1,cols-1));
                    next_i=min(t_i+1,cols-1);
                }
                if(next_val>energy.at<double>(j+1,max(t_i-1,0))){
                    next_val=energy.at<double>(j+1,max(t_i-1,0));
                    next_i=max(t_i-1,0);
                }
                t_i=next_i;
                val+=next_val;
            }
            dp[t_i]=min(dp[t_i],val);
        }
        //backtracking
        int path[rows];
        double min_val=1e308;
        int min_index=-1;
        for(int j=0;j<cols;j++){
            if(min_val>dp[j]){
                min_val=dp[j];
                min_index=j; 
            }
        }
        path[rows-1]=min_index;
        for(int j=rows-2;j>=0;j--){
            int t_i=path[j+1];
            double next_val=1e308;
            int next_i;
            if(next_val>energy.at<double>(j,t_i)){
                next_val=energy.at<double>(j,t_i);
                next_i=t_i;
            }
            if(next_val>energy.at<double>(j,min(t_i+1,cols-1))){
                next_val=energy.at<double>(j,min(t_i+1,cols-1));
                next_i=min(t_i+1,cols-1);
            }
            if(next_val>energy.at<double>(j,max(t_i-1,0))){
                next_val=energy.at<double>(j,max(t_i-1,0));
                next_i=max(t_i-1,0);
            }
            path[j]=next_i;
        }
        //remove seam
        Mat new_image(rows, cols-1, CV_8UC3);
        for(int j=0;j<rows;j++){
            int idx=0;
            for(int k=0;k<cols;k++){
                if(k!=path[j]){
                    new_image.at<Vec3b>(j,idx++)=image.at<Vec3b>(j,k);
                }
                else{
                    continue;
                }
            }
        }
        image=new_image;
        cols--;
    }
    
    imwrite("output1.png", image);
    return 0;
}
