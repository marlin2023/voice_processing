/*********************************************************************************
  *Copyright(C),2013-2014,Yinyuetai.com
  *FileName:  // micro_ctrol.cpp
  *Author:  //chris
  *Version:  //0.0.1
  *Date:  //2013/11/6
  *Description:  //use mixer api to implement control the volume of the microphone
**********************************************************************************/
#ifndef _SPEEX_ENCODE_H_
#define _SPEEX_ENCODE_H_

#define SPEEX_PAYLOAD_TYPE        97

#include <stdint.h>
#include "yyt_type.h"
#include "rtp_header.h"


#include <speex/speex.h>
typedef struct SPEEX_ENCODE_UNION{

        /*Holds the state of the encoder*/
        void *state;

    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;

    rtp_header_t rtp_header;

}speex_encode_union_t;

YYT_HANDLE_T  spx_encode_init();

int spx_encode_frame(YYT_HANDLE_T handle ,short * const pcm_data ,char *speex_data);

int spx_encode_destroy(YYT_HANDLE_T handle );

#endif /* SPEEX_ENCODE_H_ */
