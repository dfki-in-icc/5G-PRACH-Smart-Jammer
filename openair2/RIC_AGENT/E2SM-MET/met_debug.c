#include <stdio.h>
#include <stddef.h>

#include "met_debug.h"

#include "E2SM_MET_MeasurementRecord.h"
#include "E2SM_MET_MeasurementRecord.h"
#include "E2SM_MET_MeasurementRecordItem.h"

void encode_decode(const struct asn_TYPE_descriptor_s *td, const void *struct_ptr,const void *empty_struct_ptr );
void test_met(void);


void test_met(void){

    // creating measurment record item 
    E2SM_MET_MeasurementRecordItem_t *mri = (E2SM_MET_MeasurementRecordItem_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecordItem_t));
    mri->choice.integer = 10;
    mri->present = E2SM_MET_MeasurementRecordItem_PR_integer;


    //creating measurment record and affecting UEID and the UETAG 
    E2SM_MET_MeasurementRecord_t *deb_mr = (E2SM_MET_MeasurementRecord_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecord_t));
    deb_mr->ueID = 11;
    // E2SM_MET_UETag_t	*ueTag = (E2SM_MET_UETag_t *)calloc(1, sizeof(E2SM_MET_UETag_t));
    // int ret22 = OCTET_STRING_fromString(ueTag,"AAAA");
    // if (ret22  != 0) exit(-1);
    // fprintf(stderr, " the %s  ---- size : %d \n\n", ueTag->buf, ueTag->size);
    // deb_mr->ueTag = ueTag;

    deb_mr->ueTag.buf  = (uint8_t *)strdup("AA");
    deb_mr->ueTag.size = strlen("AA");

    //adding reacord item to the list of measrecords 
    int ret = ASN_SEQUENCE_ADD(&deb_mr->measRecordItem.list, mri);
    if (ret != 0) exit(-1);


    // tmp record for debug function
    E2SM_MET_MeasurementRecord_t *mritmp = (E2SM_MET_MeasurementRecord_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecord_t));

    E2SM_MET_MeasurementRecordItem_t *mri0 = (E2SM_MET_MeasurementRecordItem_t *)calloc(1, sizeof(E2SM_MET_MeasurementRecordItem_t));

    encode_decode(&asn_DEF_E2SM_MET_MeasurementRecord,deb_mr,mritmp);  

}


void encode_decode(const struct asn_TYPE_descriptor_s *td, const void *struct_ptr,const void *empty_struct_ptr ){

    fprintf(stderr,"start-encode decode------------------  --------------------\n");

    uint8_t buffer[8192];
    size_t buffer_size = 8192;

    //encoding
    asn_enc_rval_t ret_enc = asn_encode_to_buffer(
        0,
        ATS_ALIGNED_BASIC_PER,
        td,
        struct_ptr,
        buffer,
        buffer_size);

    if (ret_enc.encoded < 0) {
        fprintf(stderr, "ERROR encoding, name=%s, tag=%s", ret_enc.failed_type->name, ret_enc.failed_type->xml_tag);
    }


    xer_fprint(stderr, td,struct_ptr);

    // uint8_t buffer2[8192] = {0x00, 0x09, 0x02, 0x00, 0x41, 0x41, 0x41, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x14};
    for (int i = 0; i < ret_enc.encoded; i++)
    {
        fprintf(stderr, "0x%02x,", buffer[i]);
    }
    fprintf(stderr," encoded is : %ld \n\n",ret_enc.encoded);

    //decoding 
    asn_dec_rval_t er = asn_decode(0, ATS_ALIGNED_BASIC_PER,td,(void **) &empty_struct_ptr, buffer, ret_enc.encoded );
    if (er.code != RC_OK) {
        fprintf(stderr, "\tERROR decoding - consumed : %ld - code %d\n ",er.consumed,er.code);
    }else{
        fprintf(stderr,"\t  -------   start decoding ------------\n");
        xer_fprint(stderr, td ,empty_struct_ptr);
    }

    fprintf(stderr,"\tend-encode-decode------------------  --------------------\n");

}



void log_decode_buf_content(const void *buffer,size_t buf_len,void **struct_to_decode_and_print,const struct asn_TYPE_descriptor_s *type_to_decode, char title[] ){
    
    fprintf(stderr,"\tstart-decode------------------ %s --------------------\n",title);

    ///hard coded for measItem 

E2SM_MET_MeasurementRecordItem_t *mr = (E2SM_MET_MeasurementRecordItem_t *)calloc(1,sizeof(E2SM_MET_MeasurementRecordItem_t));
    asn_dec_rval_t er = asn_decode(0, ATS_ALIGNED_BASIC_PER,&asn_DEF_E2SM_MET_MeasurementRecordItem,(void **) &mr, buffer, buf_len);
    if (er.code != RC_OK) {
        fprintf(stderr, "\tERROR decoding - consumed : %ld - code %d\n ",er.consumed,er.code);
    }else{
        fprintf(stderr,"\t  -------   start decoding ------------\n");
        xer_fprint(stderr, &asn_DEF_E2SM_MET_MeasurementRecord,mr);
    }


    fprintf(stderr,"\tend-decode------------------ %s --------------------\n",title);

}