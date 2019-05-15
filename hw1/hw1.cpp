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

#define SIZE 256
using namespace std;

void readin_RGB(unsigned char image_data[3][SIZE][SIZE] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "rb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fread(image_data, sizeof(unsigned char), SIZE* SIZE * 3, file);
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

void readin_gray(unsigned char image_data[SIZE][SIZE] , string name){
  FILE *file;
  if (!(file = fopen(name.c_str(), "rb"))){
    printf("Cannot open file!\n");
    exit(1);
  }
  fread(image_data, sizeof(unsigned char), SIZE* SIZE, file);
  fclose(file);
}

void histogram(unsigned char image_data[SIZE][SIZE] , int histo[256]){
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      histo[image_data[i][j]]++;
    }
  }
}

void write_out_histo(int histo[256] , string name){
  string tmp = "\0";
  for(int i = 0 ; i < 256 ; i++){
    tmp = tmp + to_string(i) + '\t' + to_string(histo[i]) + '\t';
    for(int j = 0 ; j < histo[i]/4 ; j++){
      tmp = tmp + '-';
    }
    tmp = tmp + "\r\n";
  }
  ofstream output(name.c_str());
  output << tmp;
}

double PSNR(unsigned char original[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
  double MSE = 0.0;
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      MSE += pow((original[i][j] - result[i][j]) , 2);
    }
  }
  MSE = MSE / (SIZE*SIZE);
  return 10.0*log(255.0*255.0/MSE);
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

void median_filter(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE] , int box_size){
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      vector<int>record;
      for(int k = -(box_size/2) ; k <= box_size/2 ; k++){
        for(int l = -(box_size/2) ; l <= box_size/2 ; l++){
          if(i+k < 0 || i+k > SIZE-1 || j+l < 0 || j+l > SIZE-1){
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

void dilation(unsigned char image_data[SIZE][SIZE] , unsigned char result[SIZE][SIZE]){
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

int main(void)
{
  // read in image
  unsigned char image_data[3][SIZE][SIZE];
  string name = "../raw/sample1.raw";
  readin_RGB(image_data , name);

  // warm up: RGB to grayscale
  unsigned char gray_result[SIZE][SIZE];
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      gray_result[j][i] = round(0.2989 * (float)image_data[0][i][j] + 0.5870 * (float)image_data[1][i][j] + 0.1140 * (float)image_data[2][i][j]);
    }
  }

  // write out image
  name = "../raw/B.raw";
  write_image(gray_result , name);

  // problem 1
  // decrease brightness
  unsigned char image_data2[SIZE][SIZE];
  name = "../raw/sample2.raw";
  readin_gray(image_data2 , name);

  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      gray_result[i][j] = image_data2[i][j] / 3;
    }
  }

  name = "../raw/D.raw";
  write_image(gray_result , name);

  // count for histogram
  int count_sample2[256] = {0};
  int count_D[256] = {0};
  histogram(image_data2 , count_sample2);
  histogram(gray_result , count_D);
  name = "sample2_histogram.txt";
  write_out_histo(count_sample2 , name);
  name = "D_histogram.txt";
  write_out_histo(count_D , name);
  
  //histo equal
  int cdf[256] = {0};
  int counter = 0;
  int cdf_min = 0 , flag = 1;
  int histo_H[256] = {0};
  for(int i = 0 ; i < 256 ; i++){
    counter += count_D[i];
    if(counter > 0 && flag){
      cdf_min = counter;
      flag = 0;
    }
    cdf[i] = counter;
  }
  unsigned char HE_result[SIZE][SIZE];
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      HE_result[i][j] = round(((float)(cdf[gray_result[i][j]] - cdf_min)/(float)(SIZE*SIZE-cdf_min))*255.0);
      histo_H[HE_result[i][j]]++;
    }
  }
  name = "../raw/H.raw";
  write_image(HE_result , name);

  //local histo equal
  int histo_L[256] = {0};
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      counter = 0;
      int county = 0 , histo_local[256] = {0};
      for(int x = -4 ; x < 5 ; x++){
        for(int y = -4 ; y < 5 ; y++){
          if((i + x) < 0 || (i + x) > (SIZE-1) || (j + y) < 0 || (j + y) > (SIZE-1)){
            continue;
          }
          county++;
          histo_local[gray_result[i+x][j+y]]++;
        }
      }
      for(int k = 0 ; k < gray_result[i][j] ; k++){
        counter += histo_local[k];
      }
      HE_result[i][j] = round(((float)(counter)/(float)(county))*255.0);
      histo_L[HE_result[i][j]]++;
    }
  }
  name = "../raw/L.raw";
  write_image(HE_result , name);

  //histogram for H and L
  name = "H_histogram.txt";
  write_out_histo(histo_H , name);
  name = "L_histogram.txt";
  write_out_histo(histo_L , name);

  // log transform
  unsigned char log_result[SIZE][SIZE];
  float max_value = 0.0;
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      if (max_value < (float)gray_result[i][j]/255){
        max_value = (float)gray_result[i][j]/255;
      }
    }
  }
  double c[3] = {100 , 1000 , 255/(log(1 + max_value))};
  for(int index = 0 ; index < 3 ; index++){
    int histo_log_trans[256] = {0};
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        log_result[i][j] = round(c[index] * log(1+(float)gray_result[i][j]/255));
        histo_log_trans[log_result[i][j]]++;
      }
    }
    name = "../raw/log_trans_" + to_string(c[index]) + ".raw";
    write_image(log_result , name);
    name = "log_trans_" + to_string(c[index]) + "_histogram.txt";
    write_out_histo(histo_log_trans , name);
  }

  // inverse log transform
  unsigned char inv_log_result[SIZE][SIZE];
  c[2] = 255/(exp(max_value)-1);
  for(int index = 0 ; index < 3 ; index++){
    int histo_invlog_trans[256] = {0};
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        inv_log_result[i][j] = round(c[index] * (exp((float)gray_result[i][j]/255.0) - 1));
        histo_invlog_trans[inv_log_result[i][j]]++;
      }
    }
    name = "../raw/invlog_" + to_string(c[index]) + "_trans.raw";
    write_image(inv_log_result , name);
    name = "invlog_trans_" + to_string(c[index]) + "_histogram.txt";
    write_out_histo(histo_invlog_trans , name);
  }
  
  // power law transform
  unsigned char power_law_transform[SIZE][SIZE];
  float r[3] = {0.5 , 2 , 3};
  for(int index1 = 0 ; index1 < 3 ; index1++){
    c[2] = 255/(pow(max_value , r[index1]));
    c[0] = 10000;
    for(int index2 = 0 ; index2 < 3 ; index2++){
      int histo_powerlaw_trans[256] = {0};
      for(int i = 0 ; i < SIZE ; i++){
        for(int j = 0 ; j < SIZE ; j++){
          power_law_transform[i][j] = round(c[index2] * pow((float)gray_result[i][j]/255 , r[index1]));
          histo_powerlaw_trans[power_law_transform[i][j]]++;
        }
      }
      name = "../raw/powerlaw_trans_" + to_string(r[index1]) + "_" + to_string(c[index2]) + ".raw";
      write_image(power_law_transform , name);
      name = "powerlaw_trans_" + to_string(r[index1]) + "_" + to_string(c[index2]) + "_histogram.txt";
      write_out_histo(histo_powerlaw_trans , name);
    }
  }
  
  // generate gaussian noise
  unsigned char image_data3[SIZE][SIZE];
  name = "../raw/sample3.raw";
  readin_gray(image_data3 , name);

  srand(time(NULL));
  float amp[2] = {10.0 , 30.0};

  random_device rd; 
  mt19937 gen(rd()); 
  normal_distribution<float> d(0.0, 1.0); 
     
  for(int index = 0 ; index < 2 ; index++){
    unsigned char gaussian_result[SIZE][SIZE];
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        float ran = d(gen);
        int value = image_data3[i][j] + round(amp[index] * ran);
        if(value >= 255){
          value = 255;
        }
        if(value <= 0){
          value = 0;
        }
        gaussian_result[i][j] = value;
      }
    }
    name = "../raw/G" + to_string((index+1)) + ".raw";
    write_image(gaussian_result , name);
  }

  //generate salt and pepper noise
  float thres[3] = {0.05 , 0.1};
  uniform_real_distribution<> dis(0.0, 1.0);
  for(int index = 0 ; index < 2 ; index++){
    unsigned char saltandpepper_result[SIZE][SIZE];
    for(int i = 0 ; i < SIZE ; i++){
      for(int j = 0 ; j < SIZE ; j++){
        float value = dis(gen);
        if(value < thres[index]){
          saltandpepper_result[i][j] = 0;
        }
        else if(value > (1-thres[index])){
          saltandpepper_result[i][j] = 255;
        }
        else{
          saltandpepper_result[i][j] = image_data3[i][j];
        }
      }
    }
    name = "../raw/S" + to_string((index+1)) + ".raw";
    write_image(saltandpepper_result , name);
  }

  //remove noise
  //box filter
  unsigned char gaussian_result[SIZE][SIZE];
  name = "../raw/G1.raw";
  readin_gray(gaussian_result , name);

  unsigned char remove_gaussian[SIZE][SIZE];
  box_filter(gaussian_result , remove_gaussian , 3);
  name = "../raw/RG.raw";
  write_image(remove_gaussian , name);

  // median filter
  unsigned char saltandpepper_result[SIZE][SIZE];
  name = "../raw/S1.raw";
  readin_gray(saltandpepper_result , name);

  unsigned char remove_salt[SIZE][SIZE];
  median_filter(saltandpepper_result , remove_salt , 3);
  name = "../raw/RS.raw";
  write_image(remove_salt , name);

  //PSNR
  double PSNR_noise_clean_RG = PSNR(gaussian_result , remove_gaussian);
  cout << "PSNR between G1 and RG: " << PSNR_noise_clean_RG << endl;
  double PSNR_original_clean_RG = PSNR(image_data3 , remove_gaussian);
  cout << "PSNR between sample3 and RG: " << PSNR_original_clean_RG << endl;

  double PSNR_noise_clean_RS = PSNR(saltandpepper_result , remove_salt);
  cout << "PSNR between S1 and RS: " << PSNR_noise_clean_RS << endl;
  double PSNR_original_clean_RS = PSNR(image_data3 , remove_salt);
  cout << "PSNR between sample3 and RS: " << PSNR_original_clean_RS << endl;
  
  // remove wrinkle
  unsigned char image_data4[SIZE][SIZE];
  name = "../raw/sample4.raw";
  readin_gray(image_data4 , name);
  // // box filter for 3*3 and 5*5
  int filter[2] = {3 , 5};
  for(int index = 0 ; index < 2 ; index++){
    unsigned char box_filter_result[SIZE][SIZE];
    box_filter(image_data4 , box_filter_result , filter[index]);
    name = "../raw/rm_wrinkle_box_filter_" + to_string(filter[index]) + ".raw";
    write_image(box_filter_result , name);
  }
  // // median filter for 3*3 and 5*5
  for(int index = 0 ; index < 2 ; index++){
    unsigned char median_filter_result[SIZE][SIZE];
    box_filter(image_data4 , median_filter_result , filter[index]);
    name = "../raw/rm_wrinkle_median_filter_" + to_string(filter[index]) + ".raw";
    write_image(median_filter_result , name);
  }
  // // dilation
  unsigned char dilation_result[SIZE][SIZE];
  dilation(image_data4 , dilation_result);
  name = "../raw/rm_wrinkle_dilation.raw";
  write_image(dilation_result , name);
  
  // // opening
  unsigned char opening_result[SIZE][SIZE];
  dilation(erosion_result , opening_result);
  name = "../raw/rm_wrinkle_opening.raw";
  write_image(opening_result , name);

  //testing 
  unsigned char test[SIZE][SIZE];
  dilation(opening_result , test);
  name = "../raw/test.raw";
  write_image(test , name);

  //testing again
  unsigned char test2[SIZE][SIZE];
  dilation(test , test2);
  name = "../raw/test2.raw";
  write_image(test2 , name);

  unsigned char test3[SIZE][SIZE];
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      test3[i][j] = (test2[i][j] + image_data4[i][j])/2;
    }
  }
  name = "../raw/test3.raw";
  write_image(test3 , name);

  // testing dilation again then adding origianl image
  unsigned char test4[SIZE][SIZE];
  dilation(test2 , test4);
  dilation(test4 , test2);
  dilation(test2 , test4);
  dilation(test4 , test2);
  for(int i = 0 ; i < SIZE ; i++){
    for(int j = 0 ; j < SIZE ; j++){
      test3[i][j] = (test2[i][j] + image_data4[i][j])/2;
    }
  }
  name = "../raw/test4.raw";
  write_image(test3 , name);


  exit(0);
}

