#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void select_roi(Mat &frame, Mat &res){
  int rows = frame.rows;
  int cols = frame.cols;

  Point points[1][4];
  points[0][0] = Point(cols*0.05, rows);
  points[0][1] = Point(cols*0.1, rows*0.4);
  points[0][2] = Point(cols*0.6, rows*0.4);
  points[0][3] = Point(cols*0.95, rows);
  Mat mask = Mat::zeros(frame.size(), CV_8UC1);
  const Point* ppt[1] = {points[0]};
  int npt[] = {4};
  fillPoly(mask, ppt, npt, 1, Scalar(255,0,0), 8);
  frame.copyTo(res, mask);
  
} 

void RecognizeLines(Mat frame){
  //  Rect rect(0, 0, frame.cols / 2, frame.rows / 2);  
  resize(frame, frame, Size(1000, 800));
 // int startX=0,startY=400,width=700,height=300;
 //  Mat roi(frame, Rect(startX,startY,width,height));

    Mat edges;
    Mat res;
    Scalar colors[2] = {Scalar(210,210,210), Scalar(255,255,255)}; 
    select_roi(frame, edges);
    cvtColor(edges, edges, COLOR_BGR2GRAY);
    Mat tmp(frame.size(),CV_8U);
    inRange(edges, colors[0], colors[1], tmp);
    dilate(tmp,tmp,Mat(),Point(-1,-1),2); 
    erode(tmp,tmp,Mat(),Point(-1,-1),2);
    GaussianBlur(edges, tmp, Size(9,9),0);
    Canny(tmp, tmp, 50, 150);
    
    vector<Vec2f> lines;
    HoughLines(tmp, lines, 3, CV_PI/180, 150, 0, 0 );
    for( size_t i = 0; i < lines.size(); i++ ) {
      float rho = lines[i][0], theta = lines[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      line( frame, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    



    imshow("Tracking Window", edges);
  //  imshow("Tracking Window 2", frame);

  
}






int main(int argc, char* argv[]) {

    VideoCapture cap("DRIVING.MP4"); 
  //  VideoCapture cap("TEST.MOV"); 
    if ( !cap.isOpened() ) return -1;
    double fps = cap.get(CAP_PROP_FPS); 
    cout << "Frame per seconds : " << fps << endl;
    namedWindow("Tracking Window",WINDOW_AUTOSIZE); 
  //  createTrackbar("Trackbar1", "Tracking Window", &gmin, 1000);
  //  createTrackbar("Trackbar2", "Tracking Window", &gmax, 1000);
     while(1) {
        Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess) {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }
        else 
        RecognizeLines(frame);

        resize(frame, frame, Size (1000, 800));

      //  imshow("Tracking Window", frame);
        if(waitKey(2) == 1) {
            break;
        }
    }
}