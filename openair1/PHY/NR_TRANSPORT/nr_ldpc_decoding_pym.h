
/*! \file PHY/NR_TRANSPORT/nr_ulsch_decoding.c
* \brief FPGA decoding each code block
* \author Sendren Xu, SY Yeh(fdragon), Hongming, Terng-Yin Hsu
* \date 2022-05-20
* \version 3.0
* \email: summery19961210@gmail.com
*/



#include <stdint.h>

typedef struct {
    unsigned char max_schedule;
    unsigned char SetIdx;
    int Zc;			 
    unsigned char numCB;		 
    unsigned char BG;		
    unsigned char max_iter;      
    int nRows;			 
    int numChannelLls;
    int numFillerBits;		
}DecIFConf;

int nrLDPC_decoder_FPGA_PYM(int8_t *buf,int8_t *buf_out,DecIFConf dec_conf);
// int nrLDPC_decoder_FPGA_PYM();