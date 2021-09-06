void oran_fh_if4p5_south_out(RU_t *ru,
			     int frame,
			     int slot,
			     uint64_t timestamp);

void oran_fh_if4p5_south_in(RU_t *ru,
			    int *frame,
			    int *slot);



typedef struct {
  eth_state_t           e;
  shared_buffers        buffers;
  rru_config_msg_type_t last_msg;
  int                   capabilities_sent;
  void                  *oran_priv;
} oran_eth_state_t;

