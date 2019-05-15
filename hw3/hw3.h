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

#define SIZE1 256
#define SIZE2 512
using namespace std;

void readin_gray(unsigned char image_data[SIZE1][SIZE1] , string name);

void write_image(unsigned char result[SIZE1][SIZE1] , string name);

void erosion(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h);

void bound_extract(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h);

void dilation(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , unsigned char kernel[3][3] , int w , int h);

void invsserse(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

int labeling(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void recurssion(unsigned char image[SIZE1][SIZE1] , unsigned char label[SIZE1][SIZE1] , int label_value , int x , int y);

void diff(unsigned char image1[SIZE1][SIZE1] , unsigned char image2[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void uni(unsigned char image1[SIZE1][SIZE1] , unsigned char image2[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void opening(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void skeleton(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

