#include "hw3.h"

#define SIZE1 256
#define SIZE2 512
using namespace std;

int main(){
  unsigned char image[SIZE1][SIZE1] = {0};
  unsigned char B[SIZE1][SIZE1] = {0};
  unsigned char kernel[3][3] = {{255 , 255 , 255} , {255 , 255 , 255} , {255 , 255 , 255}};
  readin_gray(image , "./raw/sample1.raw");
  bound_extract(image , B , kernel , 3 , 3);
  write_image(B , "./raw/B.raw");
  unsigned char label[SIZE1][SIZE1] = {0};
  int number = labeling(image , label);
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      label[i][j] = label[i][j] * 30;
    }
  }
  write_image(label , "./raw/label.raw");
  printf("# of objects:%d\n" , number-1);
  unsigned char skeleton_reuslt[SIZE1][SIZE1];
  skeleton(image , skeleton_reuslt);
  write_image(skeleton_reuslt , "./raw/S.raw");
  return 0;
}