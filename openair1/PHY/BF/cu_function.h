#ifndef __CU_FUNCTION_H
#define __CU_FUNCTION_H

#define  NUM_THREADS 3
#define  RADAR_SAMPLE 500

#ifdef __cplusplus
extern "C" {  
#endif

    int global_antenna;
    int global_QR_iteration;
    int global_total_round;
    int global_angle;
    int global_type;
    int global_SNR;
    int global_multi_input;
    int global_RA;

    //void MUSIC_init(int subframe, uint32_t *rx_data, float *result);

    //int global_QR_count;
    //void qr_test(double *matA ,int rowA, int colA);
    //void MUSIC_DOA_1D_CPU(int M, int snr, int qr_iter, int multi_input, float *result);
    //void MVDR_DOA_1D_CPU(int M, int snr, int qr_iter, int multi_input, float *result);
    //void music_mvdr_init(int global_music_type,float *result);

#ifdef __cplusplus  
} // extern "C"  
#endif

#endif