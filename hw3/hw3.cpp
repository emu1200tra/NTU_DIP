#include "hw3.h"

#define SIZE1 256
#define SIZE2 512
using namespace std;


void readin_gray(unsigned char image_data[SIZE1][SIZE1] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "rb"))){
    printf("Cannot open file! :%s\n" , name.c_str());
    exit(1);
  }
  fread(image_data, sizeof(unsigned char), SIZE1* SIZE1, file);
  fclose(file);
}

void write_image(unsigned char result[SIZE1][SIZE1] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "wb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fwrite(result, sizeof(unsigned char), SIZE1* SIZE1, file);
  fclose(file);
}

void erosion(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      int counter = 0 , check_sign = 0;
      for(int k = -w/2 ; k <= w/2 ; k++){
        for(int l = -h/2 ; l <= h/2 ; l++){
          if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1){
            check_sign++;
            if(image[i+k][j+l] == kernel[k+(w/2)][l+(h/2)]){
              counter++;
            }
          }
        }
      }
      if(counter == check_sign){
        result[i][j] = 255;        
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void bound_extract(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h){
  unsigned char tmp[SIZE1][SIZE1];
  erosion(image , tmp , kernel , w , h);
  printf("erosion!\n");
  write_image(tmp , "./raw/erosion_result.raw");
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = image[i][j] - tmp[i][j];
    }
  }
}

void dilation(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = 0;
    }
  }
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      if(image[i][j] == 255){
        for(int k = -w/2 ; k <= w/2 ; k++){
          for(int l = -h/2 ; l <= h/2 ; l++){
            if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1 && kernel[k+w/2][l+h/2] == 255){
              result[i+k][j+l] = 255;        
            }
          }
        }
      }
    }
  }
}

void invsserse(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = 255 - image[i][j];
    }
  }
}

int labeling(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  int label_value = 1;
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = 0;
    }
  }
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      if(image[i][j] == 255 && result[i][j] == 0){
        recurssion(image , result , label_value , i , j);
        label_value++;
      }
    }
  }
  return label_value;
}

void recurssion(unsigned char image[SIZE1][SIZE1] , unsigned char label[SIZE1][SIZE1] , int label_value , int x , int y){
  label[x][y] = label_value;
  if(image[x+1][y] == 255 && label[x+1][y] == 0 && x+1 < SIZE1){
    recurssion(image , label , label_value , x+1 , y);
  }
  if(image[x-1][y] == 255 && label[x-1][y] == 0 && x-1 >= 0){
    recurssion(image , label , label_value , x-1 , y);
  }
  if(image[x][y+1] == 255 && label[x][y+1] == 0 && y+1 < SIZE1){
    recurssion(image , label , label_value , x , y+1);
  }
  if(image[x][y-1] == 255 && label[x][y-1] == 0 && y-1 >= 0){
    recurssion(image , label , label_value , x , y-1);
  }
  if(image[x+1][y+1] == 255 && label[x+1][y+1] == 0 && x+1 < SIZE1 && y+1 < SIZE1){
    recurssion(image , label , label_value , x+1 , y+1);
  }
  if(image[x-1][y+1] == 255 && label[x-1][y+1] == 0 && x-1 >= 0 && y+1 < SIZE1){
    recurssion(image , label , label_value , x-1 , y+1);
  }
  if(image[x+1][y-1] == 255 && label[x+1][y-1] == 0 && x+1 < SIZE1 && y-1 >= 0){
    recurssion(image , label , label_value , x+1 , y-1);
  }
  if(image[x-1][y-1] == 255 && label[x-1][y-1] == 0 && x-1 >= 0 && y-1 >= 0){
    recurssion(image , label , label_value , x-1 , y-1);
  }
}

void diff(unsigned char image1[SIZE1][SIZE1] , unsigned char image2[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  for(int i = 0 ; i < SIZE1 ; i++){
    for (int j = 0; j < SIZE1; j++){
      if (image1[i][j] == 255 && image2[i][j] == 0){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void uni(unsigned char image1[SIZE1][SIZE1] , unsigned char image2[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      if(image1[i][j] == 255 || image2[i][j] == 255){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void opening(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  unsigned char tmp[SIZE1][SIZE1] = {0};
  unsigned char kernel[3][3] = {{255 , 255 , 255} , {255 , 255 , 255} , {255 , 255 , 255}};
  erosion(image , tmp , kernel , 3 , 3);
  dilation(tmp , result , kernel , 3 , 3);
}

void skeleton(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  unsigned char erosion_result[SIZE1][SIZE1] = {0};
  unsigned char kernel[3][3] = {{255 , 255 , 255} , {255 , 255 , 255} , {255 , 255 , 255}};
  erosion(image , erosion_result , kernel , 3 , 3);
  unsigned char skeleton_result[SIZE1][SIZE1] = {0};
  unsigned char open_result[SIZE1][SIZE1] = {0};
  opening(erosion_result , open_result);
  diff(erosion_result , open_result , skeleton_result);
  unsigned char tmp[SIZE1][SIZE1] = {0};
  unsigned char tmp2[SIZE1][SIZE1] = {0};
  unsigned char tmp3[SIZE1][SIZE1] = {0};
  for(int i = 0 ; i < 20 ; i++){
    erosion(erosion_result , tmp , kernel , 3 , 3);
    opening(tmp , open_result);
    diff(tmp , open_result , tmp2);
    uni(skeleton_result , tmp2 , tmp3);
    erosion(tmp , erosion_result , kernel , 3 , 3);
    opening(erosion_result , open_result);
    diff(erosion_result , open_result , tmp2);
    uni(tmp2 , tmp3 , skeleton_result);
  }
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = skeleton_result[i][j];
    }
  }
}

