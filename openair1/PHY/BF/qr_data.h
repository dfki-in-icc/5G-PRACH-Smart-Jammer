#ifndef __QR_DATA_H
#define __QR_DATA_H

// C++
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>
#include <ccomplex>
// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    
    int row;
    int col;
    int col_cut = col/2;

    std::complex<double> *A;
    std::complex<double> *Q;
    std::complex<double> *R;


}qr_data;



typedef struct {
    
    int rowA;
    int rowB;
    int colB;
    //int row_test = 0;
    int row_count;

    std::complex<double> *matA;
    std::complex<double> *matB;
    std::complex<double> *matC;


}mat_data;




#endif