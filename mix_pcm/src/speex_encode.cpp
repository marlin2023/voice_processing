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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/timeb.h>
#include <sys/time.h>

#include "speex_encode.h"

///*The frame size in hardcoded for this sample code but it doesn��t have to be*/
#define FRAME_SIZE 160
#define TAG "SPEEX_ENCODE_C"

/*rtpsession.c*/
static void rtp_header_init_from_session(rtp_header_t *rtp ,uint16_t seq_number){
        rtp->version = 2;
        rtp->padbit = 0;
        rtp->extbit = 0;
        rtp->markbit= 0;
        rtp->cc = 0;
        rtp->paytype = SPEEX_PAYLOAD_TYPE;//session->snd.pt;
        rtp->ssrc = 0;//set later
        rtp->timestamp = 0;        //
        /* set a seq number */
        rtp->seq_number= seq_number;
}


YYT_HANDLE_T  spx_encode_init(){

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


    /**====compute ts for sequnce number====== **/
    //1second
    struct timeb tp;
    ftime(&tp);  ////tp.time  in seconds
    //2.millisecond
    struct timeval tv;
    //double time_start;
    gettimeofday(&tv ,NULL);
    //time_start =tv.tv_sec * 1000000 + tv.tv_usec;
    uint16_t seq_numb = (tp.time * 1000 + tv.tv_usec / 1000) & ( (2<<15) - 1);

    //add test_nu the first two bytes
    rtp_header_init_from_session(&speex_encode_u->rtp_header ,seq_numb);
    return (YYT_HANDLE_T )speex_encode_u;
}


int spx_encode_frame(YYT_HANDLE_T handle ,short *const pcm_data ,char *speex_data){
   // char cbits[200];
    float input[FRAME_SIZE];
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

    /*Flush all the bits in the struct so we can encode a new frame*/
    speex_bits_reset(&speex_encode_u->bits);

    /*Encode the frame*/
   // memcpy(input,pcm_data,FRAME_SIZE*2);
    int i;
    for (i = 0; i < FRAME_SIZE; i++)
          input[i] = pcm_data[i];
    speex_encode(speex_encode_u->state, input, &speex_encode_u->bits);
//    //write rtp packet header!!!!!RTP HEADER
//    memcpy(speex_data ,&speex_encode_u->rtp_header ,RTP_HEADER_SIZE);

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

    /*Copy the bits to an array of char that can be written*/
    speex_bits_write(&speex_encode_u->bits, &speex_data[RTP_HEADER_SIZE], 38);

	/*int nbBytes = speex_bits_write(&speex_encode_u->bits, speex_data, 38);
	fwrite(speex_data ,1 ,38 ,fp_speex_send);*/
	//printf("after speex_encode ,write nbBytes = %d  to file !\n" ,nbBytes);
	return 0;
}

int spx_encode_destroy(YYT_HANDLE_T handle ){
    speex_encode_union_t * speex_encode_u = (speex_encode_union_t *)handle;

    /*Destroy the encoder state*/
    speex_encoder_destroy(speex_encode_u->state);
    /*Destroy the bit-packing struct*/
    speex_bits_destroy(&speex_encode_u->bits);

    free(speex_encode_u);
	return 0;
}
