/*********************************************************************************
  *Copyright(C),2013-2014,Yinyuetai.com
  *FileName:  // dsound_play.h
  *Author:  //chris
  *Version:  //0.0.1
  *Date:  //2013/11/12
  *Description:  //define the interface of dsound to play pcm data
**********************************************************************************/
#ifndef DSOUND_PLAY_H_
#define DSOUND_PLAY_H_
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <stdlib.h>
#include <speex/speex.h>

#include "jitter_buffer.h"

#pragma comment (lib ,"libspeex.lib")
#pragma   comment(lib,"winmm.lib")
#pragma   comment(lib,"dsound.lib") 
#pragma   comment(lib,"dxguid.lib")

#define MAX_AUDIO_BUF			 2 //16	 //4
#define BUFFERNOTIFYSIZE		320	 //20ms speex data size--320 bytes ,8000 sample . mono .16s_le

#define N_CHANNAL				1	//channel number
#define BIT_PER_SAMPLE			16	//bits_per_sample
#define PCM_SAMPLE				8000	//sample


typedef  long	 DSOUND_HANDLE;	//dsound object handle
//typedef	char  uint8_t;


//typedef struct {
//
////	LPDIRECTSOUND8 g_pDsd;	//1.dsound handle
////
////	LPDIRECTSOUNDBUFFER lpbuffer;		//	2.primary buffer
////	LPDIRECTSOUNDBUFFER8 g_pDSBuffer8; //3.// the IDirectSoundBuffer8  interface of secondary buffer
////
////	LPDIRECTSOUNDNOTIFY8 g_pDSNotify;
////	DSBPOSITIONNOTIFY g_aPosNotify[MAX_AUDIO_BUF];	//the array of setting notify
////	HANDLE g_event[MAX_AUDIO_BUF];    //播放通知
////
////	DWORD g_dwNextWriteOffset ;
//
//	int speex_handle;
//	jitter_buffer_t  jitter_buffer;
//}dsound_union_t;
#include "SDL_audio.h"
#define VOICE_FRAME_SIZE  160 //480
typedef struct {

	short RawPCMData[VOICE_FRAME_SIZE];
	int StopPlayVoice;
	SDL_AudioSpec fmt;

	int speex_handle;
	jitter_buffer_t  jitter_buffer;
}dsound_union_t;



//success ,init_audio return TRUE
bool init_audio(dsound_union_t *dsound_handle, int channels, int bits_per_sample, int sample_rate, int format);

//success ,return 0;
int put_audio_data(dsound_union_t *dsound_handle ,char* encoded_audio_data, int size);

//get audio data ,and play
//
void get_audio_play(dsound_union_t *dsound_handle );

void play_audio(dsound_union_t *dsound_handle ,char* data, int size);


void destory_audio(dsound_union_t *dsound_handle);

#endif
