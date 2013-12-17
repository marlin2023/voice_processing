/*
 * mix_pcm.h
 *
 *  Created on: Nov 27, 2013
 *      Author: chris
 */

#ifndef MIX_PCM_H_
#define MIX_PCM_H_

// PCM16 range: [-32767:32768]
#define MAX_LINEAR_SAMPLE 32737
#include <stdint.h>
#include "yyt_type.h"

typedef struct channel_handle_t		//every channel have one
{
	uint32_t	ts_rtp_base;										//time stamp in rtp header
	long long 	ts_system_base;									//time stamp in local system

	YYT_HANDLE_T speex_decode_handle;												//speex_decode_handle
	YYT_HANDLE_T speex_encode_handle;												//speex_encode_handle

}channel_handle_t;



#endif /* MIX_PCM_H_ */
