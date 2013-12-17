/*********************************************************************************
  *Copyright(C),2013-2014,Yinyuetai.com
  *FileName:  // speex_decode.h
  *Author:  //chris
  *Version:  //0.0.1
  *Date:  //2013/11/7
  *Description:  //define the interface of speex to implement decode encoded speex data
**********************************************************************************/
#ifndef SPEEX_ENCODE_H_
#define SPEEX_ENCODE_H_
#define FRAME_SIZE 160
//#pragma comment (lib ,"libspeex.lib")
#include <speex/speex.h>
#include "yyt_type.h"
#include "rtp_header.h"
typedef struct SPEEX_DECODE_UNION{

	/*Holds the state of the decoder*/
	void *state;

	/*Holds bits so they can be read and written to by the Speex routines*/
	SpeexBits bits;

	rtp_header_t rtp_header;
}speex_decode_union_t;

YYT_HANDLE_T  spx_decode_init();	//LP64

int spx_decode_frame(YYT_HANDLE_T handle ,char *src_speex_data ,short *target_pcm_data_buffer);


int spx_decode_destroy(YYT_HANDLE_T handle );

uint32_t get_rtp_stamp(unsigned char *src_speex_data);

#endif //speex_encode.h
