/*********************************************************************************
  *Copyright(C),2013-2014,Yinyuetai.com
  *FileName:  // speex_decode.cpp
  *Author:  //chris
  *Version:  //0.0.1
  *Date:  //2013/11/7
  *Description:  //implement decode encoded speex data
**********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "speex_decode.h"
///*The frame size in hardcoded for this sample code but it doesn��t have to be*/
//#define FRAME_SIZE 160

YYT_HANDLE_T  spx_decode_init(){
	speex_decode_union_t * speex_decode_u = (speex_decode_union_t *)malloc(sizeof(speex_decode_union_t));
	if(speex_decode_u == NULL){
		printf("speex_decode_union malloc failed ...\n");
		exit(1);
	}

	/*Create a new decoder state in narrowband mode*/
	speex_decode_u->state = speex_decoder_init(&speex_nb_mode);
	/*Set the perceptual enhancement on*/
	int tmp = 1;
	speex_decoder_ctl(speex_decode_u->state, SPEEX_SET_ENH, &tmp);

	/*Initialization of the structure that holds the bits*/
	speex_bits_init(&speex_decode_u->bits);

	return (YYT_HANDLE_T )speex_decode_u;
}


int spx_decode_frame(YYT_HANDLE_T handle ,char *src_speex_data ,short *target_pcm_data_buffer){
	speex_decode_union_t * speex_decode_u = (speex_decode_union_t *)handle;

	/*Speex handle samples as float, so we need an array of floats*/
	float output[FRAME_SIZE];
	//add
	speex_bits_reset(&speex_decode_u->bits);
	int nbBytesSpeex = 38;
	/*Copy the data into the bit-stream struct*/  //speex_data -> receive speex audio data
	speex_bits_read_from(&speex_decode_u->bits, &src_speex_data[12], nbBytesSpeex); //rtp header size 12 bytes
	/*Decode the data*/
	int ret = speex_decode(speex_decode_u->state, &speex_decode_u->bits, output); //here ,is float data
	if(ret != 0 ){return -1;}

	/*Copy from float to short (16 bits) for output*/
	int i ;
	for (i = 0; i < FRAME_SIZE; i++)
		target_pcm_data_buffer[i] = output[i];

//	//read rtp packet header!!!!!RTP HEADER
//	memcpy(&speex_decode_u->rtp_header ,src_speex_data ,RTP_HEADER_SIZE);
	return 0;

}

int spx_decode_destroy(YYT_HANDLE_T handle ){
	speex_decode_union_t * speex_decode_u = (speex_decode_union_t *)handle;

	/*Destroy the decoder state*/
	speex_decoder_destroy(speex_decode_u->state);
	/*Destroy the bit-stream truct*/
	speex_bits_destroy(&speex_decode_u->bits);
	free(speex_decode_u);
	return 0;
}

uint32_t get_rtp_stamp(unsigned char *src_speex_data){

	rtp_header_t rtp_header;
	memcpy(&rtp_header ,src_speex_data ,RTP_HEADER_SIZE);
	return ntohl(rtp_header.timestamp);
}
