#ifndef SPEEX_ENCODE_H_
#define SPEEX_ENCODE_H_

#include "speex_encode.h"
#include "CaptureSound.h"

typedef struct dsound_capture_union{

	int sppex_handle;

	CCaptureSound *m_cap;

	int ns_handle;

}dsound_capture_t;

/*
 * success return 0 ,else non-zero
 * */
int dsound_capture_init(dsound_capture_t *dsound_capture ,uint32_t seq_number_in_ms);

//int dsound_capture_process(dsound_capture_t *dsound_capture ,char ** speex_audio_data ,int * speex_audio_size);
int dsound_capture_process(dsound_capture_t *dsound_capture ,char * speex_audio_data ,int  speex_audio_size);

void dsound_capture_destroy(dsound_capture_t *dsound_capture);

#endif

