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

int* find_Vertical_Seam(const Mat& energy) {
    int rows=energy.rows;
    int cols=energy.cols;
    Mat dp(rows,cols,CV_64F);
    Mat backtrack(rows,cols,CV_32S);
    for (int j=0;j<cols;j++) {
        dp.at<double>(0,j)=energy.at<double>(0,j);
    }
    for (int i=1;i<rows;i++) {
        for (int j=0;j<cols;j++) {
            double left=(j>0)?dp.at<double>(i-1,j-1):1e308;
            double up=dp.at<double>(i-1,j);
            double right=(j<cols-1)?dp.at<double>(i-1,j+1):1e308;
            double minPrev=left;
            int idx=j-1;
            if (up<minPrev){
                minPrev=up; 
                idx=j; 
            }
            if(right<minPrev){ 
                minPrev=right; 
                idx=j+1; 
            }
            dp.at<double>(i,j)=energy.at<double>(i,j)+minPrev;
            backtrack.at<int>(i,j)=idx;
        }
    }
    double minEnergy=1e308;
    int minCol=0;
    for (int j=0;j<cols;j++) {
        if (dp.at<double>(rows-1,j)<minEnergy){
            minEnergy=dp.at<double>(rows-1,j);
            minCol=j;
        }
    }
    int* seam=new int[rows];
    seam[rows-1]=minCol;
    for (int i=rows-1;i>0;i--) {
        seam[i-1]=backtrack.at<int>(i,seam[i]);
    }
    return seam; 
}

int* find_Horizontal_Seam(const Mat& energy){
    int rows=energy.rows;
    int cols=energy.cols;
    Mat dp(rows, cols, CV_64F);
    Mat backtrack(rows, cols, CV_32S);
    for(int i=0;i<rows;i++){
        dp.at<double>(i,0)=energy.at<double>(i,0);
    }
    for(int j=1;j<cols;j++){
        for(int i=0;i<rows;i++){
            double up=(i>0)?dp.at<double>(i-1,j-1):1e308;
            double left=dp.at<double>(i,j-1);
            double down=(i<rows-1)?dp.at<double>(i+1,j-1):1e308;
            double minPrev=up;
            int idx=i-1;
            if(left<minPrev){
                minPrev=left;
                idx=i;
            }
            if(down<minPrev){
                minPrev=down;
                idx=i+1;
            }
            dp.at<double>(i,j)=energy.at<double>(i,j)+minPrev;
            backtrack.at<int>(i,j)=idx;
        }
    }
    double minEnergy=1e308;
    int minRow=0;
    for(int i=0;i<rows;i++){
        if(dp.at<double>(i,cols-1)<minEnergy){
            minEnergy=dp.at<double>(i,cols-1);
            minRow=i;   
        }
    }
    int* seam = new int[cols];
    seam[cols-1]=minRow;
    for(int j=cols-1;j>0;j--){
        seam[j-1]=backtrack.at<int>(seam[j],j);
    }
    return seam;

}

Mat remove_Horizontal_Seam(const Mat& image, int* seam){
    int rows=image.rows;
    int cols=image.cols;
    Mat new_image(rows-1,cols,CV_8UC3);
    for(int j=0;j<cols;j++){
        int k=0;
        for(int i=0;i<rows;i++){
            if(i==seam[j]) continue;
            new_image.at<Vec3b>(k,j)=image.at<Vec3b>(i,j);
            k++;
        }
    }
    return new_image;
}

void make_vertical_seam_visible(Mat &image, int* seam){
    int rows=image.rows;
    for(int i=0;i<rows;i++){
        image.at<Vec3b>(i,seam[i])=Vec3b(0, 0,0);
    }
    imshow("Seam", image);
    return;
}

void make_horizontal_seam_visible(Mat &image, int* seam){
    int cols=image.cols;
    for(int j=0;j<cols;j++){
        image.at<Vec3b>(seam[j], j)=Vec3b(0,0, 0);
    }
    imshow("Seam",image);
    return;
}
Mat remove_Vertical_Seam(const Mat& image, const int* seam) {
    int rows=image.rows;
    int cols=image.cols;

    Mat new_image(rows,cols - 1,CV_8UC3);

    for (int i=0;i<rows;i++) {
        int k=0; 
        for (int j=0;j<cols;j++) {
            if (j==seam[i]){
            continue;
            }
            new_image.at<Vec3b>(i,k)=image.at<Vec3b>(i,j);
            k++;
        }
    }

    return new_image;
}

int main() {
    char file_path[1000];
    printf("Enter image file path: ");
    scanf("%[^\n]", file_path);
    Mat image = imread(file_path);
    if (image.empty()) {
        printf("Could not read the image\n");
        return 1;
    }
    int rows=image.rows;
    int cols=image.cols;
    int target_rows, target_cols;
    printf("Original image size: %d x %d\n", rows, cols);
    printf("Enter target rows and cols: ");
    scanf("%d %d", &target_rows, &target_cols);
    if(target_rows<=0 || target_rows>rows || target_cols<=0 || target_cols>cols){
        printf("Invalid target dimensions\n");
        return 1;
    }
    printf("Resizing to: %d x %d\n", target_rows, target_cols);                                                                                                                                                                                                 
    int vertical_seams=cols-target_cols;
    int horizontal_seams=rows-target_rows;
    for(int i=0;i<vertical_seams;i++){
        printf("Removing vertical seam %d/%d\r", i+1, vertical_seams);
        fflush(stdout); 
        Mat energy(rows, cols, CV_64F);
        calculate_energy(image, energy, rows, cols);
        int* seam = find_Vertical_Seam(energy);
        make_vertical_seam_visible(image, seam);
        waitKey(1);
        Mat new_image = remove_Vertical_Seam(image, seam);
        image=new_image;
        delete[] seam;
        cols--;
    }
    for(int i=0;i<horizontal_seams;i++){
        printf("Removing horizontal seam %d/%d\r", i+1, horizontal_seams);
        fflush(stdout); 
        Mat energy(rows, cols, CV_64F);
        calculate_energy(image, energy, rows, cols);
        int* seam=find_Horizontal_Seam(energy);
        make_horizontal_seam_visible(image, seam);
        waitKey(1);
        Mat new_image=remove_Horizontal_Seam(image, seam);
        image=new_image;
        delete[] seam;
        rows--;
    }

    imwrite("output1.png", image);
    return 0;
}
