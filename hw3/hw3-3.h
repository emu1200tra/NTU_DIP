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

void dilation(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void erosion(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void opening(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void closing(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);

void readin_gray(unsigned char image_data[SIZE1][SIZE1] , string name);

void write_image(unsigned char result[SIZE1][SIZE1] , string name);

void median_filter(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , int box_size1);

void box_filter(unsigned char image_data[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1] , int box_size1);

void histogram(unsigned char image_data[SIZE1][SIZE1] , int histo[256]);

void changing(unsigned char image[SIZE1][SIZE1] , unsigned char result[SIZE1][SIZE1]);