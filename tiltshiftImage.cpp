
/* 
  Programa para aplicar o efeito de tiltshift
  em uma imagem

  Autor : Alex Alves

*/


#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void Alocar(int **x, int **x2);
void setValores(int *borrar,int *borrar_slider, int *foco_inicio_slider,int *foco_fim_slider);
void Borrar(int *b);
void TiltShift(int * x1, int *x2, Mat image,Mat borrado,Mat matriz);
void on_borrar(int, void*);
void on_inicio(int, void*);
void on_fim(int, void*);


int main(int argvc, char** argv){
  int *borrar, *borrar_slider, *foco_inicio_slider,*foco_fim_slider;
  Mat image, borrado;

  image = imread(argv[1]);
  Mat matriz(image.rows,image.cols,image.type());

  Alocar(&borrar,&borrar_slider);
  Alocar(&foco_inicio_slider,&foco_fim_slider);

  setValores(borrar,borrar_slider,foco_inicio_slider,foco_fim_slider);
 
  namedWindow("Tiltshift", 1);

 
  createTrackbar( "Borrar", "Tiltshift",borrar_slider,*borrar,on_borrar );
  createTrackbar( "Inicio ", "Tiltshift",foco_inicio_slider,image.rows-30,on_inicio );
  createTrackbar( "Fim", "Tiltshift",foco_fim_slider,image.rows,on_fim );
  
  while(1){

    on_borrar(*borrar_slider, 0 );
    on_inicio(*foco_inicio_slider, 0 );
    on_fim(*foco_fim_slider, 0 );

    Borrar(borrar_slider);
    blur(image,borrado, Size(*borrar_slider,*borrar_slider),Point(-1,-1));
    if(*foco_fim_slider>=*foco_inicio_slider ){
      TiltShift(foco_inicio_slider ,foco_fim_slider,image,borrado,matriz);
    }else{
      TiltShift(foco_fim_slider,foco_inicio_slider,image,borrado,matriz);
    }
    imshow("Tiltshift",matriz);
    if(waitKey(20)==27){
      imwrite("tiltshift.jpg",matriz);
      break;
    } 
  }
   
  return 0;
}
void Alocar(int **x, int **x2){
  *x = new int;
  *x2 = new int;
}
void setValores(int *borrar,int *borrar_slider, int *foco_inicio_slider,int *foco_fim_slider){
    *borrar =20;
    *borrar_slider=3;
    *foco_inicio_slider=0;
    *foco_fim_slider=200;
}
void Borrar(int *b){

  if(*b<5) *b=3;
  else if(*b>5 && *b<7) *b=5;
  else if(*b>7 && *b<9) *b=7;
  else if(*b>9 && *b<13) *b=9;
  else if(*b>=13 && *b<15) *b=13;
  else if(*b>15) *b=15;
}


void TiltShift(int * x1, int *x2, Mat image,Mat borrado,Mat matriz){

  for(int i=0;i<image.rows;i++){
    for(int j=0;j<image.cols;j++){
      if(i>=*x1 && i<=*x2){
        matriz.at<float>(i,j)= image.at<float>(i,j);

      }else {
        matriz.at<float>(i,j)= borrado.at<float>(i,j);

      }    
    }
  }
}
// Funcoes vazias mas servem para direcionar os ponteiros
void on_borrar(int, void*){
}

void on_inicio(int, void*){
}
void on_fim(int, void*){
}