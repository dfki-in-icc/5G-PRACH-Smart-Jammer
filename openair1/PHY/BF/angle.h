 
 /*! \file openairinterface5g/openair1/PHY/BF/angle.h
 * \brief Integrate MUSIC & MVDR algorithm multi-user fix UE RB
 * \author NYCU OpinConnect Sendren Xu, Terng-Yin Hsu, Ming-Hsun Wu, Chao-Hung Hsu
 * \email  sdxu@mail.ntust.edu.tw, tyhsu@cs.nctu.edu.tw, sam0104502@gmail.com, abby88771@gmail.com
 * \date   17-11-2022
 * \version 1.0
 * \note
 * \warning
 */
#ifndef __ANGLE_H
#define __ANGLE_H

#define MU_USER_NB 4
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
    int global_multi_user_num;
    int global_multi_angle[MU_USER_NB];

    //single user
    void beam_weight_int16_t(int16_t *bws, int M, int angle);
    void beam_weight_int8_t(int8_t *bws, int M, int angle);
    void beam_weight_int32_t(int32_t *bws, int M, int angle);
    void MUSIC_init_16(int16_t *rx_data, float *result);
    void MVDR_init_16(int16_t *rx_data, float *result);
    void MUSIC_init_32(int32_t *rx_data, float *result);
    void MUSIC_init_FD(int32_t *rx_data, float *result);
    void MVDR_init_FD(int32_t *rx_data, float *result);

   //mu user
    void beam_weight_mu_user_int8_t(int8_t **bws, int M, int mu_user, int angle);
    void MUSIC_MU_init(int32_t **rxdataF, float *result , int angle);
    void MVDR_MU_init(int32_t **rxdataF, float *result, int angle);
    void MUSIC_MU_ue_rb_init(int32_t **rxdataF, float *result , int angle ,int ue_sc);
    void MVDR_MU_ue_rb_init(int32_t **rxdataF, float *result, int angle ,int ue_sc);


#ifdef __cplusplus  
} // extern "C"  
#endif

#endif