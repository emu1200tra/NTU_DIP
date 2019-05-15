#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include <string>
#include <cstring>
#include <fstream>
#include <math.h>
#include <random>
#include <time.h>
#include <algorithm>    
#include <vector>  

#define SIZE 512
using namespace std;

void readin_gray(unsigned char image_data[SIZE][SIZE] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "rb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fread(image_data, sizeof(unsigned char), SIZE* SIZE, file);
  fclose(file);
}

void write_image(unsigned char result[SIZE][SIZE] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "wb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fwrite(result, sizeof(unsigned char), SIZE* SIZE, file);
  fclose(file);
}

void prewitt (unsigned char image[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int threshold){
  unsigned char tmp_list[SIZE+2][SIZE+2] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      tmp_list[i+1][j+1] = image[i][j];
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int p1 = -(tmp_list[i][j] + tmp_list[i][j+1] + tmp_list[i][j+2]) + (tmp_list[i+2][j] + tmp_list[i+2][j+1] + tmp_list[i+2][j+2]);
      int p2 = -(tmp_list[i][j] + tmp_list[i+1][j] + tmp_list[i+2][j]) + (tmp_list[i][j+2] + tmp_list[i+1][j+2] + tmp_list[i+2][j+2]);
      float value = sqrt(p1*p1 + p2*p2);
      if (value > threshold){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void sobel(unsigned char image[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int threshold){

  unsigned char tmp_list[SIZE+2][SIZE+2] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      tmp_list[i+1][j+1] = image[i][j];
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int p1 = -(tmp_list[i][j] + 2*tmp_list[i][j+1] + tmp_list[i][j+2]) + (tmp_list[i+2][j] + 2*tmp_list[i+2][j+1] + tmp_list[i+2][j+2]);
      int p2 = -(tmp_list[i][j] + 2*tmp_list[i+1][j] + tmp_list[i+2][j]) + (tmp_list[i+2][j] + 2*tmp_list[i+2][j+1] + tmp_list[i+2][j+2]);
      float value = sqrt(p1*p1 + p2*p2);
      if (value > threshold){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void lapla_mask(unsigned char image[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int threshold  , int kernel[11][11] , int parameter ){
  unsigned char image_list[SIZE+10][SIZE+10];
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      image_list[i+5][j+5] = image[i][j];
    }
  }
  int counter = 0;
  int record[SIZE+10][SIZE+10] = {0};
  int range_list[11] = {-5 , -4 , -3 , -2 , -1 , 0 , 1 , 2 , 3 , 4 , 5};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      for(int k = 0 ; k < 11 ; k++){
        for(int l = 0 ; l < 11 ; l++){
          counter += image_list[i+5+range_list[k]][j+5+range_list[l]] * kernel[5+range_list[k]][5+range_list[l]];
        }
      }
      counter *= parameter;
      if (counter > threshold){
        record[i+5][j+5] = 1;
      }
      else if(-counter > threshold){
        record[i+5][j+5] = -1;
      }
      else{
        record[i+5][j+5] = 0;
      }
      counter = 0;
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = 0;
      if(record[i+5][j+5] == 1){
        for(int k = 0 ; k < 11 ; k++){
          for(int l = 0 ; l < 11 ; l++){
            if(record[i+5+range_list[k]][j+5+range_list[l]] == -1){
              result[i][j] = 255;
            }
          }
        }
      }
    }
  }
}

void grad(unsigned char **image , unsigned char gradient[SIZE][SIZE] , int G1[SIZE][SIZE] , int G2[SIZE][SIZE]){
  unsigned char tmp_list[SIZE+2][SIZE+2] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      tmp_list[i+1][j+1] = image[i][j];
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int p1 = -(tmp_list[i][j] + tmp_list[i][j+1] + tmp_list[i][j+2]) + (tmp_list[i+2][j] + tmp_list[i+2][j+1] + tmp_list[i+2][j+2]);
      int p2 = -(tmp_list[i][j] + tmp_list[i+1][j] + tmp_list[i+2][j]) + (tmp_list[i][j+2] + tmp_list[i+1][j+2] + tmp_list[i+2][j+2]);
      G1[i][j] = p1;
      G2[i][j] = p2;
      gradient[i][j] = sqrt(p1*p1 + p2*p2);
    }
  }
}

void grad_2(unsigned char image[SIZE][SIZE] , unsigned char gradient[SIZE][SIZE] , int G1[SIZE][SIZE] , int G2[SIZE][SIZE]){
  unsigned char tmp_list[SIZE+2][SIZE+2] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      tmp_list[i+1][j+1] = image[i][j];
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int p1 = -(tmp_list[i][j] + tmp_list[i][j+1] + tmp_list[i][j+2]) + (tmp_list[i+2][j] + tmp_list[i+2][j+1] + tmp_list[i+2][j+2]);
      int p2 = -(tmp_list[i][j] + tmp_list[i+1][j] + tmp_list[i+2][j]) + (tmp_list[i][j+2] + tmp_list[i+1][j+2] + tmp_list[i+2][j+2]);
      G1[i][j] = p1;
      G2[i][j] = p2;
      gradient[i][j] = sqrt(p1*p1 + p2*p2);
    }
  }
}

void canny(unsigned char image[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int threshold1 , int threshold2){
  unsigned char **image_list = (unsigned char **)malloc((SIZE+4) * sizeof(unsigned char*));
  unsigned char **smooth_image = (unsigned char **)malloc((SIZE+4) * sizeof(unsigned char*));
  
  for(int i = 0 ; i < SIZE+4 ; i++){
    image_list[i] = (unsigned char *)malloc((SIZE+4) * sizeof(unsigned char));
    smooth_image[i] = (unsigned char *)malloc((SIZE+4) * sizeof(unsigned char));
    for(int j = 0 ; j < SIZE+4 ; j++){
      image_list[i][j] = 0;
      smooth_image[i][j] = 0;
    }
  }
  unsigned char gradient[SIZE][SIZE] = {0};
  unsigned char tmp[SIZE][SIZE] = {0};
  int kernel[5][5] = {{2 , 4 , 5 , 4 , 2} , {4 , 9 , 12 , 9 , 4} , {5 , 12 , 15 , 12 , 5} , {4 , 9 , 12 , 9 , 4} , {2 , 4 , 5 , 4 , 2}};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      image_list[i+2][j+2] = image[i][j];
    }
  }
  int counter = 0;
  int record_value = 0;

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      for(int k = -2 ; k < 3 ; k++){
        for(int l = -2 ; l < 3 ; l++){
          counter += image_list[i+2+k][j+2+l] * kernel[2+k][2+l];
          if(i+2+k > 2 && i+2+k < SIZE+1 && j+2+l > 2 && j+2+l < SIZE+1){
            record_value += kernel[2+k][2+l];
          }
        }
      }
      smooth_image[i+2][j+2] = (float)counter / record_value;
      counter = 0;
      record_value = 0;
    }
  } 
  free(image_list);
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      tmp[i][j] = smooth_image[i+2][j+2];
    }
  }
  free(smooth_image);
  int G1[SIZE][SIZE];
  int G2[SIZE][SIZE];
  grad(smooth_image , gradient , G1 , G2);
  float theta[SIZE][SIZE];
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      theta[i][j] = atan((float)G2[i][j] / (float)G1[i][j]);
    }
  }
  int max_map[SIZE][SIZE] = {0};
  for(int i = 1 ; i < SIZE-1 ; i++){
    for(int j = 1 ; j < SIZE-1 ; j++){
      if((theta[i][j] > 22.5 && theta[i][j] <= 67.5) || (theta[i][j] > -157.5 && theta[i][j] <= -112.5)){
        if(gradient[i][j] > gradient[i-1][j+1] && gradient[i][j] > gradient[i+1][j-1]){
          max_map[i][j] = gradient[i][j];
        }
      }
      else if((theta[i][j] > 67.5 && theta[i][j] <= 112.5) || (theta[i][j] > -112.5 && theta[i][j] <= -67.5)){
        if(gradient[i][j] > gradient[i-1][j] && gradient[i][j] > gradient[i+1][j]){
          max_map[i][j] = gradient[i][j];
        }
      }
      else if((theta[i][j] <= 22.5 && theta[i][j] >= -22.5) || (theta[i][j] >= -157.5 && theta[i][j] <= -157.5)){
        if(gradient[i][j] > gradient[i][j-1] && gradient[i][j] > gradient[i][j+1]){
          max_map[i][j] = gradient[i][j];
        }
      }
      else{
        if(gradient[i][j] > gradient[i-1][j-1] && gradient[i][j] > gradient[i+1][j+1]){
          max_map[i][j] = gradient[i][j];
        }
      }
    }
  }
  unsigned char label_map[SIZE][SIZE] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(max_map[i][j] >= threshold1){
        label_map[i][j] = 2;
      }
      else if(max_map[i][j] < threshold1 && max_map[i][j] >= threshold2){
        label_map[i][j] = 1;
      }
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(label_map[i][j] == 2){
        result[i][j] = 255;
      }
      else if (label_map[i][j] == 1 && (label_map[i-1][j-1] == 2 || label_map[i-1][j] == 2 || label_map[i-1][j+1] == 2 ||
               label_map[i][j-1] == 2 || label_map[i][j+1] == 2 || label_map[i+1][j-1] == 2 || label_map[i+1][j] == 2 || label_map[i+1][j+1] == 2)){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void dilation(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  unsigned char map_record[SIZE+4][SIZE+4] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      map_record[i+2][j+2] = image_data[i][j];
    }
  }

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(image_data[i][j] == 255){
        /*
        map_record[i+1][j] += 1 ; map_record[i+3][j] += 1 ; map_record[i+2][j] += 1;
        map_record[i][j+1] += 1 ; map_record[i+1][j+1] += 1 ; map_record[i+2][j+1] += 1 ; map_record[i+3][j+1] += 1 ; map_record[i+4][j+1] += 1 ;
        map_record[i][j+2] += 1 ; map_record[i+1][j+2] += 1 ; map_record[i+2][j+2] += 1 ; map_record[i+3][j+2] += 1 ; map_record[i+4][j+2] += 1 ;
        map_record[i][j+3] += 1 ; map_record[i+1][j+3] += 1 ; map_record[i+2][j+3] += 1 ; map_record[i+3][j+3] += 1 ; map_record[i+4][j+3] += 1 ;
        map_record[i+1][j+4] += 1 ; map_record[i+3][j+4] += 1 ; map_record[i+2][j+4] += 1;
        */
        map_record[i+1][j+1] += 1 ; map_record[i+2][j+1] += 1 ; map_record[i+3][j+1] += 1 ;
        map_record[i+1][j+2] += 1 ; map_record[i+2][j+2] += 1 ; map_record[i+3][j+2] += 1 ;
        map_record[i+1][j+3] += 1 ; map_record[i+2][j+3] += 1 ; map_record[i+3][j+3] += 1 ;
      }
    }
  }

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(map_record[i+2][j+2] != 0){
        result[i][j] = 255;
      }
      else{
        result[i][j] = 0;
      }
    }
  }
}

void box_filter(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int box_size){
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      float counter = 0.0 , sum_pixel = 0.0;
      for(int k = -(box_size/2) ; k <= box_size/2 ; k++){
        for(int l = -(box_size/2) ; l <= box_size/2 ; l++){
          if(i+k < 0 || i+k > SIZE-1 || j+l < 0 || j+l > SIZE-1){
            continue;
          }
          counter++;
          sum_pixel += image_data[i+k][j+l];
        }
      }
      result[i][j] = round(sum_pixel/counter);
    }
  }
}

int main(){
  unsigned char image_data[SIZE][SIZE];
  unsigned char result[SIZE][SIZE];
  string name = "../raw/sample1.raw";
  readin_gray(image_data , name);
  int threshold[6] = {25 , 50 , 75 , 100 , 125 , 150};
  int threshold2[6] = {1000 , 1500 , 2000 , 2500 , 3000 , 3500};
  int threshold3[6] = {0 , 1 , 2 , 5 , 7 , 10};
  int mask1[11][11] = {{0 , 0 , 0 , -1 , -1 , -2 , -1 , -1 , 0 , 0 , 0} ,
                       {0 , 0 , -2 , -4 , -8 , -9 , -8 , -4 , -2 , 0 , 0} ,
                       {0 , -2 , -7 , -15 , -22 , -23 , -22 , -15 , -7 , -2 , 0} ,
                       {-1 , -4 , -15 , -24 , -14 , -1 , -14 , -24 , -15 , -4 , -1} ,
                       {-1 , -8 , -22 , -14 , 52 , 103 , 52 , -14 , -22 , -8 , -1} ,
                       {-2 , -9 , -23 , -1 , 103 , 178 , 103 , -1 , -23 , -9 , -2} ,
                       {-1 , -8 , -22 , -14 , 52 , 103 , 52 , -14 , -22 , -8 , -1} ,
                       {-1 , -4 , -15 , -24 , -14 , -1 , -14 , -24 , -15 , -4 , -1} ,
                       {0 , -2 , -7 , -15 , -22 , -23 , -22 , -15 , -7 , -2 , 0} ,
                       {0 , 0 , -2 , -4 , -8 , -9 , -8 , -4 , -2 , 0 , 0} ,
                       {0 , 0 , 0 , -1 , -1 , -2 , -1 , -1 , 0 , 0 , 0}};
  int maks2[11][11] = {{-1 , -3 , -4 , -6 , -7 , -8 , -7 , -6 , -4 , -3 , -1} ,
                       {-3 , -5 , -8 , -11 , -13 , -13 , -13 , -11 , -8 , -5 , -3} ,
                       {-4 , -8 , -12 , -16 , -17 , -17 , -17 , -16 , -12 , -8 , -4} ,
                       {-6 , -11 , -16 , -16 , 0 , 15 , 0 , -16 , -16 , -11 , -6} ,
                       {-7 , -13 , -17 , 0 , 85 , 160 , 85 , 0 , -17 , -13 , -7} ,
                       {-8 , -13 , -17 , 15 , 160 , 283 , 160 , 15 , -17 , -13 , -8} ,
                       {-7 , -13 , -17 , 0 , 85 , 160 , 85 , 0 , -17 , -13 , -7} ,
                       {-6 , -11 , -16 , -16 , 0 , 15 , 0 , -16 , -16 , -11 , -6} ,
                       {-4 , -8 , -12 , -16 , -17 , -17 , -17 , -16 , -12 , -8 , -4} ,
                       {-3 , -5 , -8 , -11 , -13 , -13 , -13 , -11 , -8 , -5 , -3} ,
                       {-1 , -3 , -4 , -6 , -7 , -8 , -7 , -6 , -4 , -3 , -1}};
  int threshold4[6] = {10 , 20 , 30 , 40 , 50 , 60};
  int threshold5[4] = {30 , 50 , 70 , 100};
  for(int i = 0 ; i < 6 ; i++){
    prewitt(image_data , result , threshold[i]);
    name = "../raw/prewitt_" + to_string(threshold[i]) + "_result.raw";
    write_image(result , name);  
    sobel(image_data , result , threshold[i]);
    name = "../raw/sobel_" + to_string(threshold[i]) + "_result.raw";
    write_image(result , name);
    lapla_mask(image_data , result , threshold2[i] , mask1 , 1);
    name = "../raw/LOG_" + to_string(threshold2[i]) + "_result.raw";
    write_image(result , name);
    lapla_mask(image_data , result , threshold3[i] , maks2, 1);
    name = "../raw/DOG_" + to_string(threshold3[i]) + "_result.raw";
    write_image(result , name);
    for(int j = 0 ; j < 4 ; j++){
      if(threshold5[j] > threshold4[i]){
        canny(image_data , result , threshold5[j] , threshold4[i]);
        name = "../raw/canny_" + to_string(threshold5[j]) + "_" + to_string(threshold4[i]) + "_result.raw";
        write_image(result, name);
      }
    }
  }
  name = "../raw/sample2.raw";
  readin_gray(image_data , name);
  unsigned char test_gradient[SIZE][SIZE];
  int G1[SIZE][SIZE];
  int G2[SIZE][SIZE];
  FILE *file = fopen("test.txt" , "w");
  grad_2(image_data , test_gradient , G1 , G2);
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      fprintf(file, "%d ", test_gradient[i][j]);
    }
    fprintf(file , "\n");
  }
  name = "../raw/test_result.raw";
  write_image(test_gradient , name);
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = 255 - test_gradient[i][j];
      if(result[i][j] < 0) result[i][j] = 0;
      if(result[i][j] > 255) result[i][j] = 255;
    }
  }
  name = "../raw/test_result2.raw";
  write_image(result , name);
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(result[i][j] > 135) result[i][j] = 255;
      else result[i][j] = 0;
      result[i][j] = 255 - result[i][j];
    }
  }
  name = "../raw/test_result3.raw";
  write_image(result , name);
  dilation(result , image_data);
  name = "../raw/test_result4.raw";
  write_image(image_data , name);

}