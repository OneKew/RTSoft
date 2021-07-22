#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "algorithm"


using namespace cv;
using namespace std;

void select_roi(Mat &frame, Mat &res){
  int rows = frame.rows;
  int cols = frame.cols;
  Point points[1][4];
  points[0][0] = Point(cols*0, rows*0.85);
  points[0][1] = Point(cols*0.277, rows*0.48);
  points[0][2] = Point(cols*0.465, rows*0.48);
  points[0][3] = Point(cols*0.72, rows*0.85);
  Mat mask = Mat::zeros(frame.size(), CV_8UC1);
  const Point* ppt[1] = {points[0]};
  int npt[] = {4};
  fillPoly(mask, ppt, npt, 1, Scalar(255,0,0), 8);
  frame.copyTo(res, mask);
} 

Point ptleft1last, ptleft2last, ptright1last, ptright2last;
int t = 0;
int lxmax = 0, lymax = 1000, rxmax = 1000, rymax = 1000;

void RecognizeLines(Mat frame){ 
    if (t % 30 == 0){
      lxmax = 0;
      lymax = 1000;
      rxmax = 1000;
      rymax = 1000;
    }

  resize(frame, frame, Size(1000, 800));
    Mat edges;
    Mat tmp(frame.size(),CV_8U);
    Scalar colors[2] = {Scalar(210,210,210), Scalar(255,255,255)}; 
    select_roi(frame, edges);
    cvtColor(edges, edges, COLOR_BGR2GRAY);
    inRange(edges, colors[0], colors[1], tmp);
    dilate(tmp,tmp,Mat(),Point(-1,-1),3); 
    erode(tmp,tmp,Mat(),Point(-1,-1),3);
    GaussianBlur(tmp, tmp, Size(5,5),0);
    Canny(tmp, tmp, 50, 150);
    
    vector<Vec4i> lines;
    HoughLinesP(tmp, lines, 2, CV_PI/180, 0, 0, 100);
    int left_lines [lines.size()][4] = {}, right_lines [lines.size()][4] = {};

    for( size_t i = 0; i < lines.size(); i++ ) {
     
      Vec4i l = lines[i];
      if ((l[0] <= 390) && (l[2] <= 390)){
        left_lines[i][0] = l[0];
        left_lines[i][1] = l[1];
        left_lines[i][2] = l[2];
        left_lines[i][3] = l[3];
      }
      if ((l[0] > 390) && (l[2] > 390)) {
        right_lines[i][0] = l[0];
        right_lines[i][1] = l[1];
        right_lines[i][2] = l[2];
        right_lines[i][3] = l[3];
      }
    }


    int l2 = 0, l3 = 1000;
    int r2 = 1000, r3 = 1000;

    for (size_t i = 0; i < lines.size(); i++){
      if (left_lines[i][0] != 0) {
        if (left_lines[i][2] > l2) l2 = left_lines[i][2];
        if (left_lines[i][3] < l3) l3 = left_lines[i][3];
      }
      if (l2 > lxmax) lxmax = l2;
      if (l3 < lymax) lymax = l3;
      if (right_lines[i][0] != 0){
        if (right_lines[i][2] < r2) r2 = right_lines[i][2];
        if (right_lines[i][3] < r3) r3 = right_lines[i][3];
      }
      if ((r2 < rxmax)) rxmax = r2;
      if ((r3 < rymax)) rymax = r3;
    }

    Point ptleft2, ptright2;
    if ((l2 != 0) && (l3 != 1000)) {
    ptleft2.x = int(l2);
    ptleft2.y = int(l3);
    }
    else {
      ptleft2.x = 10;
      ptleft2.y = 770;
    }
    if ((r2 != 1000) && (r3 != 1000)){
      ptright2.x = int(r2);
      ptright2.y = int(r3);
    }
    else
    {
      ptright2.x = 660;
      ptright2.y = 655;
    }

    t++;
    
    if ((l2 != 0) && (l3 != 1000)){
      if ((r2 == 1000) && (r3 == 1000)) 
      line(frame, Point(10, 770), ((ptleft2 + Point(lxmax, lymax)) / 2), Scalar(0,0,255), 8, LINE_AA);
      else
      if ((((ptright2 + Point(rxmax, rymax)) / 2) - ((ptleft2 + Point(lxmax, lymax)) / 2)).x <= 40)
      line(frame, Point(10, 770), ((ptleft2 + Point(lxmax, lymax)) / 2), Scalar(0,0,255), 8, LINE_AA);
      else
      line(frame, Point(10, 770), ((ptleft2 + Point(lxmax, lymax)) / 2), Scalar(0,255,0), 8, LINE_AA);

      
    }

    if ((r2 != 1000) && (r3 != 1000)){
      if ((l2 == 0) && (l3 == 1000)) 
      line(frame, Point(660, 655), ((ptright2 + Point(rxmax, rymax)) / 2), Scalar(0,0,255), 8, LINE_AA);
      else
      if ((((ptright2 + Point(rxmax, rymax)) / 2) - ((ptleft2 + Point(lxmax, lymax)) / 2)).x <= 40)
      line(frame, Point(660, 655), ((ptright2 + Point(rxmax, rymax)) / 2), Scalar(0,0,255), 8, LINE_AA);
      else
      line(frame, Point(660, 655), ((ptright2 + Point(rxmax, rymax)) / 2), Scalar(0,255,0), 8, LINE_AA);


    }


    if ((l2 == 0) && (l3 == 1000)){
      putText(frame, "Route error!!!", Point(300, frame.rows / 4), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255), 4);
    }

     if ((r2 == 1000) && (r3 == 1000)){
      putText(frame, "Route error!!!", Point(300, frame.rows / 4), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255), 4);
    }

    if ((((ptright2 + Point(rxmax, rymax)) / 2) - ((ptleft2 + Point(lxmax, lymax)) / 2)).x <= 40) {
      putText(frame, "Route error!!!", Point(300, frame.rows / 4), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 255), 4);
    }




    imshow("Tracking Window", frame);
   // imshow("Tracking Window2", tmp);

  
}






int main(int argc, char* argv[]) {

    VideoCapture cap("DRIVING.MP4");  

    if ( !cap.isOpened() ) return -1;
    double fps = cap.get(CAP_PROP_FPS); 
    cout << "Frame per seconds : " << fps << endl;
    namedWindow("Tracking Window",WINDOW_AUTOSIZE); 
     while(1) {
        Mat frame;
        bool bSuccess = cap.read(frame); 
        if (!bSuccess) {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }
        else 
        RecognizeLines(frame);

    //    resize(frame, frame, Size (1000, 800));

     //   imshow("Tracking Window", frame);
        if(waitKey(2) == 1) {
            break;
        }
    }
}
