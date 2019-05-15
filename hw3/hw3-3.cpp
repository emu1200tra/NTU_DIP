#include "hw3-3.h"

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

void median_filter(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , int box_size1){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      vector<int>record;
      for(int k = -(box_size1/2) ; k <= box_size1/2 ; k++){
        for(int l = -(box_size1/2) ; l <= box_size1/2 ; l++){
          if(i+k < 0 || i+k > SIZE1-1 || j+l < 0 || j+l > SIZE1-1){
            continue;
          }
          record.push_back(image_data[i+k][j+l]);
        }
      }
      sort(record.begin() , record.end());
      int value = 0;
      int place = record.size()/2;
      if(record.size() % 2 == 0){
        value = (record[place-1] + record[place])/2;
      }
      else{
        value = record[place];
      }
      result[i][j] = value;
    }
  }
}

void box_filter(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , int box_size1){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      float counter = 0.0 , sum_pixel = 0.0;
      for(int k = -(box_size1/2) ; k <= box_size1/2 ; k++){
        for(int l = -(box_size1/2) ; l <= box_size1/2 ; l++){
          if(i+k < 0 || i+k > SIZE1-1 || j+l < 0 || j+l > SIZE1-1){
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

void dilation(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  /*
  int map_record[SIZE1+4][SIZE1+4];
  for(int i = 0 ; i < SIZE1+4 ; i++){
    for(int j = 0 ; j < SIZE1+4 ; j++){
      if(i < 2 || i > SIZE1+1 || j < 2 || j > SIZE1+1){
        map_record[i][j] = -1;
      }
      else{
        map_record[i][j] = image_data[i-2][j-2];
      }
    }
  }
  */
  unsigned char kernel[9][5] = {{0 , 0 , 1 , 0 , 0} , {0 , 1 , 1 , 1 , 0} , {0 , 1 , 1 , 1 , 0} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {0 , 1 , 1 , 1 , 0} , {0 , 1 , 1 , 1 , 0} , {0 , 0 , 1 , 0 , 0}};
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      /*
      int list[21] = {map_record[i+1][j] , map_record[i+3][j] , map_record[i+2][j] , map_record[i][j+1] , map_record[i+1][j+1] , map_record[i+2][j+1] ,
                      map_record[i+3][j+1] , map_record[i+4][j+1] , map_record[i][j+2] , map_record[i+1][j+2] , map_record[i+2][j+2] , map_record[i+3][j+2] ,
                      map_record[i+4][j+2] , map_record[i][j+3] , map_record[i+1][j+3] , map_record[i+2][j+3] , map_record[i+3][j+3] , map_record[i+4][j+3] ,
                      map_record[i+1][j+4] , map_record[i+3][j+4] , map_record[i+2][j+4]};
      */
      vector<int> list;
      for(int k = -4 ; k < 5 ; k++){
        for(int l = -2 ; l < 3 ; l++){
          if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1 && kernel[k+4][l+2] == 1){
            list.push_back(image_data[i+k][j+l]);
          }
        }
      }
      sort(list.begin(), list.end());
      result[i][j] = list.back();
    }
  }
}

void erosion(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  /*
  int map_record[SIZE1+4][SIZE1+4];
  for(int i = 0 ; i < SIZE1+4 ; i++){
    for(int j = 0 ; j < SIZE1+4 ; j++){
      if(i < 2 || i > SIZE1+1 || j < 2 || j > SIZE1+1){
        map_record[i][j] = 256;
      }
      else{
        map_record[i][j] = image_data[i-2][j-2];
      }
    }
  }
  */
  unsigned char kernel[9][5] = {{0 , 0 , 1 , 0 , 0} , {0 , 1 , 1 , 1 , 0} , {0 , 1 , 1 , 1 , 0} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {1 , 1 , 1 , 1 , 1} , {0 , 1 , 1 , 1 , 0} , {0 , 1 , 1 , 1 , 0} , {0 , 0 , 1 , 0 , 0}};
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      /*
      int list[21] = {map_record[i+1][j] , map_record[i+3][j] , map_record[i+2][j] , map_record[i][j+1] , map_record[i+1][j+1] , map_record[i+2][j+1] ,
                      map_record[i+3][j+1] , map_record[i+4][j+1] , map_record[i][j+2] , map_record[i+1][j+2] , map_record[i+2][j+2] , map_record[i+3][j+2] ,
                      map_record[i+4][j+2] , map_record[i][j+3] , map_record[i+1][j+3] , map_record[i+2][j+3] , map_record[i+3][j+3] , map_record[i+4][j+3] ,
                      map_record[i+1][j+4] , map_record[i+3][j+4] , map_record[i+2][j+4]};
      */
      vector<int> list;
      for(int k = -4 ; k < 5 ; k++){
        for(int l = -2 ; l < 3 ; l++){
          if(i+k >= 0 && i+k < SIZE1 && j+l >= 0 && j+l < SIZE1 && kernel[k+4][l+2] == 1){
            list.push_back(image_data[i+k][j+l]);
          }
        }
      }
      sort(list.begin(), list.end());
      result[i][j] = list[0];
    }
  }
}

void opening(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  unsigned char tmp[SIZE1][SIZE1];
  erosion(image , tmp);
  dilation(tmp , result);
}

void closing(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  unsigned char tmp[SIZE1][SIZE1];
  dilation(image , tmp);
  erosion(tmp , result);
}

void histogram(unsigned char image_data[SIZE1][SIZE1] , int histo[256]){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      histo[image_data[i][j]]++;
    }
  }
}

void changing(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]){
  for(int i = 0 ; i < SIZE1 ; i++){
    for(int j = 0 ; j < SIZE1 ; j++){
      result[i][j] = image[i][j];
    }
  }
}