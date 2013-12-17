/*********************************************************************************
*Copyright(C),2013-2014,Yinyuetai.com
*FileName:  // dsound_play.cpp
*Author:  //chris
*Version:  //0.0.1
*Date:  //2013/11/12
*Description:  //define the interface of dsound to play pcm data
**********************************************************************************/
#include "SDL.h"
#include "SDL_audio.h"
#include "dsound_play.h"
#include "speex_decode.h"

FILE *fp_play_test = NULL;
#pragma comment (lib ,"SDL2.lib")
#pragma   comment(lib,"Winmm.lib")
#pragma   comment(lib,"msimg32.lib")
#pragma   comment(lib,"version.lib")
#pragma   comment(lib,"imm32.lib")
#if 0
//IDirectSoundBuffer *tempBuffer;
//bool init_audio(dsound_union_t *dsound_handle, int channels, int bits_per_sample, int sample_rate, int format){
//
//	fp_play_test = fopen("client_play.spx" ,"wb+"); //
//
//	/*1.Initialize DirectSound */
//	SetConsoleTitle("yyt_dsound_player");//set windows title
//	HRESULT hr;
//	if(FAILED( hr = DirectSoundCreate8(NULL,&dsound_handle->g_pDsd,NULL))){
//		printf("Cannot create a DirectSound device.\n");
//		return FALSE;
//	}
//
//	/*2.Set cooperative level */
//	if(FAILED(hr = dsound_handle->g_pDsd->SetCooperativeLevel(FindWindow(NULL,"yyt_dsound_player"), DSSCL_EXCLUSIVE)))  //DSSCL_NORMAL DSSCL_EXCLUSIVE  DSSCL_PRIORITY//GetDesktopWindow() FindWindow(NULL,"player")
//	{
//		switch(hr)
//		{
//		case DSERR_ALLOCATED:
//			printf("DSERR_ALLOCATED\n");
//			break;
//		case DSERR_INVALIDPARAM:
//			printf("DSERR_INVALIDPARAM\n");
//			break;
//		case DSERR_UNINITIALIZED:
//			printf("DSERR_UNINITIALIZED\n");
//			break;
//		case DSERR_UNSUPPORTED:
//			printf("DSERR_UNSUPPORTED\n");
//			break;
//		default:
//			break;
//		}
//
//		return FALSE;
//	}
//
//	/*3.set DirectSound buffer descriptor	*/
//	// setup the WAVEFORMATEX structure
//	WAVEFORMATEX wave_format;
//	DSBUFFERDESC dsbd;
//
//	ZeroMemory(&wave_format, sizeof(WAVEFORMATEX));
//	wave_format.wFormatTag      = WAVE_FORMAT_PCM;
//	wave_format.nChannels       = channels;//1;        // mono
//	wave_format.nSamplesPerSec  = sample_rate;//8000;
//	wave_format.wBitsPerSample  = bits_per_sample;//16;
//	wave_format.nBlockAlign     = (wave_format.wBitsPerSample / 8) * wave_format.nChannels;
//	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
//	wave_format.cbSize = 0;
//
//	// Set up DSBUFFERDESC structure.
//	printf("sizeof(dsbd) =%d\n",sizeof(dsbd));
//	/*memset(&dsbd,0,sizeof(dsbd));
//	dsbd.dwSize = sizeof(dsbd);*/
//	memset(&dsbd,0,sizeof(DSBUFFERDESC));
//	dsbd.dwSize = sizeof(DSBUFFERDESC);
//	dsbd.dwFlags =
//		 DSBCAPS_CTRLVOLUME		//The buffer has volume control capability.
//		|DSBCAPS_CTRLFREQUENCY
//		|DSBCAPS_CTRLPAN
//		| DSBCAPS_GLOBALFOCUS		//The buffer is a global sound buffer. With this flag set, an application using DirectSound can continue to play its buffers if the user switches focus to another application,
//		| DSBCAPS_CTRLPOSITIONNOTIFY		//The buffer has position notification capability.
//		| DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME ;
//
//	dsbd.lpwfxFormat = &wave_format;
//	dsbd.dwBufferBytes = MAX_AUDIO_BUF * BUFFERNOTIFYSIZE ;
//
//
//	/*4.create LPDIRECTSOUNDBUFFER sound buffer	*/
//	HRESULT hr1;
//	if(DS_OK != (hr1=(dsound_handle->g_pDsd->CreateSoundBuffer(&dsbd,&dsound_handle->lpbuffer,NULL))))
//	{
//		switch(hr1)
//		{
//		case DSERR_ALLOCATED:
//			printf("CreateSoundBuffer DSERR_ALLOCATED\n");
//			break;
//		case DSERR_BADFORMAT:
//			printf("CreateSoundBuffer DSERR_BADFORMAT\n");
//			break;
//		case DSERR_CONTROLUNAVAIL:
//			printf("CreateSoundBuffer DSERR_CONTROLUNAVAIL\n");
//			break;
//		case DSERR_INVALIDCALL:
//			printf("CreateSoundBuffer DSERR_INVALIDCALL\n");
//			break;
//		case DSERR_INVALIDPARAM:
//			printf("CreateSoundBuffer DSERR_INVALIDPARAM\n");
//			break;
//		case DSERR_NOAGGREGATION:
//			printf("CreateSoundBuffer DSERR_NOAGGREGATION\n");
//			break;
//		case DSERR_OUTOFMEMORY:
//			printf("CreateSoundBuffer DSERR_OUTOFMEMORY\n");
//			break;
//		case DSERR_UNINITIALIZED:
//			printf("CreateSoundBuffer DSERR_UNINITIALIZED\n");
//			break;
//		case DSERR_UNSUPPORTED:
//			printf("CreateSoundBuffer DSERR_UNSUPPORTED\n");
//			break;
//		}//end switch
//		return FALSE;
//	}
//
//	////get   secondary buffer  g_pDSBuffer8
//	//http://www.rastertek.com/dx10tut14.html
//	//http://www.cppblog.com/lovedday/archive/2007/07/26/28827.html   DSound使用的一个博客
//	// Create a temporary sound buffer with the specific buffer settings.
//	//IDirectSoundBuffer *tempBuffer;
//	//HRESULT result;
//	//result = dsound_handle->g_pDsd->CreateSoundBuffer(&dsbd, &tempBuffer, NULL);
//	//if(FAILED(result))
//	//{
//	//	printf("createsoundbuffer error ..\n");
//	//	return false;
//	//}
//
//	//// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
//	//result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&dsound_handle->g_pDSBuffer8);
//	//if(FAILED(result))
//	//{
//	//	printf("queryInterface dsbuffer8 error .\n");
//	//	return false;
//
//	//}
//
//	//// Release the temporary buffer.
//	//tempBuffer->Release();
//	//tempBuffer = 0;
//
//	HRESULT result = dsound_handle->lpbuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&dsound_handle->g_pDSBuffer8);
//	if(FAILED(result))
//	{
//		printf("queryInterface dsbuffer8 error .\n");
//		return FALSE;
//
//	}
//	dsound_handle->lpbuffer->Release();   //add
//
//	/* 4、set DirectSound Position Notify */
//	HRESULT hr3;
//	if(FAILED(hr3=(dsound_handle->g_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&dsound_handle->g_pDSNotify))))
//	{
//		switch(hr3)
//		{
//		case E_NOINTERFACE:
//			printf("E_NOINTERFACE hr3\n");
//			break;
//		default:
//			printf("an unknow error hr3,%d\n",hr3);
//			break;
//		}
//		return FALSE ;
//	}
//
//	int i ;
//	for(i =0;i<MAX_AUDIO_BUF;i++)
//	{
//		dsound_handle->g_aPosNotify[i].dwOffset = (i+ 1) *BUFFERNOTIFYSIZE - 1;
//		dsound_handle->g_event[i]=::CreateEvent(NULL,false,false,NULL);	//create event
//		if(dsound_handle->g_event[i] == NULL){
//			printf("create event failed .\n");
//		}
//		dsound_handle->g_aPosNotify[i].hEventNotify=dsound_handle->g_event[i];
//	}
//	dsound_handle->g_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,dsound_handle->g_aPosNotify);	//set notify position
//	dsound_handle->g_pDSNotify->Release();
//
//
//	/*5.start play audio in buffer*/
//	DWORD res=5;
//	int flag=1;
//
//	/*.read to play	*/
//	dsound_handle->g_pDSBuffer8->SetCurrentPosition(0);
//	//dsound_handle->g_pDSBuffer8->SetVolume(-10000); // set the attenuation of the sound DSBVOLUME_MAX
//	if(dsound_handle->g_pDSBuffer8->SetVolume(DSBVOLUME_MAX) != DS_OK){ //DSBVOLUME_MIN DSBVOLUME_MAX -2000
//		printf("SetVolume failed ...\n\n");
//	}
//	dsound_handle->g_pDsd->SetSpeakerConfig(DSSPEAKER_COMBINED(DSSPEAKER_MONO ,DSSPEAKER_GEOMETRY_MAX));   //set speaker_config
//	dsound_handle->g_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);	//here ,already start to play audio ,use LOOPING mark
//	dsound_handle->g_dwNextWriteOffset =0;
//	dsound_handle->speex_handle = spx_decode_init();
//	if(dsound_handle->speex_handle <= 0){
//		printf("speex decode initialize failed .\n");
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
///*This function use to provide pcm */
//int ProcessBuffer(dsound_union_t *dsound_handle,char* pcm_data, int size)
//{
//	//printf("this is processBuffer in ,in the front of ProcessBuffer \n");
//	VOID* pDSLockedBuffer = NULL;
//	VOID* pDSLockedBuffer2 = NULL;
//	DWORD dwDSLockedBufferSize;
//	DWORD dwDSLockedBufferSize2;
//	HRESULT hr;
//
//	hr=dsound_handle->g_pDSBuffer8->Lock(dsound_handle->g_dwNextWriteOffset,size,&pDSLockedBuffer
//		,&dwDSLockedBufferSize, &pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
//	//printf("dwDSLockedBufferSize = %d ,dwDSLockedBufferSize2 =%d \n" ,dwDSLockedBufferSize ,dwDSLockedBufferSize2);
//	if(hr == DSERR_BUFFERLOST)
//	{
//		dsound_handle->g_pDSBuffer8->Restore();
//		dsound_handle->g_pDSBuffer8->Lock(dsound_handle->g_dwNextWriteOffset,size,&pDSLockedBuffer
//			,&dwDSLockedBufferSize,&pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
//	}
//	if(SUCCEEDED(hr))
//	{
//		////fread((BYTE*)pDSLockedBuffer,dwDSLockedBufferSize,1,fp);
//		//if(0==(test=fread((BYTE*)pDSLockedBuffer,dwDSLockedBufferSize,1,fp)))
//		//	return 0;
//		memcpy((BYTE*)pDSLockedBuffer, pcm_data,dwDSLockedBufferSize);
//		//printf("dwDSLockedBufferSize is%d\n",test);
//		dsound_handle->g_dwNextWriteOffset+=dwDSLockedBufferSize;
//		if (NULL != pDSLockedBuffer2)
//		{
//			////fread((BYTE*)pDSLockedBuffer2,dwDSLockedBufferSize2,1,fp);
//			//if(0==(test=fread((BYTE*)pDSLockedBuffer2,dwDSLockedBufferSize2,1,fp)))
//			//	return 0;
//			memcpy((BYTE*)pDSLockedBuffer2, pcm_data + dwDSLockedBufferSize ,dwDSLockedBufferSize2);
//			//printf("dwDSLockedBufferSize2 is%d\n",test);
//			dsound_handle->g_dwNextWriteOffset+=dwDSLockedBufferSize2;
//		}
//		dsound_handle->g_dwNextWriteOffset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
//
//		//printf("this is %d of buffer\n",g_dwNextWriteOffset);
//		hr = dsound_handle->g_pDSBuffer8->Unlock(pDSLockedBuffer,dwDSLockedBufferSize,
//			pDSLockedBuffer2,dwDSLockedBufferSize2);
//	}
//	return 1;
//}
//
//int put_audio_data(dsound_union_t *dsound_handle ,char* encoded_audio_data, int size){
//
//	return jitter_buffer_push(&dsound_handle->jitter_buffer, encoded_audio_data ,size);
//
//}
//
//void get_audio_play(dsound_union_t *dsound_handle ){
//
//	char rtp_data[50];
//	int ret1 = jitter_buffer_pop(&dsound_handle->jitter_buffer ,rtp_data ,50);
//	if( ret1 == -1){
//		return ;
//	}
//	else if(ret1 == NO_JITTER_BUFFER_DATA){
//		memset(rtp_data ,0 ,50 );
//	}
//	fwrite(rtp_data ,1 ,50 ,fp_play_test);
//
//	int res = WaitForMultipleObjects (MAX_AUDIO_BUF, dsound_handle->g_event, FALSE, INFINITE); //播放时等待通告事件被触发，它是WaitForMultipleObjects函数的工作 INFINITE代表只要没收到通知，无限等下去
//	if((res >= WAIT_OBJECT_0 + 0 )&&(res <= WAIT_OBJECT_0+3))// WaitForMultipleObjects函数的返回值就能够获取事件号。获取到事件号后，用这个数字减去 WAIT_OBJECT_0，得到的结果就是触发的事件索引号
//	{
//		if(ret1 != NO_JITTER_BUFFER_DATA){
//			//spx_decode_frame(dsound_handle->speex_handle ,encoded_audio_data ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
//			spx_decode_frame(dsound_handle->speex_handle ,rtp_data + 12 ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
//
//			int ii;
//			for(ii = 0; ii < 160 ; ii ++){	//short to char
//				((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii] = (byte)(((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] & 0xff);
//				((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii + 1] = (byte)((((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] >> 8) & 0xff);
//			}
//			ProcessBuffer(dsound_handle, ((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data ,320);
//
//		}else{
//			char tmp_buffer[320];
//			memset(tmp_buffer ,0 ,320);
//			ProcessBuffer(dsound_handle, tmp_buffer ,320);
//		}
//
//	}//end if
//
//}
//
//
//void play_audio(dsound_union_t *dsound_handle ,char* encoded_audio_data, int size){  //flow in is speex data ,size is 38
//
//	if(size != 50){	//rtp header 12 bytes ,and speex data 38 bytes  //!!!!!!!!!!!!!
//		printf("audio data loose \n");
//		return;
//	}
//
//	int res = WaitForMultipleObjects (MAX_AUDIO_BUF, dsound_handle->g_event, FALSE, INFINITE); //播放时等待通告事件被触发，它是WaitForMultipleObjects函数的工作 INFINITE代表只要没收到通知，无限等下去
//	if((res >= WAIT_OBJECT_0 + 0 )&&(res <= WAIT_OBJECT_0+3))// WaitForMultipleObjects函数的返回值就能够获取事件号。获取到事件号后，用这个数字减去 WAIT_OBJECT_0，得到的结果就是触发的事件索引号
//	{
//		//spx_decode_frame(dsound_handle->speex_handle ,encoded_audio_data ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
//		spx_decode_frame(dsound_handle->speex_handle ,encoded_audio_data + 12 ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
//
//		int ii;
//		for(ii = 0; ii < 160 ; ii ++){	//short to char
//			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii] = (byte)(((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] & 0xff);
//			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii + 1] = (byte)((((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] >> 8) & 0xff);
//		}
//		ProcessBuffer(dsound_handle, ((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data ,320);
//
//	}//end if
//
//}
//
//
//
//void destory_audio(dsound_union_t *dsound_handle)
//{
//
//	dsound_handle->g_pDSBuffer8->Stop();
//
//	if (dsound_handle->g_pDsd)
//	{
//		dsound_handle->g_pDsd->Release();
//		dsound_handle->g_pDsd = NULL;
//	}
//}
#endif
void RecvVoiceData(void *userdata, Uint8 *stream, int len)
{

	dsound_union_t *dsound_handle = (dsound_union_t *)userdata;
	//printf("in cb function \n");
	//acquire pcm data
	char rtp_data[50];
	int ret1 = jitter_buffer_pop(&dsound_handle->jitter_buffer, rtp_data, 50);
	if (ret1 == -1){
		return;
	}
	else if (ret1 == NO_JITTER_BUFFER_DATA){
		memset(rtp_data, 0, 50);
	}
	fwrite(rtp_data, 1, 50, fp_play_test);

	//provide pcm data
	if (ret1 != NO_JITTER_BUFFER_DATA){
		//spx_decode_frame(dsound_handle->speex_handle ,encoded_audio_data ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
		spx_decode_frame(dsound_handle->speex_handle, rtp_data + 12, ((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);

		int ii;
		for (ii = 0; ii < 160; ii++){	//short to char
			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2 * ii] = (byte)(((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] & 0xff);
			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2 * ii + 1] = (byte)((((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] >> 8) & 0xff);
		}
		SDL_memcpy(stream, ((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data, 320);  //480*2
		//ProcessBuffer(dsound_handle, ((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data ,320);

	}
	else{
		char tmp_buffer[320];
		memset(tmp_buffer, 0, 320);
		SDL_memcpy(stream, tmp_buffer, 320);
		//ProcessBuffer(dsound_handle, tmp_buffer ,320);
	}
	//	if(fread(RawPCMData, sizeof(short), 160, FpRawData)==160)
	//	{
	//		SDL_memcpy(stream, RawPCMData, 320);  //480*2
	//	}
	//	else
	//	{
	//		StopPlayVoice=1;
	//	}
}

//success ,init_audio return TRUE
bool init_audio(dsound_union_t *dsound_handle, int channels, int bits_per_sample, int sample_rate, int format){
	fp_play_test = fopen("client_play.spx", "wb+"); //

	dsound_handle->speex_handle = spx_decode_init();
	if (dsound_handle->speex_handle <= 0){
		printf("speex decode initialize failed .\n");
		return FALSE;
	}
	dsound_handle->StopPlayVoice = 0;

	//	//设置16位 单声道 8kHz的音频
	//	dsound_handle->fmt.freq = 8000;
	//	dsound_handle->fmt.format = AUDIO_S16;
	//	dsound_handle->fmt.channels = 1;
	//	dsound_handle->fmt.samples = 160;
	//	dsound_handle->fmt.callback = RecvVoiceData;
	//	//dsound_handle->fmt.userdata = NULL;   //可以直接在内部传值给callback函数
	//	dsound_handle->fmt.userdata = dsound_handle;   //可以直接在内部传值给callback函数

	return TRUE;
}

int put_audio_data(dsound_union_t *dsound_handle, char* encoded_audio_data, int size){

	return jitter_buffer_push(&dsound_handle->jitter_buffer, encoded_audio_data, size);

}





void get_audio_play(dsound_union_t *dsound_handle){
	int ret;
	ret = SDL_Init(SDL_INIT_AUDIO);
	if (ret < 0){
		fprintf(stderr, "ret = %d ,can not SDL_Init:%s\n", ret, SDL_GetError());
	}
	//设置16位 单声道 8kHz的音频
	dsound_handle->fmt.freq = 8000;
	dsound_handle->fmt.format = AUDIO_S16;
	dsound_handle->fmt.channels = 1;
	dsound_handle->fmt.samples = 160;
	dsound_handle->fmt.callback = RecvVoiceData;
	//dsound_handle->fmt.userdata = NULL;   //可以直接在内部传值给callback函数
	dsound_handle->fmt.userdata = dsound_handle;   //可以直接在内部传值给callback函数

	//打开音频设备，并开始回放
	ret = SDL_OpenAudio(&dsound_handle->fmt, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "ret = %d ,can not open audio device:%s\n", ret, SDL_GetError());
		//exit(1);
		return;
	}

	//	FpRawData=fopen(argv[1], "rb");
	//FpRawData=fopen("1.pcm", "rb");
	//回放音频数据
	SDL_PauseAudio(0);
	printf("Using audio driver: %s\ ,dsound_handle->fmt.size = %d n", SDL_GetCurrentAudioDriver(), dsound_handle->fmt.size);
	//	while ((dsound_handle->StopPlayVoice!=1)&&(SDL_GetAudioStatus()==SDL_AUDIO_PLAYING))
	//	{
	//		SDL_Delay(1000);
	//	}
	//	char rtp_data[50];
	//	int ret1 = jitter_buffer_pop(&dsound_handle->jitter_buffer ,rtp_data ,50);
	//	if( ret1 == -1){
	//		return ;
	//	}
	//	else if(ret1 == NO_JITTER_BUFFER_DATA){
	//		memset(rtp_data ,0 ,50 );
	//	}
	//	fwrite(rtp_data ,1 ,50 ,fp_play_test);
	//
	//	//provide pcm data
	//	if(ret1 != NO_JITTER_BUFFER_DATA){
	//		//spx_decode_frame(dsound_handle->speex_handle ,encoded_audio_data ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
	//		spx_decode_frame(dsound_handle->speex_handle ,rtp_data + 12 ,((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data);
	//
	//		int ii;
	//		for(ii = 0; ii < 160 ; ii ++){	//short to char
	//			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii] = (byte)(((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] & 0xff);
	//			((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data[2*ii + 1] = (byte)((((speex_decode_union_t *)(dsound_handle->speex_handle))->speex_data[ii] >> 8) & 0xff);
	//		}
	//		ProcessBuffer(dsound_handle, ((speex_decode_union_t *)(dsound_handle->speex_handle))->pcm_data ,320);
	//
	//	}else{
	//		char tmp_buffer[320];
	//		memset(tmp_buffer ,0 ,320);
	//		ProcessBuffer(dsound_handle, tmp_buffer ,320);
	//	}
}


void destory_audio(dsound_union_t *dsound_handle){

}
