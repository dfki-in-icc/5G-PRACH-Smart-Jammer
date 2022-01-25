
 /*! \file openairinterface5g/openair1/PHY/BF/angle.h
 * \brief Integrate MUSIC algorithm single user Op8
 * \author NYCU OpinConnect Terng-Yin Hsu, Sendren Xu, Min-Hsun Wu
 * \email  tyhsu@cs.nctu.edu.tw, sdxu@mail.ntust.edu.tw, sam0104502@gmail.com
 * \date   25-1-2022
 * \version 1.1
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
    int global_total_round;
    int global_angle;
    int global_type;
    int global_SNR;
    int global_DOA_algorithm;

    void beam_weight_int16_t(int16_t *bws, int M, int angle);
    void beam_weight_int8_t(int8_t *bws, int M, int angle);
    void MUSIC_init_16(int16_t *rx_data, float *result);

#ifdef __cplusplus  
} // extern "C"  
#endif

#endif