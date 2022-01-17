#ifndef __ANGLE_H
#define __ANGLE_H

#ifdef __cplusplus
extern "C" {  
#endif
    
    int global_antenna;
    int global_QR_iteration;
    int global_total_round;
    int global_angle;
    int global_type;
    int global_SNR;
    int global_RA;

    void beam_weight_out_theta(int16_t *bws, int th_theta_num, int16_t *th_theta_ex);
    //void beam_weight_s(int16_t *bws, int th_theta_num);
    void beam_weight_int16_t(int16_t *bws, int M, int angle);
    void beam_weight_int8_t(int8_t *bws, int M, int angle);
    // void MUSIC_init(uint32_t *rx_data, float *result);
    void MUSIC_init(int32_t *rx_data, float *result);
    void MUSIC_init_16(int16_t *rx_data, float *result);
    //void MUSIC_init(int16_t *rx_data, float *result);
    //void MUSIC_init_test(uint32_t *rx_data, float *result);

#ifdef __cplusplus  
} // extern "C"  
#endif

#endif