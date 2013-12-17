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
#define RTP_HEADER_SIZE                12

#include <stdint.h>

#pragma comment (lib ,"libspeex.lib")
typedef struct rtp_header
{
#ifdef  ORTP_BIGENDIAN
        uint16_t version:2;
        uint16_t padbit:1;
        uint16_t extbit:1;
        uint16_t cc:4;
        uint16_t markbit:1;
        uint16_t paytype:7;
#else
        uint16_t cc:4;
        uint16_t extbit:1;
        uint16_t padbit:1;
        uint16_t version:2;
        uint16_t paytype:7;
        uint16_t markbit:1;
#endif
        uint16_t seq_number;
        uint32_t timestamp;
        uint32_t ssrc;
        uint32_t csrc[16];
} rtp_header_t;

#include <speex/speex.h>
typedef struct SPEEX_ENCODE_UNION{

        /*Holds the state of the encoder*/
        void *state;

    /*Holds bits so they can be read and written to by the Speex routines*/
    SpeexBits bits;

    rtp_header_t rtp_header;
}speex_encode_union_t;

int  spx_encode_init();

int spx_encode_frame(int handle ,short * const pcm_data ,char *speex_data);

int spx_destroy(int handle );

#endif /* SPEEX_ENCODE_H_ */