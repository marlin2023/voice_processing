/*********************************************************************************
  *Copyright(C),2013-2014,Yinyuetai.com
  *FileName:  // micro_ctrol.cpp
  *Author:  //chris
  *Version:  //0.0.1
  *Date:  //2013/11/6
  *Description:  //use mixer api to implement control the volume of the microphone
**********************************************************************************/

/*
 * speex_encode.c
 *
 *  Created on: Jun 18, 2013
 *      Author: chris
 */

#include <speex/speex.h>
#include <speex/speex_preprocess.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <arpa/inet.h>
#include <winsock.h>

#include "speex_encode.h"
//#pragma comment (lib ,"libspeex.lib")

///*The frame size in hardcoded for this sample code but it doesn��t have to be*/
#define FRAME_SIZE 160
#define TAG "SPEEX_ENCODE_C"

/*rtpsession.c*/
static void rtp_header_init_from_session(rtp_header_t *rtp ,uint32_t seq_number_in_ms){ //first time  seq_number is set to 0 ,others is time_diff in millisecond
        rtp->version = 2;
        rtp->padbit = 0;
        rtp->extbit = 0;
        rtp->markbit= 0;
        rtp->cc = 0;
        rtp->paytype = SPEEX_PAYLOAD_TYPE;//session->snd.pt;
        rtp->ssrc = 0;//set later
        //rtp->timestamp = 0;        // first time is 0
        rtp->timestamp = (seq_number_in_ms * 8 ) &( (2<<31) -1);  // unit: in samples 20ms = 160 samples ,and 1ms have 8 samples
        /* set a seq number */
        rtp->seq_number=0;
}


int  spx_encode_init(uint32_t seq_number_in_ms){

    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)malloc(sizeof(speex_encode_union_t));
    if(speex_encode_u == NULL){
        printf("speex_encode_union malloc failed ...\n");
        exit(1);
    }

        /*Create a new encoder state in narrowband mode*/
   speex_encode_u->state = speex_encoder_init(&speex_nb_mode);

    /*Set the quality to 8 (15 kbps)*/
    int tmp = 8;
    speex_encoder_ctl(speex_encode_u->state, SPEEX_SET_QUALITY, &tmp);

    /*Initialization of the structure that holds the bits*/
    speex_bits_init(&speex_encode_u->bits);

    rtp_header_init_from_session(&speex_encode_u->rtp_header ,seq_number_in_ms);

    //add
    //add
    speex_encode_u->m_st = speex_preprocess_state_init(160, 8000);  //160 samples = 8000*0.02
	int i=1;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_DENOISE, &i);
	int noiseSuppress = -20; //-20 ,-25
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress);
	i=0;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_AGC, &i);
	int f=16000;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &f);
	i=0;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
	f=.0;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
	f=.0;
	speex_preprocess_ctl(speex_encode_u->m_st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);

    return (int )speex_encode_u;
}


int spx_encode_frame(int handle ,short *const pcm_data ,char *speex_data){
    float input[FRAME_SIZE];
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

//    //把16bits的值转化为float,以便speex库可以在上面工作
//    spx_int16_t * ptr=(spx_int16_t *)pcm_data;
//
//    if (speex_preprocess_run(speex_encode_u->m_st, ptr))//预处理 打开了静音检测和降噪
//	{
////		printf("speech,");
////		fwrite(in, sizeof(short), FRAME_SIZE, fout3);
//	}

    /*Flush all the bits in the struct so we can encode a new frame*/
    speex_bits_reset(&speex_encode_u->bits);

    /*Encode the frame*/
    int i;
    for (i = 0; i < FRAME_SIZE; i++)
          input[i] = pcm_data[i];
    speex_encode(speex_encode_u->state, input, &speex_encode_u->bits);

    /*1. generate seq_number in rtp header in every packet	*/
    speex_encode_u->rtp_header.seq_number ++ ;
    uint16_t seq_number_tmp = speex_encode_u->rtp_header.seq_number;
    speex_encode_u->rtp_header.seq_number = htons(speex_encode_u->rtp_header.seq_number);
    /*2. generate timestamp in rtp header in every packet	*/
    speex_encode_u->rtp_header.timestamp += FRAME_SIZE;
    uint32_t time_stamp_tmp = speex_encode_u->rtp_header.timestamp;
    speex_encode_u->rtp_header.timestamp = htonl(speex_encode_u->rtp_header.timestamp);


    memcpy(speex_data ,&speex_encode_u->rtp_header ,RTP_HEADER_SIZE);   		//write rtp packet header!!!!!RTP HEADER

    /*	do sth after memcpy*/
    speex_encode_u->rtp_header.seq_number = seq_number_tmp;
    speex_encode_u->rtp_header.timestamp = time_stamp_tmp;

    /*The END: Copy the bits to an array of char that can be written*/
    int nbBytes = speex_bits_write(&speex_encode_u->bits, &speex_data[RTP_HEADER_SIZE], 38);
    
	return 0;
}

int spx_encode_destroy(int handle ){
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

    /*Destroy the encoder state*/
    speex_encoder_destroy(speex_encode_u->state);
    /*Destroy the bit-packing struct*/
    speex_bits_destroy(&speex_encode_u->bits);

    speex_preprocess_state_destroy(speex_encode_u->m_st);
    free(speex_encode_u);
	return 0;
}
