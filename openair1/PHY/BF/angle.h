
 /*! \file openairinterface5g/openair1/PHY/BF/angle.h
 * \brief Integrate MUSIC & MVDR algorithm single user Op7.2
 * \author NYCU OpinConnect Sendren Xu, Terng-Yin Hsu, Ming-Hsun Wu, Chao-Hung Hsu
 * \email  sdxu@mail.ntust.edu.tw, tyhsu@cs.nctu.edu.tw, sam0104502@gmail.com, abby88771@gmail.com
 * \date   28-9-2022
 * \version 1.3
 * \note
 * \warning
 */

#ifndef __ANGLE_H
#define __ANGLE_H

#ifdef __cplusplus
extern "C" {  
#endif
    
    int global_antenna;
    int global_QR_iteration;
    //int global_total_round;
    int global_angle;
    // int global_type;
    int global_SNR;
    int global_DOA_algorithm;

    //single user
    void beam_weight_int16_t(int16_t *bws, int M, int angle);
    void beam_weight_int8_t(int8_t *bws, int M, int angle);
    void MUSIC_init_TD(int16_t *rx_data, float *result);
    void MVDR_init_TD(int16_t *rx_data, float *result);
    void MUSIC_init_FD(int32_t *rx_data, float *result);
    void MVDR_init_FD(int32_t *rx_data, float *result);

#ifdef __cplusplus  
} // extern "C"  
#endif

#endif