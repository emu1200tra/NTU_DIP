#include "hw3-3.h"

#define SIZE1 256
#define SIZE2 512
using namespace std;


int main(){
  unsigned char image[SIZE1][SIZE1] = {0};
  unsigned char result[SIZE1][SIZE1] = {0};
  unsigned char tmp[SIZE1][SIZE1] = {0};
  readin_gray(image , "./raw/sample3.raw");
  
  median_filter(image , result , 13);
  opening(result , tmp);
  closing(tmp , image);
  
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = int((result[i][j] / 5))*5;
      if (result[i][j] >= 255) result[i][j] = 255;
    }
  }

  unsigned char testing[SIZE1][SIZE1] = {0};
  erosion(image , tmp);
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      testing[i][j] = image[i][j];
      result[i][j] = (image[i][j] - tmp[i][j]);
      if(result[i][j] != 0){
        testing[i][j] = (image[i][j-1] + image[i][j+1] + image[i-1][j] + image[i+1][j])/4;
      }
    }
  } 

  changing(testing , result);

  unsigned char kernel[5][5] = {{0 , 1 , 1 , 1 , 0} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {0 , 1 , 1 , 1 , 0}};

  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      if(result[i][j] > 127){
        vector<int> list;
        for(int k = -2 ; k < 3 ; k++){
          for(int l = -2 ; l < 3 ; l++){
            if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1 && kernel[k+2][l+2] == 1){
              list.push_back(result[i+k][j+l]);
            }
          }
        }
        sort(list.begin(), list.end());
        image[i][j] = list.back();
      }
      else{
        vector<int> list;
        for(int k = -2 ; k < 3 ; k++){
          for(int l = -2 ; l < 3 ; l++){
            if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1 && kernel[k+2][l+2] == 1){
              list.push_back(result[i+k][j+l]);
            }
          }
        }
        sort(list.begin(), list.end());
        image[i][j] = list[0];
      }
    }
  } 

  median_filter(image , result , 9);

  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = int((result[i][j] / 7))*7;
      if (result[i][j] >= 255) result[i][j] = 255;
    }
  }

  write_image(result , "./raw/seq.raw");

  return 0;  
}

