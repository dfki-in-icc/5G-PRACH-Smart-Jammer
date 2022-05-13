/*******************************************************************************
 *
 * <COPYRIGHT_TAG>
 *
 *******************************************************************************/

#ifndef XRAN_LIB_WRAP_HPP
#define XRAN_LIB_WRAP_HPP

#include <exception>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <malloc.h>
#include <stdint.h>

#include "common.hpp"
#include "xran_fh_o_du.h"
#include "xran_common.h"
#include "xran_frame_struct.h"


#define XRAN_UT_CFG_FILENAME            "conf.json"

#define XRAN_UT_KEY_GLOBALCFG           "GLOBAL"
#define XRAN_UT_KEY_GLOBALCFG_IO        "io_cfg"
#define XRAN_UT_KEY_GLOBALCFG_EAXCID    "eAxCId_cfg"
#define XRAN_UT_KEY_GLOBALCFG_PRACH     "prach_cfg"
#define XRAN_UT_KEY_GLOBALCFG_RU        "ru_cfg"
#define XRAN_UT_KEY_GLOBALCFG_SLOT      "slotcfg_"

#define MAX_NUM_OF_XRAN_CTX             (2)

#define SW_FPGA_TOTAL_BUFFER_LEN        (4*1024*1024*1024)
#define SW_FPGA_SEGMENT_BUFFER_LEN      (1*1024*1024*1024)
#define SW_FPGA_FH_TOTAL_BUFFER_LEN     (1*1024*1024*1024)
#define FPGA_TO_SW_PRACH_RX_BUFFER_LEN  (8192)

#define MAX_ANT_CARRIER_SUPPORTED (XRAN_MAX_SECTOR_NR*XRAN_MAX_ANTENNA_NR)

extern "C"
{
extern uint32_t xran_lib_ota_tti;
extern uint32_t xran_lib_ota_sym;
extern uint32_t xran_lib_ota_sym_idx;

void sym_ota_cb(struct rte_timer *tim, void *arg);
void tti_ota_cb(struct rte_timer *tim, void *arg);
}

class xranLibWraper
{
public:
    typedef enum
    {
        XRANFTHTX_OUT = 0,
        XRANFTHTX_PRB_MAP_OUT,
        XRANFTHTX_SEC_DESC_OUT,
        XRANFTHRX_IN,
        XRANFTHRX_PRB_MAP_IN,
        XRANFTHTX_SEC_DESC_IN,
        XRANFTHRACH_IN,
#if 1
        XRANSRS_IN, //  Added this entry as in sample app
#endif
        MAX_SW_XRAN_INTERFACE_NUM
    } SWXRANInterfaceTypeEnum;

    enum nChBw
    {
        PHY_BW_5MHZ   =   5, PHY_BW_10MHZ  =  10, PHY_BW_15MHZ  =  15,
        PHY_BW_20MHZ  =  20, PHY_BW_25MHZ  =  25, PHY_BW_30MHZ  =  30,
        PHY_BW_40MHZ  =  40, PHY_BW_50MHZ  =  50, PHY_BW_60MHZ  =  60,
        PHY_BW_70MHZ  =  70, PHY_BW_80MHZ  =  80, PHY_BW_90MHZ  =  90,
        PHY_BW_100MHZ = 100, PHY_BW_200MHZ = 200, PHY_BW_400MHZ = 400
    };

    // F1 Tables 38.101-1 Table 5.3.2-1. Maximum transmission bandwidth configuration NRB
    const uint16_t nNumRbsPerSymF1[3][13] =
    {
    //      5MHz   10MHz   15MHz   20MHz   25MHz   30MHz   40MHz   50MHz   60MHz   70MHz   80MHz   90MHz  100MHz
        {    25,     52,     79,    106,    133,    160,    216,    270,      0,      0,      0,      0,      0 },  // Numerology 0 (15KHz)
        {    11,     24,     38,     51,     65,     78,    106,    133,    162,      0,    217,    245,    273 },  // Numerology 1 (30KHz)
        {     0,     11,     18,     24,     31,     38,     51,     65,     79,      0,    107,    121,    135 }   // Numerology 2 (60KHz)
    };

    // F2 Tables 38.101-2 Table 5.3.2-1. Maximum transmission bandwidth configuration NRB
    const uint16_t nNumRbsPerSymF2[2][4] =
    {
    //     50MHz  100MHz  200MHz  400MHz
        {    66,    132,    264,      0 },  // Numerology 2 (60KHz)
        {    32,     66,    132,    264 }   // Numerology 3 (120KHz)
    };


protected:
    char argv[25] = {'u','n','i','t','t','e','s','t','\0'};
    //char argv[25] = "unittest";

    std::string m_dpdk_dev_up, m_dpdk_dev_cp, m_dpdk_bbdev;

    void *m_xranhandle;

    uint8_t m_du_mac[6]; // = { 0x00,0x11, 0x22, 0x33, 0x44, 0x55 }; // Sofia: this is hard coded here and then it is read from the conf file
    uint8_t m_ru_mac[XRAN_VF_MAX][6]; //= { 0x00,0x11, 0x22, 0x33, 0x44, 0x66 }; // Sofia: this is hard coded here and then it is read from the conf file
    bool m_bSub6;
    uint32_t m_nSlots; // = 20;

    struct xran_fh_config   m_xranConf;
    struct xran_fh_init     m_xranInit;

    struct xran_timer_ctx {
        uint32_t    tti_to_process;
    } m_timer_ctx[MAX_NUM_OF_XRAN_CTX];

    /* io struct */
    BbuIoBufCtrlStruct m_sFrontHaulTxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    BbuIoBufCtrlStruct m_sFrontHaulTxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    BbuIoBufCtrlStruct m_sFrontHaulRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    BbuIoBufCtrlStruct m_sFrontHaulRxPrbMapBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    BbuIoBufCtrlStruct m_sFHPrachRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];


    /* Cat B */
    BbuIoBufCtrlStruct m_sFHSrsRxBbuIoBufCtrl[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR];

    /* buffers lists */
    struct xran_flat_buffer m_sFrontHaulTxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    struct xran_flat_buffer m_sFrontHaulTxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_flat_buffer m_sFrontHaulRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];
    struct xran_flat_buffer m_sFrontHaulRxPrbMapBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR];
    struct xran_flat_buffer m_sFHPrachRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANTENNA_NR][XRAN_NUM_OF_SYMBOL_PER_SLOT];

    /* Cat B SRS buffers */
    struct xran_flat_buffer m_sFHSrsRxBuffers[XRAN_N_FE_BUF_LEN][XRAN_MAX_SECTOR_NR][XRAN_MAX_ANT_ARRAY_ELM_NR][XRAN_MAX_NUM_OF_SRS_SYMBOL_PER_SLOT];

    void    *m_nInstanceHandle[XRAN_PORTS_NUM][XRAN_MAX_SECTOR_NR]; // instance per sector
    uint32_t m_nBufPoolIndex[XRAN_MAX_SECTOR_NR][MAX_SW_XRAN_INTERFACE_NUM];   // every api owns unique buffer pool

    uint32_t m_nSW_ToFpga_FTH_TxBufferLen;
    uint32_t m_nFpgaToSW_FTH_RxBufferLen;

    int32_t m_nSectorIndex[XRAN_MAX_SECTOR_NR];

    int iq_bfw_buffer_size_dl = 0;
    int iq_bfw_buffer_size_ul = 0;

    /* beamforming weights for UL (O-DU) */
    int16_t *p_tx_dl_bfw_buffer[MAX_ANT_CARRIER_SUPPORTED];
    int32_t tx_dl_bfw_buffer_size[MAX_ANT_CARRIER_SUPPORTED];
    int32_t tx_dl_bfw_buffer_position[MAX_ANT_CARRIER_SUPPORTED];

    /* beamforming weights for UL (O-DU) */
    int16_t *p_tx_ul_bfw_buffer[MAX_ANT_CARRIER_SUPPORTED];
    int32_t tx_ul_bfw_buffer_size[MAX_ANT_CARRIER_SUPPORTED];
    int32_t tx_ul_bfw_buffer_position[MAX_ANT_CARRIER_SUPPORTED];


private:
    json m_global_cfg;

    template<typename T>
    T get_globalcfg(const std::string &type, const std::string &parameter_name)
    {
        return m_global_cfg[XRAN_UT_KEY_GLOBALCFG][type][parameter_name];
    }

    template<typename T>
    std::vector<T> get_globalcfg_array(const std::string &type, const std::string &parameter_name)
    {
        auto array_size = m_global_cfg[XRAN_UT_KEY_GLOBALCFG][type][parameter_name].size();

        std::vector<T> result(array_size);

        for(unsigned number = 0; number < array_size; number++)
            result.at(number) = m_global_cfg[XRAN_UT_KEY_GLOBALCFG][type][parameter_name][number];

        return result;
    }

    uint16_t get_eaxcid_mask(int numbit, int shift);

    int init_memory();

public:
    // Class constructor
    xranLibWraper();

    // Class Distructor
    ~xranLibWraper();

    int SetUp();

    void TearDown();
   
    int Init(struct xran_fh_config *pCfg = nullptr); 

    void Cleanup();  

    void Open(xran_ethdi_mbuf_send_fn send_cp, xran_ethdi_mbuf_send_fn send_up,
            void *fh_rx_callback, void *fh_rx_prach_callback, void *fh_srs_callback); 

    void Close(); 

    int Start(); 

    int Stop(); 

    /* emulation of timer */
    void update_tti(); 

    void update_symbol_index(); 

    int apply_cpenable(bool flag); 

    int get_slot_config(const std::string &cfgname, struct xran_frame_config *pCfg); 

    int get_num_rbs(uint32_t nNumerology, uint32_t nBandwidth, bool nSub6);
    
    void *get_xranhandle();

    void *get_timer_ctx();

    int get_symbol_index();

    bool is_running();

    enum xran_category get_rucategory();

    int get_numerology();

    int get_duplextype();

    uint32_t get_num_cc();

    uint32_t get_num_eaxc();

    uint32_t get_num_eaxc_ul();

    uint32_t get_num_dlrbs();

    uint32_t get_num_ulrbs();

    uint32_t get_num_antelmtrx();

    bool is_cpenable();

    bool is_prachenable();

    bool is_dynamicsection();
    
    bool get_sub6();

    void get_cfg_prach(struct xran_prach_config *pCfg); 

    void get_cfg_frame(struct xran_frame_config *pCfg); 

    void get_cfg_ru(struct xran_ru_config *pCfg); 

    void get_cfg_fh(struct xran_fh_config *pCfg);

};


/* external declaration for the instance */
extern xranLibWraper *xranlib;


#endif //XRAN_LIB_WRAP_HPP
