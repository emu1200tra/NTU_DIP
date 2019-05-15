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
#define PI 3.14159265
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

unsigned char truncate(float value)
{
    if(value < 0) return 0;
    else if(value > 255) return 255;
    else return (unsigned char)value;
}

void sharpen(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  float **tmp1 = (float**)malloc(SIZE*sizeof(float*));
  float **tmp2 = (float**)malloc(SIZE*sizeof(float*));
  unsigned char loww_pass[SIZE][SIZE];

  box_filter(image_data , loww_pass , 9);
  for(int i = 0 ; i < SIZE ; i++){
    tmp1[i] = (float*)malloc(SIZE*sizeof(float));
    tmp2[i] = (float*)malloc(SIZE*sizeof(float));
    for(int j = 0 ; j < SIZE ; j++){
      tmp1[i][j] = image_data[i][j];
      tmp2[i][j] = loww_pass[i][j];
    }
  }  
  printf("done boxfilter\n");
  float c = 0.7;
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if(round((c/(2.0*c-1.0))*tmp1[i][j]-((1.0-c)/(2.0*c-1.0))*tmp2[i][j]) < 0){
        result[i][j] = 0;
      }
      else{
        result[i][j] = round((c/(2.0*c-1.0))*tmp1[i][j]-((1.0-c)/(2.0*c-1.0))*tmp2[i][j]);
      }
    }
  }  
  free(tmp1);
  free(tmp2);
  printf("done sharpen\n");
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

void Nevatia_Babu(unsigned char image[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int threshold){
  int **tmp_list_big = (int**)malloc((SIZE+4) * sizeof(int*));
  for(int i = 0 ; i < SIZE+4 ; i++){
    tmp_list_big[i] = (int*)malloc((SIZE+4)*sizeof(int));
    for(int j = 0 ; j < SIZE+4 ; j++){
      tmp_list_big[i][j] = 0;
      if(i >= 2 && i < SIZE+2 && j >= 2 && j < SIZE+2){
        tmp_list_big[i][j] = image[i-2][j-2];
      }
    }
  }
  int k[6] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      k[0] = (100*tmp_list_big[i][j] + 100*tmp_list_big[i][j+1] + 100*tmp_list_big[i][j+2] + 100*tmp_list_big[i][j+3] + 100*tmp_list_big[i][j+4] +
              100*tmp_list_big[i+1][j] + 100*tmp_list_big[i+1][j+1] + 100*tmp_list_big[i+1][j+2] + 100*tmp_list_big[i+1][j+3] + 100*tmp_list_big[i+1][j+4] -
             (100*tmp_list_big[i+3][j] + 100*tmp_list_big[i+3][j+1] + 100*tmp_list_big[i+3][j+2] + 100*tmp_list_big[i+3][j+3] + 100*tmp_list_big[i+3][j+4]) -
             (100*tmp_list_big[i+4][j] + 100*tmp_list_big[i+4][j+1] + 100*tmp_list_big[i+4][j+2] + 100*tmp_list_big[i+4][j+3] + 100*tmp_list_big[i+4][j+4]));  
      k[1] = (100*tmp_list_big[i][j] + 100*tmp_list_big[i][j+1] + 100*tmp_list_big[i][j+2] + 100*tmp_list_big[i][j+3] + 100*tmp_list_big[i][j+4] +
              100*tmp_list_big[i+1][j] + 100*tmp_list_big[i+1][j+1] + 100*tmp_list_big[i+1][j+2] + 78*tmp_list_big[i+1][j+3] + (-32)*tmp_list_big[i+1][j+4] +
              100*tmp_list_big[i+2][j] + 92*tmp_list_big[i+2][j+1] + (-92)*tmp_list_big[i+2][j+3] - 100*tmp_list_big[i+2][j+4] + 
              32*tmp_list_big[i+3][j] + (-78)*tmp_list_big[i+3][j+1] + (-100)*tmp_list_big[i+3][j+2] + (-100)*tmp_list_big[i+3][j+3] + (-100)*tmp_list_big[i+3][j+4] -
              (100*tmp_list_big[i+4][j] + 100*tmp_list_big[i+4][j+1] + 100*tmp_list_big[i+4][j+2] + 100*tmp_list_big[i+4][j+3] + 100*tmp_list_big[i+4][j+4]));
      k[2] = (100*tmp_list_big[i][j] + 100*tmp_list_big[i][j+1] + 100*tmp_list_big[i][j+2] + 32*tmp_list_big[i][j+3] - 100*tmp_list_big[i][j+4] +
              100*tmp_list_big[i+1][j] + 100*tmp_list_big[i+1][j+1] + 92*tmp_list_big[i+1][j+2] - 78*tmp_list_big[i+1][j+3] - 100*tmp_list_big[i+1][j+4] +
              100*tmp_list_big[i+2][j] + 100*tmp_list_big[i+2][j+1] + (-100)*tmp_list_big[i+2][j+3] - 100*tmp_list_big[i+2][j+4] + 
              100*tmp_list_big[i+3][j] + (78)*tmp_list_big[i+3][j+1] + (-92)*tmp_list_big[i+3][j+2] + (-100)*tmp_list_big[i+3][j+3] + (-100)*tmp_list_big[i+3][j+4] +
              100*tmp_list_big[i+4][j] + (-32)*tmp_list_big[i+4][j+1] + (-100)*tmp_list_big[i+4][j+2] - 100*tmp_list_big[i+4][j+3] - 100*tmp_list_big[i+4][j+4]);
      k[3] = ((-100)*tmp_list_big[i][j] + (-100)*tmp_list_big[i][j+1] + (100)*tmp_list_big[i][j+3] + (100)*tmp_list_big[i][j+4] +
              (-100)*tmp_list_big[i+1][j] + (-100)*tmp_list_big[i+1][j+1] + 100*tmp_list_big[i+1][j+3] + 100*tmp_list_big[i+1][j+4] +
              (-100)*tmp_list_big[i+2][j] + (-100)*tmp_list_big[i+2][j+1] + 100*tmp_list_big[i+2][j+3] + 100*tmp_list_big[i+2][j+4] + 
              (-100)*tmp_list_big[i+3][j] + (-100)*tmp_list_big[i+3][j+1] + 100*tmp_list_big[i+3][j+3] + 100*tmp_list_big[i+3][j+4] +
              (-100)*tmp_list_big[i+4][j] + (-100)*tmp_list_big[i+4][j+1] + 100*tmp_list_big[i+4][j+3] + 100*tmp_list_big[i+4][j+4]);
      k[4] = ((-100)*tmp_list_big[i][j] + 32*tmp_list_big[i][j+1] + 100*tmp_list_big[i][j+2] + 100*tmp_list_big[i][j+3] + 100*tmp_list_big[i][j+4] +
              (-100)*tmp_list_big[i+1][j] + (-78)*tmp_list_big[i+1][j+1] + 92*tmp_list_big[i+1][j+2] + 100*tmp_list_big[i+1][j+3] + 100*tmp_list_big[i+1][j+4] +
              (-100)*tmp_list_big[i+2][j] + (-100)*tmp_list_big[i+2][j+1] + 100*tmp_list_big[i+2][j+3] + 100*tmp_list_big[i+2][j+4] + 
              (-100)*tmp_list_big[i+3][j] + (-100)*tmp_list_big[i+3][j+1] + (-92)*tmp_list_big[i+3][j+2] + 78*tmp_list_big[i+3][j+3] + 100*tmp_list_big[i+3][j+4] +
              (-100)*tmp_list_big[i+4][j] + (-100)*tmp_list_big[i+4][j+1] + (-100)*tmp_list_big[i+4][j+2] - 32*tmp_list_big[i+4][j+3] + 100*tmp_list_big[i+4][j+4]);
      k[5] = (100*tmp_list_big[i][j] + 100*tmp_list_big[i][j+1] + 100*tmp_list_big[i][j+2] + 100*tmp_list_big[i][j+3] + 100*tmp_list_big[i][j+4] +
              (-32)*tmp_list_big[i+1][j] + 78*tmp_list_big[i+1][j+1] + 100*tmp_list_big[i+1][j+2] + 100*tmp_list_big[i+1][j+3] + 100*tmp_list_big[i+1][j+4] +
              (-100)*tmp_list_big[i+2][j] + (-92)*tmp_list_big[i+2][j+1] + 92*tmp_list_big[i+2][j+3] + 100*tmp_list_big[i+2][j+4] + 
              (-100)*tmp_list_big[i+3][j] + (-100)*tmp_list_big[i+3][j+1] + (-100)*tmp_list_big[i+3][j+2] + (-78)*tmp_list_big[i+3][j+3] + 32*tmp_list_big[i+3][j+4] +
              (-100)*tmp_list_big[i+4][j] + (-100)*tmp_list_big[i+4][j+1] + (-100)*tmp_list_big[i+4][j+2] - 100*tmp_list_big[i+4][j+3] - 100*tmp_list_big[i+4][j+4]);
      int value = *max_element(k , k+6);
      if (value > threshold) result[i][j] = 255;
      else  result[i][j] = 0;
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
        
        map_record[i+1][j] += 1 ; map_record[i+3][j] += 1 ; map_record[i+2][j] += 1;
        map_record[i][j+1] += 1 ; map_record[i+1][j+1] += 1 ; map_record[i+2][j+1] += 1 ; map_record[i+3][j+1] += 1 ; map_record[i+4][j+1] += 1 ;
        map_record[i][j+2] += 1 ; map_record[i+1][j+2] += 1 ; map_record[i+2][j+2] += 1 ; map_record[i+3][j+2] += 1 ; map_record[i+4][j+2] += 1 ;
        map_record[i][j+3] += 1 ; map_record[i+1][j+3] += 1 ; map_record[i+2][j+3] += 1 ; map_record[i+3][j+3] += 1 ; map_record[i+4][j+3] += 1 ;
        map_record[i+1][j+4] += 1 ; map_record[i+3][j+4] += 1 ; map_record[i+2][j+4] += 1;
        
       
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

void erosion(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  int map_record[SIZE+4][SIZE+4];
  for(int i = 0 ; i < SIZE+4 ; i++){
    for(int j = 0 ; j < SIZE+4 ; j++){
      if(i < 2 || i > SIZE+1 || j < 2 || j > SIZE+1){
        map_record[i][j] = 256;
      }
      else{
        map_record[i][j] = image_data[i-2][j-2];
      }
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int list[21] = {map_record[i+1][j] , map_record[i+3][j] , map_record[i+2][j] , map_record[i][j+1] , map_record[i+1][j+1] , map_record[i+2][j+1] ,
                      map_record[i+3][j+1] , map_record[i+4][j+1] , map_record[i][j+2] , map_record[i+1][j+2] , map_record[i+2][j+2] , map_record[i+3][j+2] ,
                      map_record[i+4][j+2] , map_record[i][j+3] , map_record[i+1][j+3] , map_record[i+2][j+3] , map_record[i+3][j+3] , map_record[i+4][j+3] ,
                      map_record[i+1][j+4] , map_record[i+3][j+4] , map_record[i+2][j+4]};
      vector<int> to_sort(list , list+21);
      sort(to_sort.begin(), to_sort.end());
      result[i][j] = to_sort[0];
    }
  }
}

void grad_2(unsigned char image[SIZE][SIZE] , unsigned char gradient[SIZE][SIZE]){
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
      gradient[i][j] = sqrt(p1*p1 + p2*p2);
    }
  }
}

void Gamma(unsigned char image[SIZE][SIZE] /*, unsigned char grad[SIZE][SIZE]*/ , unsigned char result[SIZE][SIZE] , float gamma){
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      /*if(grad[i][j] == 0)*/
      result[i][j] = pow(image[i][j]/255.0 , 1.0/gamma)*255.0;          
      //else result[i][j] = image[i][j];

      if(result[i][j] > 255) result[i][j] = 255;
      else if(result[i][j] < 0) result[i][j] = 0;
    }
  }
}

void dilation2(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  int map_record[SIZE+4][SIZE+4];
  for(int i = 0 ; i < SIZE+4 ; i++){
    for(int j = 0 ; j < SIZE+4 ; j++){
      if(i < 2 || i > SIZE+1 || j < 2 || j > SIZE+1){
        map_record[i][j] = -1;
      }
      else{
        map_record[i][j] = image_data[i-2][j-2];
      }
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int list[21] = {map_record[i+1][j] , map_record[i+3][j] , map_record[i+2][j] , map_record[i][j+1] , map_record[i+1][j+1] , map_record[i+2][j+1] ,
                      map_record[i+3][j+1] , map_record[i+4][j+1] , map_record[i][j+2] , map_record[i+1][j+2] , map_record[i+2][j+2] , map_record[i+3][j+2] ,
                      map_record[i+4][j+2] , map_record[i][j+3] , map_record[i+1][j+3] , map_record[i+2][j+3] , map_record[i+3][j+3] , map_record[i+4][j+3] ,
                      map_record[i+1][j+4] , map_record[i+3][j+4] , map_record[i+2][j+4]};
      vector<int> to_sort(list , list+21);
      sort(to_sort.begin(), to_sort.end());
      result[i][j] = to_sort[20];
    }
  }
}

void erosion2(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  int map_record[SIZE+4][SIZE+4];
  for(int i = 0 ; i < SIZE+4 ; i++){
    for(int j = 0 ; j < SIZE+4 ; j++){
      if(i < 2 || i > SIZE+1 || j < 2 || j > SIZE+1){
        map_record[i][j] = 256;
      }
      else{
        map_record[i][j] = image_data[i-2][j-2];
      }
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int list[21] = {map_record[i+1][j] , map_record[i+3][j] , map_record[i+2][j] , map_record[i][j+1] , map_record[i+1][j+1] , map_record[i+2][j+1] ,
                      map_record[i+3][j+1] , map_record[i+4][j+1] , map_record[i][j+2] , map_record[i+1][j+2] , map_record[i+2][j+2] , map_record[i+3][j+2] ,
                      map_record[i+4][j+2] , map_record[i][j+3] , map_record[i+1][j+3] , map_record[i+2][j+3] , map_record[i+3][j+3] , map_record[i+4][j+3] ,
                      map_record[i+1][j+4] , map_record[i+3][j+4] , map_record[i+2][j+4]};
      vector<int> to_sort(list , list+21);
      sort(to_sort.begin(), to_sort.end());
      result[i][j] = to_sort[0];
    }
  }
}

void histogram(unsigned char image_data[SIZE][SIZE] , int histo[256]){
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      histo[image_data[i][j]]++;
    }
  }
}

int main(){
  printf("mem error?\n");
  unsigned char image_data[SIZE][SIZE];
  string name = "../raw/sample3.raw";
  readin_gray(image_data , name);

  unsigned char result[SIZE][SIZE];
  
  sharpen(image_data , result);

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      image_data[i][j] = 0;      
    }
  }

  name = "../raw/C.raw";
  write_image(result , name);    
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      //int new_y = (i + 20.0*sin(2.0*PI*j/128.0));
      int new_y = round(i + (sin((float)(j/50.0))*30.0));
      if(new_y >= 0 && new_y < SIZE){
        image_data[new_y][j] = result[i][j]; 
      }
    }
  }
  printf("done reshape\n");
  name = "../raw/CtoD.raw";
  write_image(image_data , name); 

  for(int i = 0 ; i < SIZE ; i++){
    for(int j= 0 ; j < SIZE ; j++){
      result[i][j] = 0;
    }
  }

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      int new_x = round(j + (sin((float)(i/50.0))*30.0));
      if(new_x >= 0 && new_x < SIZE){
        result[i][new_x] = image_data[i][j]; 
      }
    }
  }

  name = "../raw/D.raw";
  write_image(result , name);

  name = "../raw/sample4.raw";
  readin_gray(image_data , name);
/*
  float contrast = -100;
  float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      unsigned char newvalue = truncate((float)(factor * (image_data[i][j] - 128) + 128));
      image_data[i][j] = newvalue;
    }
  }
  name = "../raw/contrast.raw";
  write_image(image_data , name);
*/


    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        result[i][j] = image_data[i][j];
      }
    }


  /*
    sharpen(image_data , result);

    name = "../raw/contrast_sharpen.raw";
    write_image(result , name);
  */
  /*
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        result[i][j] *= (255.0/220.0);
        if(result[i][j] >= 255) result[i][j] = 255;
      }
    }
  */
  /*
    grad_2(result , image_data);
    name = "../raw/grad_2.raw";
    write_image(image_data , name);


    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        if(image_data[i][j] < 50){
          image_data[i][j] = 0;
        }
        else{
          image_data[i][j] = 255;
        }
      }
    }
  */
    /*
    unsigned char tmp[SIZE][SIZE];
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

    lapla_mask(result , image_data , 3000 , maks2, 1);
    name = "../raw/DOG_test.raw";
    write_image(image_data , name);
    */
  /*
    name = "../raw/test_threshold.raw";
    write_image(image_data , name);
  */
  /*
    dilation(image_data , tmp);
    erosion(tmp , image_data);

    name = "../raw/filling_hole.raw";
    //write_image(image_data , name);
    write_image(image_data , name);

    for (int i = 0; i < SIZE; i++){
      for(int j = 0 ; j < SIZE ; j++){
        if(image_data[i][j] == 0) result[i][j] = pow(result[i][j]/255.0 , 1.0/0.2)*255.0;
      }
    }

    name = "../raw/color_result.raw";
    write_image(result , name);  
  */

    for (int i = 0; i < SIZE; i++){
      for(int j = 0 ; j < SIZE ; j++){
        result[i][j] = pow(result[i][j]/255.0 , 1.0/0.2)*255.0;
        if(result[i][j] > 255) result[i][j] = 255;
        else if(result[i][j] < 0) result[i][j] = 0;
        if(result[i][j] < 15) result[i][j] = 0;
      }
    }

    float max_value = 0.0;
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        if (max_value < (float)result[i][j]/255){
          max_value = (float)result[i][j]/255;
        }
      }
    }
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        result[i][j] = round(255/(log(1 + max_value)) * log(1+(float)result[i][j]/255));
        if(result[i][j] > 255) result[i][j] = 255;
        if(result[i][j] < 0) result[i][j] = 0;
      }
    }
    printf("%f\n", 255/(log(1 + max_value)));
    
    /*
    dilation2(result , image_data);
    erosion2(image_data , result);
    */

    int hitogram_count[256] = {0};

    int cdf[256] = {0};
    int counter = 0;
    int cdf_min = 0 , flag = 1;
    int histo_H[256] = {0};
    histogram(result , hitogram_count);
    for(int i = 0 ; i < 256 ; i++){
      counter += hitogram_count[i];
      if(counter > 0 && flag){
        cdf_min = counter;
        flag = 0;
      }
      cdf[i] = counter;
    }
    
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        image_data[i][j] = round(((float)(cdf[result[i][j]] - cdf_min)/(float)(SIZE*SIZE-cdf_min))*255.0);
      }
    }

    name = "../raw/sample4.raw";
    readin_gray(result , name);
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        image_data[i][j] = (image_data[i][j] + result[i][j])/2;
        if(image_data[i][j] < 40 && j < 384) image_data[i][j] = 0;
        else if(image_data[i][j] < 45 && j >= 384) image_data[i][j] = 0;
      }
    }
    
    name = "../raw/color_result.raw";
    write_image(image_data , name);  
  
  /*
  unsigned char color_result[SIZE][SIZE];
  Gamma(result , image_data , color_result , 0.2);
  name = "../raw/test_color.raw";
  write_image(image_data , name);*/
/*
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

  lapla_mask(result , image_data , 3000 , mask1, 1);
  name = "../raw/LOG_test.raw";
  write_image(image_data , name);

  Nevatia_Babu(result , image_data , 12500);
  name = "../raw/Nevatia_Babu.raw";
  write_image(image_data , name);

  dilation(image_data , result);
  name = "../raw/dilated.raw";
  write_image(result , name);

  dilation(result , image_data);
  name = "../raw/dilated2.raw";
  write_image(image_data , name);
*/
  
  
/*
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = 255 - image_data[i][j];
    }
  }

  name = "../raw/inverse_thres.raw";
  write_image(result , name);
*/

  name = "../raw/sample5.raw";
  readin_gray(image_data , name);


  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = image_data[i][j];
    }
  }



  for (int i = 0; i < SIZE; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = pow(result[i][j]/255.0 , 1.0/0.2)*255.0;
      if(result[i][j] > 255) result[i][j] = 255;
      else if(result[i][j] < 0) result[i][j] = 0;
      if(result[i][j] < 10) result[i][j] = 0;
    }
  }

  max_value = 0.0;
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if (max_value < (float)result[i][j]/255){
        max_value = (float)result[i][j]/255;
      }
    }
  }
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      result[i][j] = round(255/(log(1 + max_value)) * log(1+(float)result[i][j]/255));
      if(result[i][j] > 255) result[i][j] = 255;
      if(result[i][j] < 0) result[i][j] = 0;
    }
  }
  
  
  for(int i = 0 ; i < 256 ; i++){ 
    hitogram_count[i] = 0;
    cdf[i] = 0;
  }

  counter = 0;
  cdf_min = 0 , flag = 1;
  histogram(result , hitogram_count);
  for(int i = 0 ; i < 256 ; i++){
    counter += hitogram_count[i];
    if(counter > 0 && flag){
      cdf_min = counter;
      flag = 0;
    }
    cdf[i] = counter;
  }
  
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      image_data[i][j] = round(((float)(cdf[result[i][j]] - cdf_min)/(float)(SIZE*SIZE-cdf_min))*255.0);
    }
  }

  name = "../raw/sample5.raw";
  readin_gray(result , name);
/*
  float contrast = -100;
  float factor = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      unsigned char newvalue = truncate((float)(factor * (result[i][j] - 128) + 128));
      result[i][j] = newvalue;
    }
  }
  name = "../raw/contrast.raw";
  write_image(result , name);
*/
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      image_data[i][j] = (image_data[i][j] + result[i][j])/2;
      //if(image_data[i][j] < 40 && j < 384) image_data[i][j] = 0;
      //else if(image_data[i][j] < 45 && j >= 384) image_data[i][j] = 0;
      if(image_data[i][j] < 21) image_data[i][j] = 0;
    }
  }
  
  name = "../raw/color_result2.raw";
  write_image(image_data , name);  

}