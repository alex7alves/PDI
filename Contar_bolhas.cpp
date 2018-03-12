
/*
	Programa para contar bolhas e os  buracos dentro das bolhas

	Autor : Alex Alves
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


CvPoint ponto(int i, int j){
	CvPoint p;
	p.x=j;
	p.y=i;
	return p;
}

void Bordas( Mat bolha, int passo_linha,int passo_coluna)
{
	CvPoint p;
	for(int i=0; i<bolha.rows; i = i+passo_linha){
    	for(int j=0; j<bolha.cols; j= j+passo_coluna){
    		if(bolha.at<uchar>(i,j)==255){
    			p=ponto(i,j);
    			floodFill(bolha,p,0);
    		}
		}
  	}
}




int main(int argc, char** argv){
  Mat image;

  int nobjects;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  Mat bolha(image.rows,image.cols,image.type());
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  p = ponto(0,0);
  // copiando imagem original
  for(int i=0; i<image.rows; i++){
    for(int j=0; j<image.cols; j++){
    	bolha.at<uchar>(i,j) = image.at<uchar>(i,j);	
	}
  }
  // Retirando bolhas que tocam as bordas nas linhas
  Bordas(bolha,bolha.rows-1,1);
  // Retirando bolhas que tocam as bordas nas colunas
  Bordas(bolha,1,bolha.cols-1);

 
  // busca objetos com buracos presentes
/*  nobjects=0;
  for(int i=0; i<image.rows; i++){
    for(int j=0; j<image.cols; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects);
	  }
	}
  }*/
  imshow("image", image);
  imshow("Modificado",bolha);
  //imwrite("labeling.png", image);
  waitKey();
  return 0;
}
