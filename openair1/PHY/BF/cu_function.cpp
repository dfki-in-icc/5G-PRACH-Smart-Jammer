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

#include "cu_function.h"
#include "qr_data.h"
#include <immintrin.h>

// #include "matplotlib-cpp/matplotlibcpp.h"

#define PI acos(-1)
#define BLOCK_SIZE 16
#define PRINT_RESULT 1
//#define PLOT_RESULT 0


using namespace std::literals::complex_literals;
const std::complex<double> I_1(0, 1);
//using namespace std::complex_literals;
// namespace plt = matplotlibcpp;


// print complex matrix matlab
void print_complex_matrix_matlab(std::complex<double> *matA, int rowA, int colA) {
    std::cout << "[";
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colA; ++j) {
            std::cout << std::setprecision(16) << matA[i * colA + j].real() << "+" << matA[i * colA + j].imag() << "i ";
        }
        std::cout << ";" << std::endl;
    }
    std::cout << "]" << std::endl;
}


// print complex matrix
void print_complex_matrix(std::complex<double> *matA, int rowA, int colA) {
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colA; ++j) {
            std::cout << std::fixed << std::setprecision(6) << std::setw(27) << matA[i * colA + j] << " ";
            
        }
        std::cout << std::endl;
    }
}


// generate random number with normal_distribution
std::complex<double> randn() {
    std::random_device randomness_device{};
    std::mt19937 pseudorandom_generator{randomness_device()};
    auto mean = 0.0;
    auto std_dev = 1.0;
    std::normal_distribution<> distribution{mean, std_dev};
    auto sample = distribution(pseudorandom_generator);
    return (std::complex<double>)(sample);
}

// add white gaussian noise
void awgn(std::complex<double> *input_signal, std::complex<double> *output_signal, int snr, int row, int col) {
    std::complex<double> Esym;
    std::complex<double> No;
    std::complex<double> noiseSigma;
    std::complex<double> n;
    for(int i = 0; i < row * col; i++) {
        Esym += pow(abs(input_signal[i]), 2) / std::complex<double>(row * col);
        No = Esym / std::complex<double>(snr);
        noiseSigma = sqrt(No / std::complex<double>(2));
        n = noiseSigma * (randn() + randn() * 1i);
        output_signal[i] = input_signal[i] + n;
    }
}


// complex matrix addition
void complex_matrix_addition(std::complex<double> *matA, std::complex<double> *matB, int rowA, int colA) {
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colA; ++j) {
            matA[i * colA + j].real(matA[i * colA + j].real() + matB[i * colA + j].real());
            matA[i * colA + j].imag(matA[i * colA + j].imag() + matB[i * colA + j].imag());
        }
    }
}


// complex matrix subtraction
void complex_matrix_subtraction(std::complex<double> *matA, std::complex<double> *matB, int rowA, int colA) {
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colA; ++j) {
            matA[i * colA + j].real(matA[i * colA + j].real() - matB[i * colA + j].real());
            matA[i * colA + j].imag(matA[i * colA + j].imag() - matB[i * colA + j].imag());
        }
    }
}


// complex matrix multiplication
void complex_matrix_multiplication(std::complex<double> *matA, std::complex<double> *matB, std::complex<double> *matC, int rowA, int rowB, int colB) {
    memset(matC, 0, rowA * colB * sizeof(std::complex<double>));
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colB; ++j) {
            for(int k = 0; k < rowB; ++k) {
                matC[i * colB + j] += matA[i * rowB + k] * matB[k * colB + j];
            }
        }
    }
}


// get complex matrix by column
void complex_matrix_get_columns(std::complex<double> *matA, std::complex<double> *matCol, int rowA, int colA, int colTarget) {
    for(int i = 0; i < rowA; ++i) {
        matCol[i] = matA[i * colA + colTarget];
    }
}


// get complex matrix by row
void complex_matrix_get_rows(std::complex<double> *matA, std::complex<double> *matRow, int rowA, int colA, int rowTarget) {
    for(int i = 0; i < colA; ++i) {
        matRow[i] = matA[rowTarget * colA + i];
    }
}


// complex matrix conjugate transpose
void complex_matrix_conjugate_transpose(std::complex<double> *matA, int rowA, int colA) {
    std::complex<double> *temp = (std::complex<double>*)malloc(colA * rowA * sizeof(std::complex<double>));
    memcpy(temp, matA, (rowA * colA * sizeof(std::complex<double>)));
    for(int i = 0; i < rowA; ++i) {
        for(int j = 0; j < colA; ++j) {
            matA[j * rowA + i].real(temp[i * colA + j].real());
            matA[j * rowA + i].imag(-temp[i * colA + j].imag());
        }
    }
    free(temp);
}


// complex matrix conjugate transpose and multiplication
void complex_matrix_conjugate_transpose_multiplication(std::complex<double> *matA, std::complex<double> *matB, int rowA, int colA) {
    std::complex<double> *temp = (std::complex<double>*)malloc(colA * rowA * sizeof(std::complex<double>));
    memcpy(temp, matA, (rowA * colA * sizeof(std::complex<double>)));
    complex_matrix_conjugate_transpose(temp, rowA, colA);
    complex_matrix_multiplication(matA, temp, matB, rowA, colA, rowA);
    free(temp);
}


// compute Pn (pseudo-noise): matlab code: (Pn=Pn+vet_noise(:,ii)*vet_noise(:,ii)';), where (ii=1:length(vet_noise(1,:)))
void compute_Pn(std::complex<double> *Pn, std::complex<double> *vet_noise, int M, int len_t_theta) {
    std::complex<double> *vet_noise_temp = (std::complex<double>*)malloc(M * sizeof(std::complex<double>));
    std::complex<double> *Pn_temp = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    for(int i = 0; i < M - len_t_theta; ++i) {
        complex_matrix_get_columns(vet_noise, vet_noise_temp, M, M - len_t_theta, i);
        complex_matrix_conjugate_transpose_multiplication(vet_noise_temp, Pn_temp, M, 1);
        complex_matrix_addition(Pn, Pn_temp, M, M);
    }
    free(vet_noise_temp);
    free(Pn_temp);
}


// compute S_MUSIC: matlab code: (S_MUSIC(i)=1/(a_vector'*Pn*a_vector))
std::complex<double> compute_S_MUSIC(std::complex<double> *a_vector, std::complex<double> *Pn, int M) {
    std::complex<double> *Pn_a_vector_temp = (std::complex<double>*)malloc(M * sizeof(std::complex<double>));
    std::complex<double> *S_MUSIC_temp = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    complex_matrix_multiplication(Pn, a_vector, Pn_a_vector_temp, M, M, 1);
    complex_matrix_conjugate_transpose(a_vector, M, 1);
    complex_matrix_multiplication(a_vector, Pn_a_vector_temp, S_MUSIC_temp, 1, M, 1);
    std::complex<double> S_MUSIC = std::complex<double>(1) / S_MUSIC_temp[0];
    free(Pn_a_vector_temp);
    free(S_MUSIC_temp);
    return S_MUSIC;
}


// QR decomposer for c code
void qr(std::complex<double> *A, std::complex<double> *Q, std::complex<double> *R, int row, int col) {
    std::complex<double> *Q_col = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
    std::complex<double> *vector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
    std::complex<double> *Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
    std::complex<double> *power_cur = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    std::complex<double> *power_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    std::complex<double> *proj_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    std::complex<double> *proj_Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
    // time initial
    float timeStart, timeEnd;
    // struct timespec start_qr, end_qr;


    for(int i = 0; i < row * col; i += (col + 1)) {
        Q[i].real(1);
        R[i].real(1);
    }
    // timestamp start
     timeStart = clock();
    // clock_gettime(CLOCK_MONOTONIC, &start_qr); 
    for(int i = 0; i < col; ++i) {
        for(int m = 0; m < row; ++m) {
            Q[m * col + i] = A[m * col + i];
        }
        complex_matrix_get_columns(Q, Q_col, row, col, i);
        // conjuate Q_col
        complex_matrix_conjugate_transpose(Q_col, row, 1);
        memset(power_cur, 0, sizeof(std::complex<double>));
        complex_matrix_conjugate_transpose_multiplication(Q_col, power_cur, 1, row);
        power_cur[0] = sqrt(power_cur[0]);
        if(i > 0) {
            complex_matrix_get_columns(A, vector_cur, row, col, i);
            std::complex<double> *Q_col_proj = (std::complex<double>*)malloc(row * i * sizeof(std::complex<double>));
            std::complex<double> *proj_vector = (std::complex<double>*)malloc(i * sizeof(std::complex<double>));
            memset(proj_vector, 0, i * sizeof(std::complex<double>));
            for(int j = 0; j < i; ++j) {
                for(int m = 0; m < row; ++m) {
                    Q_col_proj[m * i + j] = Q[m * col + j];
                }
            }
            complex_matrix_conjugate_transpose(Q_col_proj, row, i);
            complex_matrix_multiplication(Q_col_proj, vector_cur, proj_vector, i, row, 1);
            complex_matrix_conjugate_transpose(Q_col_proj, i, row);
            memset(Q_col, 0, row * 1 * sizeof(std::complex<double>));
            complex_matrix_multiplication(Q_col_proj, proj_vector, Q_col, row, i, 1);
            complex_matrix_subtraction(vector_cur, Q_col, row, 1);
            for(int m = 0; m < row; ++m) {
                Q[m * col + i] = vector_cur[m];
            }
            for(int j = 0; j < i; ++j) {
                R[i + col * j] = proj_vector[j];
            }
            free(Q_col_proj);
            free(proj_vector);
        }
        complex_matrix_get_columns(Q, Q_col, row, col, i);
        // conjuate Q_col
        complex_matrix_conjugate_transpose(Q_col, row, 1);
        memset(power_val, 0, sizeof(std::complex<double>));
        complex_matrix_conjugate_transpose_multiplication(Q_col, power_val, 1, row);
        power_val[0] = sqrt(power_val[0]);

        //1e-4 = 0.0001
        if(power_val[0].real() / power_cur[0].real() < 1e-4) {
            R[i * row + i] = 0;
            // span again
            for(int m = 0; m < row; ++m) {
                Q[m * col + i] = 0;
            }
            Q[i * row + i].real(1);
            complex_matrix_get_columns(Q, vector_cur, row, col, i);
            for(int j = 0; j < i; ++j) {
                complex_matrix_get_columns(Q, Qvector_cur, row, col, j);
                memset(proj_val, 0, sizeof(std::complex<double>));
                complex_matrix_conjugate_transpose(Qvector_cur, row, 1);
                complex_matrix_multiplication(Qvector_cur, vector_cur, proj_val, 1, row, 1);
                complex_matrix_conjugate_transpose(Qvector_cur, 1, row);
                complex_matrix_get_columns(Q, Q_col, row, col, i);
                memset(proj_Qvector_cur, 0, row * 1 * sizeof(std::complex<double>));
                complex_matrix_multiplication(Qvector_cur, proj_val, proj_Qvector_cur, row, 1, 1);
                complex_matrix_subtraction(Q_col, proj_Qvector_cur, row, 1);
                for(int m = 0; m < row; ++m) {
                    Q[m * col + i] = Q_col[m];
                }
            }
            complex_matrix_get_columns(Q, Q_col, row, col, i);
            complex_matrix_conjugate_transpose(Q_col, row, 1);
            memset(power_val, 0, sizeof(std::complex<double>));
            complex_matrix_conjugate_transpose_multiplication(Q_col, power_val, 1, row);
            power_val[0] = sqrt(power_val[0]);
            complex_matrix_conjugate_transpose(Q_col, 1, row);
            for(int m = 0; m < row; ++m) {
                Q[m * col + i] /= power_val[0]; // Q[m * col + i] = Q[m * col + i] / power_val[0]
            }
        } else {
            R[i * row + i] = power_val[0];
            for(int m = 0; m < row; ++m) {
                Q[m * col + i] /= power_val[0];
            }
        }
    }
    // timestamp start
    // clock_gettime(CLOCK_MONOTONIC, &end_qr); 
    timeEnd = clock();

    #ifdef PRINT_RESULT
    //std::cout << std::setprecision(6) << "QR decomposer:\t\t" << (end_qr.tv_nsec - start_qr.tv_nsec) *1.0 / 1000 << " us" << std::endl;
    //std::cout << std::setprecision(6) << "QR decomposer:\t\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000   << " ms" << std::endl;
    #endif
    //global_QR_count++;
    free(Q_col);
    free(vector_cur);
    free(Qvector_cur);
    free(power_cur); 
    free(power_val);
    free(proj_val);
    free(proj_Qvector_cur);
}



// compute eigen upper triangular
void eigen_upper_triangular(std::complex<double> *A, std::complex<double> *eigenvalue, std::complex<double> *eigenvector, int row, int col) {
    std::complex<double> *vector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
    std::complex<double> *eigen_element_cur = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    std::complex<double> *vector_cur_temp = (std::complex<double>*)malloc(sizeof(std::complex<double>));
    std::complex<double> *A_col = (std::complex<double>*)malloc(1 * col * sizeof(std::complex<double>));
    std::complex<double> diff_eigen_value = 0;
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
			if(i > j){
				A[i * col + j].real(0);
				A[i * col + j].imag(0);
			} 
            if(i == j) {
                eigenvalue[i * col + j] = A[i * col + j];
                eigenvector[i * col + j].real(1);
            }
        }
    }
    for(int i = 0; i < col; ++i) {
        complex_matrix_get_columns(eigenvector, vector_cur, row, col, i);
        for(int j = i - 1; j > -1; --j) {
            diff_eigen_value = eigenvalue[i * col + i] - eigenvalue[j * col + j];
            if(diff_eigen_value.real() < 1e-8) eigen_element_cur[0] = 0;
            else {
                complex_matrix_get_rows(A, A_col, row, col, j);
                complex_matrix_multiplication(A_col, vector_cur, eigen_element_cur, 1, row, 1);
                eigen_element_cur[0] = eigen_element_cur[0] / diff_eigen_value;
            }
            vector_cur[j] = eigen_element_cur[0];
        }
        complex_matrix_conjugate_transpose(vector_cur, row, 1);
        complex_matrix_conjugate_transpose_multiplication(vector_cur, vector_cur_temp, 1, row);
        vector_cur_temp[0] = sqrt(vector_cur_temp[0]);
        complex_matrix_conjugate_transpose(vector_cur, 1, row);
        for(int m = 0; m < row; ++m) {
            eigenvector[m * col + i] = vector_cur[m] / vector_cur_temp[0];
        }
    }
    free(vector_cur);
    free(eigen_element_cur);
    free(vector_cur_temp);
    free(A_col);
}


// compute complex eigenvector and eigenvalue for c code
void eigen(std::complex<double> *A, std::complex<double> *Ve, std::complex<double> *De, int row, int col, int iter) {
   
    std::complex<double> *Q = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));
    std::complex<double> *R = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));
    std::complex<double> *Q_temp = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));
    std::complex<double> *Q_temp_clone = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));

    // float timestart_eigen_1, timeend_eigen_1;
    // float timestart_eigen_2, timeend_eigen_2;
    // float timestart_eigen_3, timeend_eigen_3;
    // float timestart_eigen_4, timeend_eigen_4; 
    // float timestart_eigen_5, timeend_eigen_5; 
    // float timestart_eigen_6, timeend_eigen_6;
    // float timestart_eigen_7, timeend_eigen_7;
    // float timestart_eigen_8, timeend_eigen_8; 


    for(int i = 0; i < row * col; i += (col + 1)) {
        Q_temp[i].real(1);
    }

    //timestart_eigen_1 = clock();
    for(int i = 0; i < iter; ++i) {
        //timestart_eigen_5 = clock();
        
        qr(A, Q, R, row, col); 
        //timestart_eigen_7 = clock();
        complex_matrix_multiplication(R, Q, A, row, row, col);
        complex_matrix_multiplication(Q_temp, Q, Q_temp_clone, row, row, col);
        memcpy(Q_temp, Q_temp_clone, row * col * sizeof(std::complex<double>));
        // timeend_eigen_7 = clock();
        // timeend_eigen_5 = clock();
    }
    // timeend_eigen_1 = clock();
    // timestart_eigen_2 = clock();
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; ++j) {
            if(i > j) A[i * col + j] = 0;
        }
    }
    // timeend_eigen_2 = clock();

    // timestart_eigen_3 = clock();
    std::complex<double> *YY0 = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));
    std::complex<double> *XX0 = (std::complex<double>*)calloc(row * col, sizeof(std::complex<double>));
    eigen_upper_triangular(A, YY0, XX0, row, col);
    memcpy(De, YY0, row * col * sizeof(std::complex<double>));
    complex_matrix_multiplication(Q_temp, XX0, Ve, row, row, col);
    //timeend_eigen_3 = clock();

    // std::cout << std::setprecision(6) << "timeend_eigen_1:\t\t" << (timeend_eigen_1 - timestart_eigen_1) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    // //std::cout << std::setprecision(6) << "timeend_eigen_2:\t\t" << (timeend_eigen_2 - timestart_eigen_2) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    // //std::cout << std::setprecision(6) << "timeend_eigen_3:\t\t" << (timeend_eigen_3 - timestart_eigen_3) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    // //std::cout << std::setprecision(6) << "timeend_eigen_4:\t\t" << (timeend_eigen_4 - timestart_eigen_4) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    // std::cout << std::setprecision(6) << "timeend_eigen_5:\t\t" << (timeend_eigen_5 - timestart_eigen_5) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    // std::cout << std::setprecision(6) << "timeend_eigen_7:\t\t" << (timeend_eigen_7 - timestart_eigen_7) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    
    free(Q);
    free(R);
    free(Q_temp);
    free(Q_temp_clone);
    free(YY0);
    free(XX0);
}




// // compute the MUSIC DOA in one dimension on CPU(test)
// void MUSIC_DOA_1D_CPU_test(int M, int snr, int qr_iter, int multi_input, float *result) {
//     #ifdef PRINT_RESULT
//     printf("--Parameter--\n");
//     printf("Antenna count:\t\t%d\n", M);
//     printf("SNR:\t\t\t%d\n", snr);
//     printf("QR iteration:\t\t%d\n", qr_iter);
//     printf("Multiple input size:\t%d\n", multi_input);
//     #endif
// // generate the signal
//     // time initial
//     float timeStart, timeEnd;
//     // float timestart_vet_noise, timeend_vet_noise;
//     // float timestart_pn, timeend_pn;
//     // float timestart_eigen, timeend_eigen;
//     //struct timespec start, end;
    
//     // parameter setting
//     const int fc = 180e+6;
//     const int c = 3e+8;
//     const double lemda = (double)c / (double)fc;
//     std::complex<double> d(lemda * 0.5);
//     std::complex<double> kc(2.0 * PI / lemda);
//     const int nd = 500;
    
//     // angle setting
//     const int len_t_theta = 3;
//     std::complex<double> *t_theta = (std::complex<double>*)malloc(len_t_theta * sizeof(std::complex<double>));
//     t_theta[0].real(7);
//     t_theta[1].real(12);
//     t_theta[2].real(20);
//     #ifdef PRINT_RESULT
//     std::cout << "Theta(degree):\t\t[";
//     for(int i = 0; i < len_t_theta; ++i) {
//         if(i != len_t_theta - 1) std::cout << t_theta[i].real() << ", ";
//         else std::cout << t_theta[i].real() << "]\n\n";
//     }
//     std::cout << "---Time---" << std::endl;
//     #endif
    
//     // A_theta matrix (M, length of t_theta)
//     std::complex<double> *A_theta = (std::complex<double>*)malloc(M * len_t_theta * sizeof(std::complex<double>));
//     for(int i = 0; i < M; ++i) {
//         for(int j = 0; j < len_t_theta; ++j) {
//             A_theta[i * len_t_theta + j] = exp(1i * kc * std::complex<double>(i) * d * sin(t_theta[j] * std::complex<double>(PI / 180)));
//         }
//     }
    
//     // t_sig matrix (length of t_theta, nd)
//     std::complex<double> *t_sig = (std::complex<double>*)malloc(len_t_theta * nd * sizeof(std::complex<double>));
//     for(int i = 0; i < len_t_theta; ++i) {
//         for(int j = 0; j < nd; ++j) {
//             t_sig[i * nd + j] = (randn() + randn() * 1i) / std::complex<double>(sqrt(2));
//             //if(i == 0) t_sig[i * nd + j] *= (std::complex<double>)2;
//         }
//     }
    
//     // sig_co matrix (M, nd)
//     std::complex<double> *sig_co = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
//     // compute sig_co
//     complex_matrix_multiplication(A_theta, t_sig, sig_co, M, len_t_theta, nd);

// // receiver
//     // x_r matrix (M, nd)
//     std::complex<double> *x_r = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
//     // memcpy(x_r, sig_co, M * nd * sizeof(std::complex<double>));
//     // add noise to the signal
//     awgn(sig_co, x_r, snr, M, nd);

// // music algorithm
//     // R_xx matrix (M, M)
//     std::complex<double> *R_xx = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
//     // matlab code:  (R_xx = 1 / M * x_r * x_r')
//     complex_matrix_conjugate_transpose_multiplication(x_r, R_xx, M, nd);
//     for(int i = 0; i < M * M; ++i) R_xx[i] /= std::complex<double>(M);
//     // compute eigenvector Ve (M, M)
//     std::complex<double> *Ve = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
//     std::complex<double> *De = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
//     // timestamp start
//     //clock_gettime(CLOCK_MONOTONIC, &start); 
//     timeStart = clock();
//     // timestart_eigen = clock();

//     eigen(R_xx, Ve, De, M, M, qr_iter);
// 	//printf("Ve \n");
// 	//print_complex_matrix(Ve,M,M);
// 	//printf("De \n");
// 	//print_complex_matrix(De,M,M);
// 	//return;
//     // timeend_eigen = clock();
//     // timestart_vet_noise = clock();
//     // get vet_noise (M, M - len_t_theta): part of Ve (eigenvector)
//     std::complex<double> *vet_noise = (std::complex<double>*)malloc(M * (M - len_t_theta) * sizeof(std::complex<double>));
//     for(int i = 0; i < M; ++i) {
//         for(int j = len_t_theta; j < M; ++j) {
//             vet_noise[i * (M - len_t_theta) + j - len_t_theta] = Ve[i * M + j];
//         }
//     }
//     // timeend_vet_noise = clock();
//     // timestart_pn = clock();
//     // Pn matrix (M, M)
//     std::complex<double> *Pn = (std::complex<double>*)calloc(M * M, sizeof(std::complex<double>));
//     compute_Pn(Pn, vet_noise, M, len_t_theta);
//     //timeend_pn = clock();
//     // timestamp end
//     timeEnd = clock();
//     //clock_gettime(CLOCK_MONOTONIC, &end);
//     #ifdef PRINT_RESULT
//     std::cout << std::setprecision(6) << "MUSIC (cpu):\t\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     // std::cout << std::setprecision(6) << "vet_noise:\t\t" << (timeend_vet_noise - timestart_vet_noise) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     // std::cout << std::setprecision(6) << "pn:\t\t" << (timeend_pn - timestart_pn) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     // std::cout << std::setprecision(6) << "eigen:\t\t" << (timeend_eigen - timestart_eigen) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     #endif
//     result[2] += (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000;

// // array pattern
//     // parameter setting
//     const int len_dth = 401;
//     double *dth = (double*)malloc(len_dth * sizeof(double));
//     double *dr = (double*)malloc(len_dth * sizeof(double));
//     for(int i = 0; i < len_dth; ++i) { // do only one time, no need to be paralleled
//         dth[i] = -10 + 0.1 * i;
//         dr[i] = dth[i] * PI / 180;
//     }
//     // compute S_MUSIC_dB
//     std::complex<double> *a_vector = (std::complex<double>*)malloc(M * sizeof(std::complex<double>));
//     std::complex<double> *S_MUSIC = (std::complex<double>*)malloc(len_dth * sizeof(std::complex<double>));
//     double *S_MUSIC_dB = (double*)malloc(len_dth * sizeof(double));
//     // timestamp start
//     timeStart = clock();


//     for(int i = 0; i < len_dth; ++i) { // can be paralleled to compute S_MUSIC_dB
//         for(int j = 0; j < M; ++j) {
//             a_vector[j] = exp(1i * kc * (std::complex<double>)j * d * sin(dr[i]));
//         }
//         S_MUSIC[i] = compute_S_MUSIC(a_vector, Pn, M);
//         // compute S_MUSIC_dB
//         S_MUSIC_dB[i] = 20 * log10(abs(S_MUSIC[i]));
//     }

//     // find Max and position
//     double max_temp = S_MUSIC_dB[0];
//     int position = 0;
//     for(int i = 0; i < len_dth; ++i) {
//         if(S_MUSIC_dB[i] > max_temp) {
//             max_temp = S_MUSIC_dB[i];
//             position = i;
//         }
//     }
//     // timestamp end
//     timeEnd = clock();
//     #ifdef PRINT_RESULT
//     std::cout << std::setprecision(6) << "Array pattern (cpu):\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     // print the result 
//     printf("\n--Result--\n");
//     std::cout << "Theta estimation:\t" << dth[position] << std::endl;
//     #endif
//     float error[3];
//     float errorFinal = 0;
//     for(int i = 0; i < 3; ++i) {
//         error[i] = abs(dth[position] - t_theta[i].real());
//         if(i == 0) errorFinal = error[i];
//         else if(error[i] < errorFinal) errorFinal = error[i];
//     }
//     if(errorFinal > result[0]) result[0] = errorFinal;
//     else if (errorFinal == 0) result[1]++;

// // plot the result
//     #ifdef PLOT_RESULT
//     std::vector<double> S_MUSIC_dB_vec(S_MUSIC_dB, S_MUSIC_dB + len_dth);
//     std::vector<double> dth_vec(dth, dth + len_dth);
//     plt::plot(dth_vec, S_MUSIC_dB_vec, "blue");
//     plt::title("MUSIC DOA Estimation");
//     plt::xlabel("Theta (degree)");
//     plt::ylabel("Power Spectrum (dB)");
//     plt::xlim(dth[0], dth[len_dth - 1]);
//     plt::grid(true);
//     plt::show();
//     #endif

// // free memory
//     free(t_theta);
//     free(A_theta);
//     free(t_sig);
//     free(sig_co);
//     free(x_r);
//     free(R_xx);
//     free(Ve);
//     free(De);
//     free(vet_noise);
//     free(Pn);
//     free(dth);
//     free(dr);
//     free(a_vector);
//     free(S_MUSIC);
//     free(S_MUSIC_dB);
// }


void print_sample(double *IQ) {
    for(int i = 0; i < 10; ++i) {
        std::cout << "[" << i << "] " << IQ[i * 2] << " " << IQ[i * 2 + 1] << std::endl;
    }
}


void convert_int16_to_double(int16_t *IQ, double *double_IQ) {
    int index = 2 * RADAR_SAMPLE;
    std::copy(IQ, IQ + RADAR_SAMPLE, double_IQ);

    // print_sample(double_IQ);
}

// compute the MUSIC DOA in one dimension on CPU
void MUSIC_DOA_1D_CPU(int M, int qr_iter, int angle, float *result, double *double_IQ) {
    #ifdef PRINT_RESULT
    printf("--Parameter--\n");
    printf("Antenna count:\t\t%d\n", M);
    printf("SNR:\t\t\t%d\n", global_SNR);
    printf("QR iteration:\t\t%d\n", qr_iter);
    #endif
// generate the signal
    // time initial
    float timeStart, timeEnd;
    // parameter setting
    const int fc = 180e+6;
    const int c = 3e+8;
    const double lemda = (double)c / (double)fc;
    std::complex<double> d(lemda * 0.5);
    std::complex<double> kc(2.0 * PI / lemda);
    const int nd = 500;
    // angle setting
    const int len_t_theta = 1;
    std::complex<double> *t_theta = (std::complex<double>*)malloc(len_t_theta * sizeof(std::complex<double>));
    t_theta[0].real(angle);
    // t_theta[1].real(12);
    // t_theta[2].real(20);
    #ifdef PRINT_RESULT
    std::cout << "Theta(degree):\t\t[";
    for(int i = 0; i < len_t_theta; ++i) {
        if(i != len_t_theta - 1) std::cout << t_theta[i].real() << ", ";
        else std::cout << t_theta[i].real() << "]\n\n";
    }
    std::cout << "---Time---" << std::endl;
    #endif
    // A_theta matrix (M, length of t_theta)
    std::complex<double> *A_theta = (std::complex<double>*)malloc(M * len_t_theta * sizeof(std::complex<double>));
    for(int i = 0; i < M; ++i) {
        for(int j = 0; j < len_t_theta; ++j) {
            A_theta[i * len_t_theta + j] = exp(I_1 * kc * std::complex<double>(i) * d * sin(t_theta[j] * std::complex<double>(PI / 180)));
        }
    }
    // t_sig matrix (length of t_theta, nd)
    std::complex<double> *t_sig = (std::complex<double>*)malloc(len_t_theta * nd * sizeof(std::complex<double>));
    memcpy(t_sig, double_IQ, 2 * nd * sizeof(double));
    // for(int i = 0; i < len_t_theta; ++i) {
    //     for(int j = 0; j < nd; ++j) {
    //         t_sig[i * nd + j] = (randn() + randn() * 1i) / std::complex<double>(sqrt(2));
    //         // if(i == 0) t_sig[i * nd + j] *= (std::complex<double>)2;
    //     }
    // }
    // sig_co matrix (M, nd)
    std::complex<double> *sig_co = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
    // compute sig_co
    complex_matrix_multiplication(A_theta, t_sig, sig_co, M, len_t_theta, nd);

// receiver
    // x_r matrix (M, nd)
    std::complex<double> *x_r = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
    // memcpy(x_r, sig_co, M * nd * sizeof(std::complex<double>));
    // add noise to the signal
    awgn(sig_co, x_r, global_SNR, M, nd);

// music algorithm
    // R_xx matrix (M, M)
    std::complex<double> *R_xx = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    // matlab code:  (R_xx = 1 / M * x_r * x_r')
    complex_matrix_conjugate_transpose_multiplication(x_r, R_xx, M, nd);
    for(int i = 0; i < M * M; ++i) R_xx[i] /= std::complex<double>(M);
    // compute eigenvector Ve (M, M)
    std::complex<double> *Ve = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    std::complex<double> *De = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    // timestamp start
    timeStart = clock();
    eigen(R_xx, Ve, De, M, M, qr_iter);
    // get vet_noise (M, M - len_t_theta): part of Ve (eigenvector)
    std::complex<double> *vet_noise = (std::complex<double>*)malloc(M * (M - len_t_theta) * sizeof(std::complex<double>));
    for(int i = 0; i < M; ++i) {
        for(int j = len_t_theta; j < M; ++j) {
            vet_noise[i * (M - len_t_theta) + j - len_t_theta] = Ve[i * M + j];
        }
    }
    // Pn matrix (M, M)
    std::complex<double> *Pn = (std::complex<double>*)calloc(M * M, sizeof(std::complex<double>));
    compute_Pn(Pn, vet_noise, M, len_t_theta);
    // timestamp end
    timeEnd = clock();
    #ifdef PRINT_RESULT
    std::cout << std::setprecision(6) << "MUSIC (cpu):\t\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    #endif
    result[2] += (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000;

// array pattern
    // parameter setting
    const int len_dth = 401;
    double *dth = (double*)malloc(len_dth * sizeof(double));
    double *dr = (double*)malloc(len_dth * sizeof(double));
    for(int i = 0; i < len_dth; ++i) { // do only one time, no need to be paralleled
        dth[i] = -10 + 0.1 * i;
        dr[i] = dth[i] * PI / 180;
    }
    // compute S_MUSIC_dB
    std::complex<double> *a_vector = (std::complex<double>*)malloc(M * sizeof(std::complex<double>));
    std::complex<double> *S_MUSIC = (std::complex<double>*)malloc(len_dth * sizeof(std::complex<double>));
    double *S_MUSIC_dB = (double*)malloc(len_dth * sizeof(double));
    // timestamp start
    timeStart = clock();
    for(int i = 0; i < len_dth; ++i) { // can be paralleled to compute S_MUSIC_dB
        for(int j = 0; j < M; ++j) {
            a_vector[j] = exp(I_1 * kc * (std::complex<double>)j * d * sin(dr[i]));
        }
        S_MUSIC[i] = compute_S_MUSIC(a_vector, Pn, M);
        // compute S_MUSIC_dB
        S_MUSIC_dB[i] = 20 * log10(abs(S_MUSIC[i]));
    }
    // find Max and position
    double max_temp = S_MUSIC_dB[0];
    int position = 0;
    for(int i = 0; i < len_dth; ++i) {
        if(S_MUSIC_dB[i] > max_temp) {
            max_temp = S_MUSIC_dB[i];
            position = i;
        }
    }
    // timestamp end
    timeEnd = clock();
    #ifdef PRINT_RESULT
    std::cout << std::setprecision(6) << "Array pattern (cpu):\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    printf("\n--Result--\n");
    std::cout << "Theta estimation:\t" << dth[position] << std::endl;
    std::cout << std::endl << "-----------------------------------------" << std::endl << std::endl;
    #endif
    float error;
    error = abs(dth[position] - t_theta[0].real());

    if(error > result[0]) result[0] = error;
    if (error != 0) result[1] += pow(error, 2);

// plot the result
    #ifdef PLOT_RESULT
    std::vector<double> S_MUSIC_dB_vec(S_MUSIC_dB, S_MUSIC_dB + len_dth);
    std::vector<double> dth_vec(dth, dth + len_dth);
    plt::plot(dth_vec, S_MUSIC_dB_vec, "blue");
    plt::title("MUSIC DOA Estimation");
    plt::xlabel("Theta (degree)");
    plt::ylabel("Power Spectrum (dB)");
    plt::xlim(dth[0], dth[len_dth - 1]);
    plt::grid(true);
    plt::show();
    #endif

// free memory
    free(t_theta);
    free(A_theta);
    free(t_sig);
    free(sig_co);
    free(x_r);
    free(R_xx);
    free(Ve);
    free(De);
    free(vet_noise);
    free(Pn);
    free(dth);
    free(dr);
    free(a_vector);
    free(S_MUSIC);
    free(S_MUSIC_dB);
}

//music initail
extern "C"
void MUSIC_init(int subframe, uint32_t *rx_data, float *result){
    
    printf("[music init]:1\n");
    int16_t *IQ = (int16_t*)malloc(2 * RADAR_SAMPLE * sizeof(int16_t));
    memcpy(IQ, (int16_t*)rx_data, RADAR_SAMPLE * sizeof(int32_t));

    double *double_IQ = (double*)malloc(2 * RADAR_SAMPLE * sizeof(double));
    convert_int16_to_double(IQ, double_IQ);


    // music_start(double_IQ);
    int M = global_antenna;
    int qr_iter = global_QR_iteration;
    int angle = global_angle;

 
    MUSIC_DOA_1D_CPU(M, qr_iter, angle, result, double_IQ);
    
    free(IQ);
    free(double_IQ);

}

//int  main(void){

//     global_antenna = 8;
//     global_QR_iteration = 100;
//     global_multi_input = 8;
//     global_SNR = 100;
//     //global_music_type = 0; //0:MUSIC;1:MVDR
//     float result[3] = {0};
//     //music_mvdr_init(global_music_type,result); 

//     MUSIC_DOA_1D_CPU_test(global_antenna,
//                         global_SNR,
//                         global_QR_iteration,
//                         global_multi_input,
//                         result
//                     );

//std::cout << sizeof(std::complex<double>) << std::endl;

// }





/*
// compute the MVDR DOA in one dimension on CPU
void MVDR_DOA_1D_CPU(int M, int snr, int qr_iter, int multi_input, float *result) {
    #ifdef PRINT_RESULT
    printf("--Parameter--\n");
    printf("Antenna count:\t\t%d\n", M);
    printf("SNR:\t\t\t%d\n", snr);
    printf("QR iteration:\t\t%d\n", qr_iter);
    printf("Multiple input size:\t%d\n", multi_input);
    #endif
// generate the signal
    // time initial
    float timeStart, timeEnd;
    // parameter setting
    const int fc = 180e+6;
    const int c = 3e+8;
    const double lemda = (double)c / (double)fc;
    std::complex<double> d(lemda * 0.5);
    std::complex<double> kc(2.0 * PI / lemda);
    const int nd = 500;
    // angle setting
    const int len_t_theta = 1;
    std::complex<double> *t_theta = (std::complex<double>*)malloc(len_t_theta * sizeof(std::complex<double>));
    t_theta[0].real(3);
    t_theta[1].real(12);
    t_theta[2].real(20);
    #ifdef PRINT_RESULT
    std::cout << "Theta(degree):\t\t[";
    for(int i = 0; i < len_t_theta; ++i) {
        if(i != len_t_theta - 1) std::cout << t_theta[i].real() << ", ";
        else std::cout << t_theta[i].real() << "]\n\n";
    }
    std::cout << "---Time---" << std::endl;
    #endif
    // A_theta matrix (M, length of t_theta)
    std::complex<double> *A_theta = (std::complex<double>*)malloc(M * len_t_theta * sizeof(std::complex<double>));
    for(int i = 0; i < M; ++i) {
        for(int j = 0; j < len_t_theta; ++j) {
            A_theta[i * len_t_theta + j] = exp(1i * kc * std::complex<double>(i) * d * sin(t_theta[j] * std::complex<double>(PI / 180)));
        }
    }
    // t_sig matrix (length of t_theta, nd)
    std::complex<double> *t_sig = (std::complex<double>*)malloc(len_t_theta * nd * sizeof(std::complex<double>));
    for(int i = 0; i < len_t_theta; ++i) {
        for(int j = 0; j < nd; ++j) {
            t_sig[i * nd + j] = (randn() + randn() * 1i) / std::complex<double>(sqrt(2));
            // if(i == 0) t_sig[i * nd + j] *= (std::complex<double>)2;
        }
    }
    // sig_co matrix (M, nd)
    std::complex<double> *sig_co = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
    // compute sig_co
    complex_matrix_multiplication(A_theta, t_sig, sig_co, M, len_t_theta, nd);

// receiver
    // x_r matrix (M, nd)
    std::complex<double> *x_r = (std::complex<double>*)malloc(M * nd * sizeof(std::complex<double>));
    // memcpy(x_r, sig_co, M * nd * sizeof(std::complex<double>));
    // add noise to the signal
    awgn(sig_co, x_r, snr, M, nd);

// mvdr algorithm
    // R_xx matrix (M, M)
    std::complex<double> *R_xx = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    // matlab code:  (R_xx = 1 / M * x_r * x_r')
    complex_matrix_conjugate_transpose_multiplication(x_r, R_xx, M, nd);
    for(int i = 0; i < M * M; ++i) R_xx[i] /= std::complex<double>(M);
    // compute eigenvector Ve (M, M)
    std::complex<double> *Ve = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    std::complex<double> *De = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
    // timestamp start
    timeStart = clock();
    eigen(R_xx, Ve, De, M, M, qr_iter);
	std::complex<double> *R_xx_inv_1 = (std::complex<double>*)malloc(M * M * sizeof(std::complex<double>));
	std::complex<double> *Pn = (std::complex<double>*)calloc(M * M, sizeof(std::complex<double>));
	for(int i = 0; i < M * M; i += (M + 1)) {
		if(abs(De[i])<0.00000000001) {
			De[i].real(1000000);
			De[i].imag(0);
		}
		else De[i]= std::complex <double> (1)/De[i];
	}
	
	complex_matrix_multiplication(Ve, De, R_xx_inv_1, M, M, M);
	complex_matrix_conjugate_transpose(Ve, M, M);
	complex_matrix_multiplication(R_xx_inv_1, Ve, Pn, M, M, M);

    timeEnd = clock();
    #ifdef PRINT_RESULT
    std::cout << std::setprecision(6) << "MVDR (cpu):\t\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    #endif
    result[2] += (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000;

// array pattern
    // parameter setting
    const int len_dth = 401;
    double *dth = (double*)malloc(len_dth * sizeof(double));
    double *dr = (double*)malloc(len_dth * sizeof(double));
    for(int i = 0; i < len_dth; ++i) { // do only one time, no need to be paralleled
        dth[i] = -10 + 0.1 * i;
        dr[i] = dth[i] * PI / 180;
    }
    // compute S_MVDR_dB
    std::complex<double> *a_vector = (std::complex<double>*)malloc(M * sizeof(std::complex<double>));
    std::complex<double> *S_MVDR = (std::complex<double>*)malloc(len_dth * sizeof(std::complex<double>));
    double *S_MVDR_dB = (double*)malloc(len_dth * sizeof(double));
    // timestamp start
    timeStart = clock();
    for(int i = 0; i < len_dth; ++i) { // can be paralleled to compute S_MVDR_dB
        for(int j = 0; j < M; ++j) {
            a_vector[j] = exp(1i * kc * (std::complex<double>)j * d * sin(dr[i]));
        }
        S_MVDR[i] = compute_S_MUSIC(a_vector, Pn, M);
        // compute S_MVDR_dB
        S_MVDR_dB[i] = 20 * log10(abs(S_MVDR[i]));
    }
    // find Max and position
    double max_temp = S_MVDR_dB[0];
    int position = 0;
    for(int i = 0; i < len_dth; ++i) {
        if(S_MVDR_dB[i] > max_temp) {
            max_temp = S_MVDR_dB[i];
            position = i;
        }
    }
    // timestamp end
    timeEnd = clock();
    #ifdef PRINT_RESULT
    std::cout << std::setprecision(6) << "Array pattern (cpu):\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
    printf("\n--Result--\n");
    std::cout << "Theta estimation:\t" << dth[position] << std::endl;
    std::cout << std::endl << "-----------------------------------------" << std::endl << std::endl;
    #endif
    float error;
    error = abs(dth[position] - t_theta[0].real());

    if(error > result[0]) result[0] = error;
    if (error != 0) result[1] += pow(error, 2);

// plot the result
    #ifdef PLOT_RESULT
    std::vector<double> S_MVDR_dB_vec(S_MVDR_dB, S_MVDR_dB + len_dth);
    std::vector<double> dth_vec(dth, dth + len_dth);
    plt::plot(dth_vec, S_MVDR_dB_vec, "blue");
    plt::title("MUSIC DOA Estimation");
    plt::xlabel("Theta (degree)");
    plt::ylabel("Power Spectrum (dB)");
    plt::xlim(dth[0], dth[len_dth - 1]);
    plt::grid(true);
    plt::show();
    #endif

// free memory
    free(t_theta);
    free(A_theta);
    free(t_sig);
    free(sig_co);
    free(x_r);
    free(R_xx);
    free(Ve);
    free(De);
    // free(vet_noise);
    free(Pn);
    free(dth);
    free(dr);
    free(a_vector);
    free(S_MVDR);
    free(S_MVDR_dB);
}

extern "C" 
void music_mvdr_init(int global_music_type, float *result){

    int antenna = global_music_antenna;
    int qr_iter = global_music_QR_iteration;
    //int angle = global_music_angle;
    int snr =global_music_SNR;
    int multi_input =global_music_multi_input;

 if(global_music_type == 0) {
        MUSIC_DOA_1D_CPU(antenna, snr, qr_iter, multi_input, result);
    } else if (global_music_type == 1) {
        MVDR_DOA_1D_CPU(antenna,snr, qr_iter, multi_input, result);
    }
}

// void complex_matrix_multiplication_sub_matrix(std::complex<double> *matA, std::complex<double> *matB, std::complex<double> *matC, int rowA, int rowB, int colB) {
// //void complex_matrix_multiplication( std::complex<double> *matA, std::complex<double> *matB, std::complex<double> *matC, int rowA, int rowB, int colB) {   
//     memset(matC, 0, rowA * colB * sizeof(std::complex<double>));
    
//     int row_half = rowA/2;

//     for (int i = 0; i < colB; i += row_half) {
//         for (int j = 0; j < rowB; j += row_half) {
//             for (int k = 0; k < colB; k += row_half) {
//                 for (int i2 = 0; i2 < row_half; ++i2) {
//                     for (int j2 = 0; j2 < row_half; ++j2) {
//                         for (int k2 = 0; k2 < row_half; ++k2) {
//                             matC[(i + i2) * rowB + (j + j2)] += matA[(i + i2) * rowA + (k + k2)] *
//                                                                  matB[(k + k2) * rowB + (j + j2)];
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }


// void complex_matrix_multiplication_sse(std::complex<double> *matA, std::complex<double> *matB, std::complex<double> *matC, int rowA, int rowB, int colB) {
    
//     for (int x = 0; x < src1_h; x += 4) {
//         for (int y = 0; y < src2_w; y += 4) {
//             __m128i des0 = _mm_setzero_si128 ();
//             __m128i des1 = _mm_setzero_si128 ();
//             __m128i des2 = _mm_setzero_si128 ();
//             __m128i des3 = _mm_setzero_si128 ();

//             for (int k = 0; k < src2_w; k += 4) {
//                 __m128i I0 = _mm_load_si128((__m128i *)(src1 + (x + 0) * src1_w + k));
//                 __m128i I1 = _mm_load_si128((__m128i *)(src1 + (x + 1) * src1_w + k));
//                 __m128i I2 = _mm_load_si128((__m128i *)(src1 + (x + 2) * src1_w + k));
//                 __m128i I3 = _mm_load_si128((__m128i *)(src1 + (x + 3) * src1_w + k));

//                 __m128i I4 = _mm_set_epi32 (src2[(k+3) * src2_w + y], src2[(k+2) * src2_w + y],
//                                             src2[(k+1) * src2_w + y], src2[k * src2_w + y]);
//                 __m128i I5 = _mm_set_epi32 (src2[(k+3) * src2_w + (y+1)],
//                                             src2[(k+2) * src2_w + (y+1)], src2[(k+1) * src2_w + (y+1)],
//                                             src2[(k+0) * src2_w + (y+1)]);
//                 __m128i I6 = _mm_set_epi32 (src2[(k+3) * src2_w + (y+2)],
//                                             src2[(k+2) * src2_w + (y+2)], src2[(k+1) * src2_w + (y+2)],
//                                             src2[(k+0) * src2_w + (y+2)]);
//                 __m128i I7 = _mm_set_epi32 (src2[(k+3) * src2_w + (y+3)],
//                                             src2[(k+2) * src2_w + (y+3)], src2[(k+1) * src2_w + (y+3)],
//                                             src2[(k+0) * src2_w + (y+3)]);

//                 __m128i T0 = _mm_mullo_epi32(I0, I4);
//                 __m128i T1 = _mm_mullo_epi32(I0, I5);
//                 __m128i T2 = _mm_mullo_epi32(I0, I6);
//                 __m128i T3 = _mm_mullo_epi32(I0, I7);

//                 __m128i T4 = _mm_mullo_epi32(I1, I4);
//                 __m128i T5 = _mm_mullo_epi32(I1, I5);
//                 __m128i T6 = _mm_mullo_epi32(I1, I6);
//                 __m128i T7 = _mm_mullo_epi32(I1, I7);

//                 __m128i T8 = _mm_mullo_epi32(I2, I4);
//                 __m128i T9 = _mm_mullo_epi32(I2, I5);
//                 __m128i T10 = _mm_mullo_epi32(I2, I6);
//                 __m128i T11 = _mm_mullo_epi32(I2, I7);

//                 __m128i T12 = _mm_mullo_epi32(I3, I4);
//                 __m128i T13 = _mm_mullo_epi32(I3, I5);
//                 __m128i T14 = _mm_mullo_epi32(I3, I6);
//                 __m128i T15 = _mm_mullo_epi32(I3, I7);

//                 __m128i T16 = _mm_unpacklo_epi32(T0, T1);
//                 __m128i T17 = _mm_unpacklo_epi32(T2, T3);
//                 __m128i T18 = _mm_unpackhi_epi32(T0, T1);
//                 __m128i T19 = _mm_unpackhi_epi32(T2, T3);

//                 __m128i T20 = _mm_unpacklo_epi64(T16, T17);
//                 __m128i T21 = _mm_unpackhi_epi64(T16, T17);
//                 __m128i T22 = _mm_unpacklo_epi64(T18, T19);
//                 __m128i T23 = _mm_unpackhi_epi64(T18, T19);

//                 T20 = _mm_add_epi32(T20, T21);
//                 T20 = _mm_add_epi32(T20, T22);
//                 T20 = _mm_add_epi32(T20, T23);

//                 des0 = _mm_add_epi32(T20, des0);

//                 T16 = _mm_unpacklo_epi32(T4, T5);
//                 T17 = _mm_unpacklo_epi32(T6, T7);
//                 T18 = _mm_unpackhi_epi32(T4, T5);
//                 T19 = _mm_unpackhi_epi32(T6, T7);

//                 T20 = _mm_unpacklo_epi64(T16, T17);
//                 T21 = _mm_unpackhi_epi64(T16, T17);
//                 T22 = _mm_unpacklo_epi64(T18, T19);
//                 T23 = _mm_unpackhi_epi64(T18, T19);

//                 T20 = _mm_add_epi32(T20, T21);
//                 T20 = _mm_add_epi32(T20, T22);
//                 T20 = _mm_add_epi32(T20, T23);

//                 des1 = _mm_add_epi32(T20, des1);

//                 T16 = _mm_unpacklo_epi32(T8, T9);
//                 T17 = _mm_unpacklo_epi32(T10, T11);
//                 T18 = _mm_unpackhi_epi32(T8, T9);
//                 T19 = _mm_unpackhi_epi32(T10, T11);

//                 T20 = _mm_unpacklo_epi64(T16, T17);
//                 T21 = _mm_unpackhi_epi64(T16, T17);
//                 T22 = _mm_unpacklo_epi64(T18, T19);
//                 T23 = _mm_unpackhi_epi64(T18, T19);

//                 T20 = _mm_add_epi32(T20, T21);
//                 T20 = _mm_add_epi32(T20, T22);
//                 T20 = _mm_add_epi32(T20, T23);

//                 des2 = _mm_add_epi32(T20, des2);

//                 T16 = _mm_unpacklo_epi32(T12, T13);
//                 T17 = _mm_unpacklo_epi32(T14, T15);
//                 T18 = _mm_unpackhi_epi32(T12, T13);
//                 T19 = _mm_unpackhi_epi32(T14, T15);

//                 T20 = _mm_unpacklo_epi64(T16, T17);
//                 T21 = _mm_unpackhi_epi64(T16, T17);
//                 T22 = _mm_unpacklo_epi64(T18, T19);
//                 T23 = _mm_unpackhi_epi64(T18, T19);

//                 T20 = _mm_add_epi32(T20, T21);
//                 T20 = _mm_add_epi32(T20, T22);
//                 T20 = _mm_add_epi32(T20, T23);

//                 des3 = _mm_add_epi32(T20, des3);
//             }

//             _mm_store_si128((__m128i *)(dst + ((x + 0) * src2_w) + y), des0);
//             _mm_store_si128((__m128i *)(dst + ((x + 1) * src2_w) + y), des1);
//             _mm_store_si128((__m128i *)(dst + ((x + 2) * src2_w) + y), des2);
//             _mm_store_si128((__m128i *)(dst + ((x + 3) * src2_w) + y), des3);
//         }
//     }


// }



// void* mat_multi_work(void *ptr){

//     mat_data *data = (mat_data*)ptr;

//     int rowA = data->rowA;
//     int colB = data->colB;
//     int rowB = data->rowB;


//     int local_m = rowA/ NUM_THREADS;
//     int row_count = data->row_count;
//     int first_row = row_count*local_m;
//     int last_row = (row_count+1)*local_m-1;
//     //int last_row_test = ;
    
//     printf("row_count : %d\n",row_count);
//     printf("first_row : %d\n",first_row);
//     printf("last_row : %d\n",last_row);
//     printf("local_m : %d\n",local_m);


//     printf("rowA : %d\n",rowA);
//     printf("colB : %d\n",colB);
//     printf("rowB : %d\n",rowB);


//     std::complex<double> *matA = data->matA;
//     std::complex<double> *matB = data->matB;
//     std::complex<double> *matC = data->matC;
  

    
//     for(int i = first_row ; i < rowA; ++i) {    
//       for(int j = 0; j < colB; ++j) {
//          for(int k = 0; k < rowB; ++k) {
//             matC[i * colB + j] += matA[i * rowB + k] * matB[k * colB + j]; 
//            // std::cout << std::fixed << std::setprecision(6) << std::setw(27) <<matC[i * colB + j] << std::endl;           
//            }
//         }
//     }

//     pthread_exit(NULL);
    
// }

// void complex_matrix_multiplication_t(std::complex<double> *matA, std::complex<double> *matB, std::complex<double> *matC, int rowA, int rowB, int colB) {
    
//     memset(matC, 0, rowA * colB * sizeof(std::complex<double>));

//     mat_data data;

//     data.rowA = rowA; 
//     data.rowB = rowB;
//     data.colB = colB;

//     data.matA = matA;
//     data.matB = matB;
//     data.matC = matC; 

//     pthread_t thread [NUM_THREADS];

//     for (int i = 0; i < NUM_THREADS; i++){
//         //memset(matC, 0, rowA * colB * sizeof(std::complex<double>));
//         data.row_count = i;
//         //printf("data.row_count : %d\n",data.row_count);
//         pthread_create(&thread[i], NULL, mat_multi_work, (void*)&data);
//         //i++;
//         //data.row_count++;
//     }

//     for (int i = 0; i < NUM_THREADS; i++){
//         pthread_join(thread[i],NULL);

//         //data.row_test++;
//      }



// }

*/
// void *multi_qr_work(void *ptr){

//     qr_data *data =(qr_data*)ptr;
//     std::complex<double> *A = data->A;
//     std::complex<double> *Q = data->Q;
//     std::complex<double> *R = data->R;
//     int row = data->row; 
//     int col = data->col;

   

//     std::complex<double> *Q_col = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     std::complex<double> *vector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     std::complex<double> *Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     std::complex<double> *power_cur = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     std::complex<double> *power_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     std::complex<double> *proj_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     std::complex<double> *proj_Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));


//     // time initial
//     //float timeStart, timeEnd;
//     struct timespec start_multi_qr_work, end_multi_qr_work;

//     for(int i = 0; i < row * col; i += (col + 1)) {
//         Q[i].real(1);
//         R[i].real(1);
//     }
//     // timestamp start
//     //timeStart = clock();
//     clock_gettime(CLOCK_MONOTONIC, &start_multi_qr_work); 
//     for(int i = 0; i < col; ++i) {
//         for(int m = 0; m < row; ++m) {
//             Q[m * col + i] = A[m * col + i];
//         }

        
//         complex_matrix_get_columns(Q, Q_col, row, col, i);
//         // conjuate Q_col
//         complex_matrix_conjugate_transpose(Q_col, row, 1);
//         memset(power_cur, 0, sizeof(std::complex<double>));
//         complex_matrix_conjugate_transpose_multiplication(Q_col, power_cur, 1, row);
//         power_cur[0] = sqrt(power_cur[0]);
//         if(i > 0) {
//             complex_matrix_get_columns(A, vector_cur, row, col, i);
//             std::complex<double> *Q_col_proj = (std::complex<double>*)malloc(row * i * sizeof(std::complex<double>));
//             std::complex<double> *proj_vector = (std::complex<double>*)malloc(i * sizeof(std::complex<double>));
//             memset(proj_vector, 0, i * sizeof(std::complex<double>));
//             for(int j = 0; j < i; ++j) {
//                 for(int m = 0; m < row; ++m) {
//                     Q_col_proj[m * i + j] = Q[m * col + j];
//                 }
//             }
//             complex_matrix_conjugate_transpose(Q_col_proj, row, i);
//             complex_matrix_multiplication(Q_col_proj, vector_cur, proj_vector, i, row, 1);
//             complex_matrix_conjugate_transpose(Q_col_proj, i, row);
//             memset(Q_col, 0, row * 1 * sizeof(std::complex<double>));
//             complex_matrix_multiplication(Q_col_proj, proj_vector, Q_col, row, i, 1);
//             complex_matrix_subtraction(vector_cur, Q_col, row, 1);
//             for(int m = 0; m < row; ++m) {
//                 Q[m * col + i] = vector_cur[m];
//             }
//             for(int j = 0; j < i; ++j) {
//                 R[i + col * j] = proj_vector[j];
//             }
//             free(Q_col_proj);
//             free(proj_vector);
//         }
//         complex_matrix_get_columns(Q, Q_col, row, col, i);
//         // conjuate Q_col
//         complex_matrix_conjugate_transpose(Q_col, row, 1);
//         memset(power_val, 0, sizeof(std::complex<double>));
//         complex_matrix_conjugate_transpose_multiplication(Q_col, power_val, 1, row);
//         power_val[0] = sqrt(power_val[0]);

//         //1e-4 = 0.0001
//         if(power_val[0].real() / power_cur[0].real() < 1e-4) {
//             R[i * row + i] = 0;
//             // span again
//             for(int m = 0; m < row; ++m) {
//                 Q[m * col + i] = 0;
//             }
//             Q[i * row + i].real(1);
//             complex_matrix_get_columns(Q, vector_cur, row, col, i);
//             for(int j = 0; j < i; ++j) {
//                 complex_matrix_get_columns(Q, Qvector_cur, row, col, j);
//                 memset(proj_val, 0, sizeof(std::complex<double>));
//                 complex_matrix_conjugate_transpose(Qvector_cur, row, 1);
//                 complex_matrix_multiplication(Qvector_cur, vector_cur, proj_val, 1, row, 1);
//                 complex_matrix_conjugate_transpose(Qvector_cur, 1, row);
//                 complex_matrix_get_columns(Q, Q_col, row, col, i);
//                 memset(proj_Qvector_cur, 0, row * 1 * sizeof(std::complex<double>));
//                 complex_matrix_multiplication(Qvector_cur, proj_val, proj_Qvector_cur, row, 1, 1);
//                 complex_matrix_subtraction(Q_col, proj_Qvector_cur, row, 1);
//                 for(int m = 0; m < row; ++m) {
//                     Q[m * col + i] = Q_col[m];
//                 }
//             }
//             complex_matrix_get_columns(Q, Q_col, row, col, i);
//             complex_matrix_conjugate_transpose(Q_col, row, 1);
//             memset(power_val, 0, sizeof(std::complex<double>));
//             complex_matrix_conjugate_transpose_multiplication(Q_col, power_val, 1, row);
//             power_val[0] = sqrt(power_val[0]);
//             complex_matrix_conjugate_transpose(Q_col, 1, row);
//             for(int m = 0; m < row; ++m) {
//                 Q[m * col + i] /= power_val[0]; // Q[m * col + i] = Q[m * col + i] / power_val[0]
//             }
//         } else {
//             R[i * row + i] = power_val[0];
//             for(int m = 0; m < row; ++m) {
//                 Q[m * col + i] /= power_val[0];
//             }
//         }
//     }
//     // timestamp start
//     clock_gettime(CLOCK_MONOTONIC, &end_multi_qr_work); 
//     //timeEnd = clock();

//     #ifdef PRINT_RESULT
//     std::cout << std::setprecision(6) << "QR decomposer:\t\t" << (end_multi_qr_work.tv_nsec - start_multi_qr_work.tv_nsec) *1.0 / 1000 << " us" << std::endl;
//     //std::cout << std::setprecision(6) << "QR decomposer:\t\t" << (timeEnd - timeStart) / CLOCKS_PER_SEC * 1000 << " ms" << std::endl;
//     #endif

//     data->Q = Q;
//     data->R = R;

//     pthread_exit(NULL);

//     free(Q_col);
//     free(vector_cur);
//     free(Qvector_cur);
//     free(power_cur); 
//     free(power_val);
//     free(proj_val);
//     free(proj_Qvector_cur);

// }





// void multi_qr(std::complex<double> *A,
//               std::complex<double> *Q, 
//               std::complex<double> *R, 
//               int row, 
//               int col){

//     pthread_t thread[NUM_THREADS];
//     qr_data data; 
//     struct timespec start_multi_qr, end_multi_qr;

//     // row = data.row;
//     // col = data.col;
//     data.row = row;
//     data.col = col;
    
//     data.A=A;
//     data.Q=Q;
//     data.R=R;


//     // std::complex<double> *Q_col = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     // std::complex<double> *vector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     // std::complex<double> *Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));
//     // std::complex<double> *power_cur = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     // std::complex<double> *power_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     // std::complex<double> *proj_val = (std::complex<double>*)malloc(sizeof(std::complex<double>));
//     // std::complex<double> *proj_Qvector_cur = (std::complex<double>*)malloc(row * 1 * sizeof(std::complex<double>));

//     // for(int i = 0; i < row * col; i += (col + 1)) {
//     //     Q[i].real(1);
//     //     R[i].real(1);
//     // }

//     clock_gettime(CLOCK_MONOTONIC, &start_multi_qr); 

//     for (int i = 0; i < NUM_THREADS ; i++){
//         pthread_create(&thread[i],NULL,multi_qr_work,(void *)&data);
//         pthread_join(thread[i], NULL);
//     }
    
//     clock_gettime(CLOCK_MONOTONIC, &end_multi_qr);
    
//     #ifdef PRINT_RESULT
//     std::cout << std::setprecision(6) << "QR decomposer:\t\t" << (end_multi_qr.tv_nsec - start_multi_qr.tv_nsec) *1.0 / 1000 << " us" << std::endl;
//     #endif

//     Q = data.Q;
//     R = data.R;

    
// }


/* //min test
extern "C" 
void qr_test(double *matA ,int rowA, int colA) {
    
    std::complex<double> *A=(std::complex<double>*)malloc(rowA* colA * sizeof(std::complex<double>));
    std::complex<double> *Q=(std::complex<double>*)malloc(rowA* colA * sizeof(std::complex<double>));
    std::complex<double> *R=(std::complex<double>*)malloc(rowA* colA * sizeof(std::complex<double>));
    for (int i = 0; i < rowA*colA; i++)
    {
        A[i].real(matA[i]);
    }
    
    qr(A,Q,R,2,2);
    printf("A:\n");
    print_complex_matrix(A ,rowA,colA);
    printf("Q:\n");
    print_complex_matrix(Q ,rowA,colA);
    printf("R:\n");
    print_complex_matrix(R ,rowA,colA);
    
    free(A);
    free(Q);
    free(R);

}
*/






/*

int main(void){

    global_antenna = 64;
    global_QR_iteration = 5;
    global_multi_input = 8;
    global_SNR = 100;
    //global_music_type = 0; //0:MUSIC;1:MVDR
    float result[3] = {0};
    //music_mvdr_init(global_music_type,result); 

    MUSIC_DOA_1D_CPU(global_antenna,
                        global_SNR,
                        global_QR_iteration,
                        global_multi_input,
                        result
                    );


    //--------------------------------------------------------------//

    // int row =8;
    // int col =8;

    // std::complex<double> A[]={ 0,  1,  2,  3,   0,  1,  2,  3,
    //                           4,  5,  6,  7,   4,  5,  6,  7,
    //                           8,  9, 10, 11,   8,  9, 10, 11,
    //                           12, 13, 14, 15, 12, 13, 14, 15,
    //                           0,  1,  2,  3,   0,  1,  2,  3,
    //                           4,  5,  6,  7,   4,  5,  6,  7,
    //                           8,  9, 10, 11,   8,  9, 10, 11,
    //                           12, 13, 14, 15, 12, 13, 14, 15
    //                         };

    // std::complex<double> B[]={ 16, 17, 18, 19, 16, 17, 18, 19,
    //                           20, 21, 22, 23, 20, 21, 22, 23,
    //                           24, 25, 26, 27, 24, 25, 26, 27,
    //                           28, 29, 30, 31, 28, 29, 30, 31,
    //                           16, 17, 18, 19, 16, 17, 18, 19,
    //                           20, 21, 22, 23, 20, 21, 22, 23,
    //                           24, 25, 26, 27, 24, 25, 26, 27,
    //                           28, 29, 30, 31, 28, 29, 30, 31
    //                         };

        
    int row = 64;
    int col = 64;
    
    std::complex<double> *A=(std::complex<double>*)malloc(row* col * sizeof(std::complex<double>));
    std::complex<double> *B=(std::complex<double>*)malloc(row* col * sizeof(std::complex<double>));
    
    fill( A, A+4096, 10);
    fill( B, B+4096, 10);

    std::complex<double> *C=(std::complex<double>*)malloc(row* col * sizeof(std::complex<double>));
    std::complex<double> *D=(std::complex<double>*)malloc(row* col * sizeof(std::complex<double>));
    
    float timestart_t, timeend_t;
    float timestart, timeend;
    
    timestart_t = clock();
    complex_matrix_multiplication_sub_matrix(A,B,C,row,row,col);
    timeend_t = clock();

    timestart = clock();
    complex_matrix_multiplication(A,B,D,row,row,col);
    timeend = clock();

    std::cout <<"\n";
    print_complex_matrix(C,row,col);
    //std::cout <<"\n"<<"\n";
    //std::cout <<"----------------------------------------------"<< std ::endl;
    //std::cout <<"correct"<< std ::endl;
    //print_complex_matrix(D,row,col);


    std::cout << std::setprecision(6) << "complex_matrix_multiplication (cpu):\t" << (timeend - timestart) / CLOCKS_PER_SEC  * 1000 << " ms" << std::endl;
    std::cout << std::setprecision(6) << "complex_matrix_multiplication_sub (cpu):\t" << (timeend_t - timestart_t) / CLOCKS_PER_SEC * 1000  << " ms" << std::endl;
    
    free(A);
    free(B);
    free(C);
    free(D);

    

    return 0;
    
}
*/


