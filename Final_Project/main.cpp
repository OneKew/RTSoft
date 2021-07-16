#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
int gmin = 1, gmax = 1;


void RecognizeLines(Mat frame){
    Scalar colors[2] = {Scalar(230,230,230), Scalar(255,255,255)};
    Mat edges, h;
    std::vector<std::vector<Point> > contours; 
    cvtColor(frame, edges, COLOR_BGR2GRAY);  
    cvtColor(edges, edges, COLOR_GRAY2)
   // Rect roi(0, 0, img.cols / 2, img.rows / 2)  надо обрезать изображение до линии горизонта
   
 //   cvtColor(frame, edges, COLOR_BGR2HSV);    я игрался с фильтрами
  //  cvtColor(frame, edges, COLOR_HSV2BGR);
   
    //                 скорее всего нужно BGR2GRAY как-то затемнить для лучшей контрастности
    //                 и затем использовать канни и гауссово размытие и выделять полосы
    //                 или придумай как выделить все белое на фильтре канни и будет четко

     imshow("Tracking Window", edges);



    Mat tmp(frame.size(),CV_8U);


    inRange(frame, colors[0], colors[1], tmp);
    dilate(tmp,tmp,Mat(),Point(-1,-1),3); 
    erode(tmp,tmp,Mat(),Point(-1,-1),1);
    
    GaussianBlur(tmp, tmp, Size( 5, 5 ), 0);
    Canny(tmp,tmp,100,150);
   // findContours(tmp, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

  //  imshow("Tracking Window", tmp);
    
 //   imshow("Tracking Window 2", edges);
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
  //  createTrackbar("Trackbar", "Tracking Window", &gmin, 9);
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