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

void readin_gray_big(unsigned char image_data[SIZE2][SIZE2] , string name);

void write_image_big(unsigned char result[SIZE2][SIZE2] , string name);

void convolution(unsigned char image[SIZE2][SIZE2] , int result[SIZE2][SIZE2] , int kernel[3][3] , float k);

int energy(int image[SIZE2][SIZE2] , int result[SIZE2][SIZE2]);

float distance_my(float x[9] , float y[9]);

void kmean(unsigned char label[SIZE2][SIZE2]);

void exchange(unsigned char image[SIZE2][SIZE2] , unsigned char label[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2]);

void filling(unsigned char image[SIZE2][SIZE2] , bool label[SIZE2][SIZE2] , int size , int counter , int *sign , int x , int y);

void hole_filling(unsigned char image[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2] , int threshold , int size);

void laws(unsigned char image[SIZE2][SIZE2] , unsigned char result[SIZE2][SIZE2]);