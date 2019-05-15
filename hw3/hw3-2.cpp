#include "hw3-2.h"

#define SIZE1 256
#define SIZE2 512
using namespace std;

int energy_plot[9][SIZE2][SIZE2] = {0};

void readin_gray_big(unsigned char image_data[SIZE2][SIZE2] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "rb"))){
    printf("Cannot open file! :%s\n" , name.c_str());
    exit(1);
  }
  fread(image_data, sizeof(unsigned char), SIZE2* SIZE2, file);
  fclose(file);
}

void write_image_big(unsigned char result[SIZE2][SIZE2] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "wb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fwrite(result, sizeof(unsigned char), SIZE2* SIZE2, file);
  fclose(file);
}

void convolution(unsigned char image[SIZE2][SIZE2] , int result[SIZE2][SIZE2] , int kernel[3][3] , float k){
  unsigned char big_image[SIZE2+2][SIZE2+2] = {0};
  for(int i = 0 ; i < SIZE2+2 ; i++){
    for(int j = 0 ; j < SIZE2+2 ; j++){
      if (i == 0){
        if(j >= 1 && j <= SIZE2){
          big_image[i][j] = image[i][j-1];
        }
        else if(j == 0){
          big_image[i][j] = image[i][j];
        }
        else{
          big_image[i][j] = image[i][SIZE2-1];
        }
      }
      else if(i == SIZE2+1){
        if(j >= 1 && j <= SIZE2){
          big_image[i][j] = image[i-2][j-1];
        }
        else if(j == 0){
          big_image[i][j] = image[i-2][j];
        }
        else{
          big_image[i][j] = image[i-2][SIZE2-1];
        }
      }
      else if(j == 0){
        if(i >= 1 && i <= SIZE2){
          big_image[i][j] = image[i-1][j];
        }       
      }
      else if(j == SIZE2+1){
        if(i >= 1 && i <= SIZE2){
          big_image[i][j] = image[i-1][j-2];
        }
      }
      else{
        big_image[i][j] = image[i][j];
      }
    }
  }
  for(int i = 1 ; i <= SIZE2 ; i++){
    for(int j = 1 ; j <= SIZE2 ; j++){
      float counter = 0.0;
      for(int k = -1 ; k < 2 ; k++){
        for(int l = -1 ; l < 2 ; l++){
          counter += big_image[i+k][j+l] * kernel[k+1][l+1];
        }
      }
      result[i-1][j-1] = round(k*counter);
      //printf("result:%d k:%f counter:%f\n", result[i-1][j-1] , k , counter);
    }
  }
}

int energy(int image[SIZE2][SIZE2] , int result[SIZE2][SIZE2]){
  int big_image[SIZE2+12][SIZE2+12] = {0};
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < 6 ; j++){
      big_image[i+6][j] = image[i][5-j];
      big_image[i+6][SIZE2+11-j] = image[i][SIZE2-6+j];
      big_image[j][i+6] = image[5-j][i];
      big_image[SIZE2+11-j][i+6] = image[SIZE2-6+j][i];
      if(i < 6){
        big_image[i][j] = image[5-i][5-j];
        big_image[i][SIZE2+6+j] = image[5-i][SIZE2-1-j];
        big_image[SIZE2+6+i][j] = image[SIZE2-1-i][5-j];
        big_image[SIZE2+6+i][SIZE2+6+j] = image[SIZE2-1-i][SIZE2-1-j];
      }
    }
    for(int j = 0 ; j < SIZE2 ; j++){
      big_image[i+6][j+6] = image[i][j];
    }
  }
  int min_value = 21473640;
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      float counter = 0.0;
      for(int k = -6 ; k < 7 ; k++){
        for(int l = -6 ; l < 7 ; l++){
          counter += pow(big_image[i+6+k][j+6+l] , 2);
        }
      }
      result[i][j] = counter;
      if(min_value > counter){
        min_value = counter;
      }
    }
  }
  return min_value;
}

float distance_my(float x[9] , float y[9]){
  float counter = 0.0;
  for(int i = 0 ; i < 9 ; i++){
    counter += pow(x[i]-y[i] , 2);
    //printf("x:%f , y:%f , counter:%f\n", x[i] , y[i] , counter);
  }
  return sqrt(counter);
}

void kmean(unsigned char label[SIZE2][SIZE2]){
  float mean[3][9] = {0};
  for(int i = 0 ; i < 9 ; i++){
    mean[0][i] = energy_plot[i][0][0];
    mean[1][i] = energy_plot[i][255][255];
    mean[2][i] = energy_plot[i][511][511];
  }
  bool sign = true;
  while(sign){
    //stage 1
    sign = false;
    for(int i = 0 ; i < SIZE2 ; i++){
      for(int j = 0 ; j < SIZE2 ; j++){
        float feature[9] = {0};
        for(int k = 0 ; k < 9 ; k++){
          feature[k] = energy_plot[k][i][j];
        }
        float d1 = distance_my(feature , mean[0]);
        float d2 = distance_my(feature , mean[1]);
        float d3 = distance_my(feature , mean[2]);
        //printf("d1:%f , d2:%f , d3:%f\n", d1 , d2 , d3);
        if(d1 <= d2 && d1 <= d3 && label[i][j] != 1){
          label[i][j] = 1;
          sign = true;
        }
        else if(d2 <= d1 && d2 <= d3 && label[i][j] != 2){
          label[i][j] = 2;
          sign = true;
        }
        else if(d3 <= d1 && d3 <= d2 && label[i][j] != 3){
          label[i][j] = 3;
          sign = true;
        }
      }
    }
    if(sign == false) break;
    //stage 2
    float sum_up[3][9] = {0};
    float counter[3] = {0};
    for(int i = 0 ; i < SIZE2 ; i++){
      for(int j = 0 ; j < SIZE2 ; j++){
        float feature[9] = {0};
        for(int k = 0 ; k < 9 ; k++){
          feature[k] = energy_plot[k][i][j];
        }
        counter[label[i][j]-1]++;
        for(int k = 0 ; k < 9 ; k++){
          sum_up[label[i][j]-1][k] += feature[k];
        }
      }
    }
    for(int i = 0 ; i < 9 ; i++){
      mean[0][i] = sum_up[0][i] / counter[0];
      mean[1][i] = sum_up[1][i] / counter[1];
      mean[2][i] = sum_up[2][i] / counter[2];
    }
  }
}

void filling(unsigned char image[SIZE2][SIZE2] , bool label[SIZE2][SIZE2] , int size , int counter , int *sign , int x , int y){
  label[x][y] = true;
  counter++;
  //printf("into...\n");
  if(image[x+1][y] == 255 && label[x+1][y] == 0 && x+1 < SIZE2){
    if(counter < size*size) filling(image , label , size , counter , sign , x+1 , y);
    else *sign = 0;
  }
  if(image[x-1][y] == 255 && label[x-1][y] == 0 && x-1 >= 0){
    if(counter < size*size) filling(image , label , size , counter , sign , x-1 , y);
    else *sign = 0;
  }
  if(image[x][y+1] == 255 && label[x][y+1] == 0 && y+1 < SIZE2){
    if(counter < size*size) filling(image , label , size , counter , sign , x , y+1);
    else *sign = 0;
  }
  if(image[x][y-1] == 255 && label[x][y-1] == 0 && y-1 >= 0){
    if(counter < size*size) filling(image , label , size , counter , sign , x , y-1);
    else *sign = 0;
  }
  if(image[x+1][y+1] == 255 && label[x+1][y+1] == 0 && x+1 < SIZE2 && y+1 < SIZE2){
   if(counter < size*size) filling(image , label , size , counter , sign , x+1 , y+1);
   else *sign = 0;
  }
  if(image[x-1][y+1] == 255 && label[x-1][y+1] == 0 && x-1 >= 0 && y+1 < SIZE2){
    if(counter < size*size) filling(image , label , size , counter , sign , x-1 , y+1);
    else *sign = 0;
  }
  if(image[x+1][y-1] == 255 && label[x+1][y-1] == 0 && x+1 < SIZE2 && y-1 >=0){
    if(counter < size*size) filling(image , label , size , counter , sign , x+1 , y-1);
    else *sign = 0;
  }
  if(image[x-1][y-1] == 255 && label[x-1][y-1] == 0 && x-1 >= 0 && y-1 >= 0){
    if(counter < size*size) filling(image , label , size , counter , sign , x-1 , y-1);
    else *sign = 0;
  }
}

void hole_filling(unsigned char image[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2] , int threshold , int size){
  unsigned char binary[SIZE2][SIZE2] = {0};
  bool label[SIZE2][SIZE2] = {0};
  bool record[SIZE2][SIZE2] = {0};
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      if(image[i][j] >= threshold) binary[i][j] = 255;
      result[i][j] = image[i][j];
    }
  }
  int sign = 1;
  //filling(unsigned char image[SIZE2][SIZE2] , bool label[SIZE2][SIZE2] , int size , int counter , int *sign , int x , int y)
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      //printf("filling...%d %d\n" , i , j);
      if(binary[i][j] == 255 && record[i][j] == 0){
        filling(binary , label , size , 0 , &sign , i , j);
      }
      if(sign == 1){
        //printf("filling..\n");
        for(int k = 0 ; k < SIZE2 ; k++){
          for(int l = 0 ; l < SIZE2 ; l++){
            if(label[k][l] == true){
              //printf("put in pixel\n");
              result[k][l] = 100;
              record[k][l] = true;
              label[k][l] = false;
            }
          }
        }
      }
      else{
        sign = 1;
        for(int k = 0 ; k < SIZE2 ; k++){
          for(int l = 0 ; l < SIZE2 ; l++){
            if(label[k][l] == true){
              record[k][l] = true;
              label[k][l] = false;
            }
          }
        }
      }
    }
  }
}


void laws(unsigned char image[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2]){
  //printf("into function\n");
  unsigned char label_result[SIZE2][SIZE2] = {0};
  int kernels[9][3][3] = {{{1 , 2 , 1} , {2 , 4 , 2} , {1 , 2 , 1}} , {{1 , 0 , -1} , {2 , 0 , -2} , {1 , 0 , -1}} , {{-1 , 2 , -1} , {-2 , 4 , -2} , {-1 , 2 , -1}} ,
                          {{-1 , -2 , -1} , {0 , 0 , 0} , {1 , 2 , 1}} , {{1 , 0 , -1} , {0 , 0 , 0} , {-1 , 0 , 1}} , {{-1 , 2 , -1} , {0 , 0 , 0} , {1 , -2 , 1}} , 
                          {{-1 , -2 , -1} , {2 , 4 , 2} , {-1 , -2 , -1}} , {{-1 , 0 , 1} , {2 , 0 , -2} , {-1 , 0 , 1}} , {{1 , -2 , 1} , {-2 , 4 , -2} , {1 , -2 , 1}}};
  float ks[9] = {1.0/36.0 , 1.0/12.0 , 1.0/12.0 , 1.0/12.0 , 1.0/4.0 , 1.0/4.0 , 1.0/12.0 , 1.0/4.0 , 1.0/4.0};
  int result_channels[SIZE2][SIZE2] = {0};
  int energy_result[SIZE2][SIZE2] = {0};
  //unsigned char energy_plot[9][SIZE2][SIZE2] = {0};
  //printf("start laws\n");
  for(int i = 0 ; i < 9 ; i++){
    int min_value = 0;
    convolution(image , result_channels , kernels[i] , ks[i]);
    min_value = energy(result_channels , energy_result);
    for(int j = 0 ; j < SIZE2 ; j++){
      for(int k = 0 ; k < SIZE2 ; k++){
        //energy_result[j][k] /= min_value;
        energy_plot[i][j][k] = energy_result[j][k];
      }
    }
    //write_image_big(energy_plot[i] , "./raw/energy_" + to_string(i) + ".raw");
  }
  kmean(label_result);
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      label_result[i][j] = (label_result[i][j]-1)*100;
    }
  }
  hole_filling(label_result , result , 128 , 30);
}

void exchange(unsigned char image[SIZE2][SIZE2] , unsigned char label[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2]){
  unsigned char pattern1[95][105] = {0};
  for(int i = 0 ; i < 105 ; i++){
    for(int j = 0 ; j < 95 ; j++){
      pattern1[j][i] = image[j][i];
    }
  }
  //305 - 435 1 - 65
  unsigned char pattern2[65][130] = {0};
  for(int i = 0 ; i < 130 ; i++){
    for(int j = 0 ; j < 65 ; j++){
      pattern2[j][i] = image[j][305+i];
    }
  }
  //442 - 512 462 - 512
  unsigned char pattern3[50][70] = {0};
  for(int i = 0 ; i < 70 ; i++){
    for(int j = 0 ; j < 50 ; j++){
      pattern3[j][i] = image[462+j][442+i];
    }
  }
  bool change_record[SIZE2][SIZE2] = {0};
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      change_record[i][j] = false;
    }
  }
  for(int i = 0 ; i < SIZE2 ; i++){
    for(int j = 0 ; j < SIZE2 ; j++){
      if(label[i][j] == 0 && change_record[i][j] == false){
        for(int k = 0 ; k < 65 ; k++){
          for(int l = 0 ; l < 130 ; l++){
            if(i+k < SIZE2 && j+l < SIZE2 && label[i+k][j+l] == 0 && change_record[i+k][j+l] == false){
              change_record[i+k][j+l] = true;
              result[i+k][j+l] = pattern2[k][l];
            }
          }
        }
      }
      else if(label[i][j] == 100 && change_record[i][j] == false){
        for(int k = 0 ; k < 50 ; k++){
          for(int l = 0 ; l < 70 ; l++){
            if(i+k < SIZE2 && j+l < SIZE2 && label[i+k][j+l] == 100 && change_record[i+k][j+l] == false){
              change_record[i+k][j+l] = true;
              result[i+k][j+l] = pattern3[k][l];
            }
          }
        }
      }
      else if(label[i][j] == 200 && change_record[i][j] == false){
        for(int k = 0 ; k < 95 ; k++){
          for(int l = 0 ; l < 105 ; l++){
            if(i+k < SIZE2 && j+l < SIZE2 && label[i+k][j+l] == 200 && change_record[i+k][j+l] == false){
              change_record[i+k][j+l] = true;
              result[i+k][j+l] = pattern1[k][l];
            }
          }
        }
      }
    }
  }
}