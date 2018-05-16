
/* 
	Programa para fazer arte usando 
	algoritmo de Canny junto com pontilhismo


*/


#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 4

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points,cinza,borrar,bordas;
  int fator =60;

  int width, height;
  int x, y;
  image = imread(argv[1]);
  Mat  arte(image.rows,image.cols,image.type());
  Vec3b cor;

  srand(time(0));
  
  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);
  
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  //points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());
  
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      cor = image.at<Vec3b>(x,y);
      circle(arte,
             cv::Point(y,x),
             RAIO,
             CV_RGB(cor[2],cor[1],cor[0]),
             -1,
             CV_AA);
    }
  }

  cvtColor(arte, cinza, CV_BGR2GRAY);
  GaussianBlur(cinza, borrar, Size(5,5), 25, 25);
  Canny(borrar, bordas,fator,3*fator);

  for(int i=0;i<arte.rows;i++){
    for(int j=0;j<arte.cols;j++){
        if(bordas.at<uchar>(i,j)==255){
          cor = arte.at<Vec3b>(i,j);
          circle(arte,
                 cv::Point(j,i),
                 1,
                 CV_RGB(cor[2],cor[1],cor[0]),
                 -1,
                 CV_AA);
          }
        }

    }

  


  //imwrite("pontos.jpg", points);
 // imshow("pontos", points);
  imshow("pontos",arte);
  waitKey();
  return 0;
    
}
