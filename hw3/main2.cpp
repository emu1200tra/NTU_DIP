#include "hw3-2.h"

#define SIZE1 256
#define SIZE2 512
using namespace std;

int main(){
  unsigned char image2[SIZE2][SIZE2] = {0};
  readin_gray_big(image2 , "./raw/sample2.raw");
  printf("complete reading\n");
  unsigned char k_label[SIZE2][SIZE2] = {0};
  laws(image2 , k_label);
  write_image_big(k_label , "./raw/k.raw");
  unsigned char exchange_result[SIZE2][SIZE2] = {0};
  exchange(image2 , k_label , exchange_result);
  write_image_big(exchange_result , "./raw/exchange.raw");
  return 0;  
}

