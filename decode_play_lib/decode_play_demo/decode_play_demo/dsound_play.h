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
#include <windows.h>//���ͷ�ļ�Ҫ������ǰ�棬���Ż���ʾ��Щ�����Ҳ���
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <stdlib.h>

#pragma   comment(lib,"winmm.lib")
#pragma   comment(lib,"dsound.lib") 
#pragma   comment(lib,"dxguid.lib")

#define MAX_AUDIO_BUF   4
#define BUFFERNOTIFYSIZE 3200	 //20ms speex data size ,8000 sample . mono .16s_le

typedef int	 DSOUND_HANDLE;	//dsound object handle
typedef	char  uint8_t;

typedef struct {

	LPDIRECTSOUND8 g_pDsd;	//1.dsound handle

	LPDIRECTSOUNDBUFFER lpbuffer;		//	2.primary buffer 
	LPDIRECTSOUNDBUFFER8 g_pDSBuffer8; //3.// the IDirectSoundBuffer8  interface of secondary buffer //���Ż�����ڶ�������

	LPDIRECTSOUNDNOTIFY8 g_pDSNotify;
	DSBPOSITIONNOTIFY g_aPosNotify[MAX_AUDIO_BUF];	//the array of setting notify
	HANDLE g_event[MAX_AUDIO_BUF];    //����֪ͨ 
	
	DWORD g_dwNextWriteOffset ;




}dsound_union_t;

bool init_audio(dsound_union_t *dsound_handle, int channels, int bits_per_sample, int sample_rate, int format);


void play_audio(dsound_union_t * dsound_union ,uint8_t* data, int size);

void destory_audio(dsound_union_t *dsound_handle);

#endif
