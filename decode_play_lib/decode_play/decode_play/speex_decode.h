///*********************************************************************************
//  *Copyright(C),2013-2014,Yinyuetai.com
//  *FileName:  // speex_decode.h
//  *Author:  //chris
//  *Version:  //0.0.1
//  *Date:  //2013/11/7
//  *Description:  //define the interface of speex to implement decode encoded speex data
//**********************************************************************************/
#ifndef SPEEX_ENCODE_H_
#define SPEEX_ENCODE_H_

//#pragma comment (lib ,"libspeex.lib")
//
#include <speex/speex.h>
// SPEEX_DECODE_UNION
typedef struct {

	/*Holds the state of the decoder*/
	void *state;

	/*Holds bits so they can be read and written to by the Speex routines*/
	SpeexBits bits;

	short	speex_data[160];
	char	pcm_data[320];

}speex_decode_union_t;

int  spx_decode_init();

int spx_decode_frame(int handle ,char *src_speex_data ,short *target_pcm_data_buffer);


int spx_destroy(int handle );

#endif //speex_encode.h
