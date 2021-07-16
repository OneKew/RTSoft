#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int gmin = 0, gmax = 0;

void RecognizeLines(Mat frame){
    Mat edges, h;
    vector<vector<Point> > contours; 
    cvtColor(frame, frame, COLOR_BGR2GRAY); 
//    cvtColor(frame, frame, COLOR_BGR2BGRA); 
//    GaussianBlur(frame,edges, Size(5, 5), 0);
 //   Canny(edges, edges, 50, 150);

   // Rect roi(0, 0, img.cols / 2, img.rows / 2)  надо обрезать изображение до линии горизонта
   
 //   cvtColor(frame, edges, COLOR_BGR2HSV);    я игрался с фильтрами
 //   cvtColor(frame, edges, COLOR_HSV2BGR);
   
    //                 скорее всего нужно BGR2GRAY как-то затемнить для лучшей контрастности
    //                 и затем использовать канни и гауссово размытие и выделять полосы
    //                 или придумай как выделить все белое на фильтре канни и будет четко

   Mat tmp(frame.size(),CV_8U);

   
    Scalar colors[2] = {Scalar(210,210,210), Scalar(255,255,255)};
    inRange(frame, colors[0], colors[1], tmp);
    dilate(tmp,tmp,Mat(),Point(-1,-1),2); 
    erode(tmp,tmp,Mat(),Point(-1,-1),2);
    GaussianBlur(tmp, tmp, Size( 1, 11 ), 0);
    Canny(tmp,tmp,200,450);

     //    imshow("Tracking Window", edges);
   // findContours(tmp, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

   // imshow("Tracking Window", tmp);
    
    imshow("Tracking Window 2", edges);
    if (contours.size() == 1)
        {
            Rect br = boundingRect(contours[0]);           
            rectangle(frame,br,Scalar(0,250,0),2);
            drawContours(frame,contours,-1, (0,0,255), 3, LINE_AA);
        }   
}






int main(int argc, char* argv[]) {

  //  VideoCapture cap("DRIVING.MP4"); 
    VideoCapture cap("TEST.MOV"); 
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

        resize(frame, frame, Size (800, 600));

      //  imshow("Tracking Window", frame);
        if(waitKey(30) == 27) {
            break;
        }
    }
}