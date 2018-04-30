#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;



void Alocar(int **x, int **x2){
  *x = new int;
  *x2 = new int;
}
void setValores(int *gl,int *gh, int *c,int *d0){
    *gl=0;
    *gh=0;
    *c=0;
    *d0=0;
}

// Funcoes vazias mas servem para direcionar os ponteiros
void on_gamma_l(int, void*){
}

void on_gamma_h(int, void*){
}
void on_c(int, void*){
}
void on_d0(int, void*){
}

void TrocarQuadrantes(Mat image,Mat matriz){

  int ql =image.rows/2,qc=image.cols/2;
  for(int i=0; i<ql ;i++)
  {
    for(int j=0; j<qc ; j++)
    {
      matriz.at<float>(i,j)=image.at<float>(i+ql,j+qc);
      matriz.at<float>(i+ql,j+qc)=image.at<float>(i,j);
      matriz.at<float>(i,j+qc)=image.at<float>(i+ql,j);
      matriz.at<float>(i+ql,j)=image.at<float>(i,j+qc);
    }
  }

}

void Filtro_Homomorfico(Mat temp, int *gl,int *gh, int *c,int *d0,int dft_M,int  dft_N)
{

  float gl_aux,gh_aux,c_aux,d0_aux;
  gl_aux= *gl/10;
  gh_aux = *gh/10;
  c_aux = *c/10;
  d0_aux = *d0/100;

  for(int i=0; i < temp.rows; i++){
    for(int j=0; j < temp.cols; j++){
      float t = (i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);
      temp.at<float>(i,j) = (gh_aux-gl_aux)*(1.0 - (float)exp(-(c_aux*t/(d0_aux*d0_aux)))) + gl_aux;
    }
  }

}



int main(int argvc, char** argv){
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  int *gl,*gh,*d0,*c;

  image = imread(argv[1]);
  cvtColor(image, imagegray, CV_BGR2GRAY);

  Alocar(&gl,&gh);
  Alocar(&c,&d0);
  setValores(gl,gh,c,d0);

  imshow("original", imagegray);

  // valores ideais dos tamanhos da imagem
  // para calculo da DFT
  int dft_M, dft_N;

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  namedWindow("Filtro", 1);

 
  createTrackbar( "c", "Filtro",c,100,on_c );
  createTrackbar( "d0 ","Filtro",d0,100,on_d0 );
  createTrackbar( "gamma_h", "Filtro",gh,100,on_gamma_h );
  createTrackbar( "gamma_l", "Filtro",gl,100,on_gamma_l );
 

  while(1){
    on_c(*c, 0 );
    on_d0(*d0, 0 );
    on_gamma_h(*gh, 0 );
    on_gamma_l(*gl, 0 );

    
    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                    dft_M - image.rows, 0,
                    dft_N - image.cols,
                    BORDER_CONSTANT, Scalar::all(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    TrocarQuadrantes(imagegray,complexImage);

    Filtro_Homomorfico(tmp,gl,gh,c,d0,dft_M,dft_N);

    // cria a matriz com as componentes do filtro e junta
    // ambas em uma matriz multicanal complexa
    Mat comps[]= {tmp, tmp};
    merge(comps, 2, filter);
    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

     // troca novamente os quadrantes
    TrocarQuadrantes(imagegray,complexImage);
 
    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("Filtro", planos[0]);
 
    if(waitKey(10)== 27 ) break; // esc pressed!
    
  }
  

  return 0;
}
