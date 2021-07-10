#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

void recogniseStickersByThreshold(Mat image,vector<Rect> &stickers) {
    Mat image_hsv;
    vector< vector<Point> > contours;
    cvtColor(image, image_hsv, COLOR_BGR2HSV ); // Преобразуем в hsv
    Mat tmp_img(image.size(),CV_8U);
    Scalar colors[4] = { Scalar(12,50,220),Scalar(25,0,100), Scalar(16,255,255), Scalar(60,80,255) };

    for (int i = 0; i < 2; i++)
    {
        inRange(image_hsv,colors[i], colors[i+2], tmp_img);
        // "Замазать" огрехи в при выделении по цвету
        dilate(tmp_img,tmp_img,Mat(),Point(-1,-1),3);
        erode(tmp_img,tmp_img,Mat(),Point(-1,-1),1);
        //Выделение непрерывных областей
        findContours(tmp_img,contours,RETR_EXTERNAL, CHAIN_APPROX_NONE);
                if (contours.size()==1) {
        Rect rect=boundingRect(contours[0]);
        stickers.push_back(rect);
        }
    }
}



int main(int argc, char* argv[]) {
 //       vector<Mat> stickers;
   
    VideoCapture cap("Sample.MOV"); // open the video file for reading
    if ( !cap.isOpened() ) return -1;
    double fps = cap.get(CAP_PROP_FPS); //get the frames per seconds of the video
    cout << "Frame per seconds : " << fps << endl;
    namedWindow("MyVideo",WINDOW_AUTOSIZE); //create a window called "MyVideo"
    while(1) {
        Mat frame;
          vector<Rect> rect;
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess) {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }
        else 
        recogniseStickersByThreshold(frame, rect);

        for (int i = 0; i < rect.size(); i++){
            rectangle(frame, rect[i], Scalar(100,255,0), 3);
        }
        resize(frame, frame, Size (1000, 700));

        imshow("MyVideo", frame);
        if(waitKey(30) == 27) {
            break;
        }
    }
}