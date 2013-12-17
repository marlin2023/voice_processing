#include "dsound_capture.h"
#include "speex_encode.h"

#include <stdio.h>
#include <windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")

bool setVolume(DWORD volume) {
	HMIXER mixer;
	int mixerNum ;//总的混音器数量
	bool is_find_mic;

	//获取当前系统总的混音器数量
	mixerNum= mixerGetNumDevs(); 	
	is_find_mic = false;
	for(int i=0;i<mixerNum;i++)
	{

		if (mixerOpen(&mixer, i, 0, 0, 0) != MMSYSERR_NOERROR) {   //这里修改麦克风音量是没有问题的，第一个你要找出那个混音器是麦克风啊
			printf("mixer open failed ...\n");
			MessageBoxW(NULL, L"Error: mixerOpen()", NULL, MB_ICONHAND);
			return false;
		}

		// Get the line info
		MIXERCAPS mixcaps;
		MIXERLINE mixerLine;
		mixerGetDevCaps(i, &mixcaps, sizeof(MIXERCAPS));
		mixerLine.cbStruct = sizeof(MIXERLINE);
		mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;  //用MIXERLINE_COMPONENTTYPE_DST_WAVEIN 作参数可得到所有录音的Connection
		mixerLine.dwSource = 0;
		mixerLine.dwDestination = 0;

		if (mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(mixer), &mixerLine, MIXER_GETLINEINFOF_SOURCE)
			!= MMSYSERR_NOERROR) {
				MessageBoxW(NULL, L"Error: mixerGetLineInfo()", NULL, MB_ICONHAND);
				return false;
		}

		if(mixerLine.dwComponentType !=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE ){
			printf("i = %d ,this device is not microphone \n" ,i);
			mixerClose(mixer);
			continue;
		}

		is_find_mic = true;
		printf("mixerLine.dwComponentType = %xH ,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE = %xH \n" ,mixerLine.dwComponentType ,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE);

		//下面的代码只有是耳麦的时候才去执行
		// Get control for mixerline
		MIXERCONTROL mixerCtrl;
		MIXERLINECONTROLS mixerLineCtrl;

		mixerLineCtrl.cbStruct = sizeof(MIXERLINECONTROLS);
		mixerLineCtrl.dwLineID = mixerLine.dwLineID;
		mixerLineCtrl.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		mixerLineCtrl.cControls = 1;
		mixerLineCtrl.pamxctrl = &mixerCtrl;
		mixerLineCtrl.cbmxctrl = sizeof(MIXERCONTROL);
		mixerLineCtrl.cControls = 5;

		if (mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(mixer), &mixerLineCtrl, MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR) {
				MessageBoxW(NULL, L"Error: mixerGetLineControls()", NULL, MB_ICONHAND);
				return false;
		}

		// Volume..
		MIXERCONTROLDETAILS mixerCtrlDetails;
		MIXERCONTROLDETAILS_UNSIGNED mixerCtrlDetailsUnsigned;

		mixerCtrlDetailsUnsigned.dwValue = volume;		//volume size 
		mixerCtrlDetails.dwControlID = mixerCtrl.dwControlID;
		mixerCtrlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mixerCtrlDetails.cMultipleItems = 0;
		mixerCtrlDetails.paDetails = &mixerCtrlDetailsUnsigned;
		mixerCtrlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		mixerCtrlDetails.cChannels = 1;

		if (mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(mixer), &mixerCtrlDetails, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE)
			!= MMSYSERR_NOERROR) {
				MessageBoxW(NULL, L"Error: mixerSetControlDetails()", NULL, MB_ICONHAND);
				return false;
		}

		mixerClose(mixer);
	}//for loop

	return is_find_mic;
}
int dsound_capture_init(dsound_capture_t *dsound_capture){

	setVolume(65535);

	dsound_capture->sppex_handle = spx_encode_init();
	if(dsound_capture->sppex_handle <= 0){
		printf("speex init failed .\n");
		exit(1);
	}

	dsound_capture->m_cap = new CCaptureSound();
	dsound_capture->m_cap->InitDirectSound();
	dsound_capture->m_cap->StartRecord();


	return 1;
}

int dsound_capture_process(dsound_capture_t *dsound_capture ,char ** speex_audio_data ,int * speex_audio_size){
	char *buffer;
	int bufsize=320;	//每次读取320字节
	buffer=(char *)malloc(bufsize + 1);
	buffer[320] = '\0';
	short to_encoded_data[160];
	char dest_audio_data[39];

	*speex_audio_data = (char *)malloc(sizeof(char)*(38 + 1));
	int i ;
	for(i = 0;i < 38 ; i ++){
		*(*speex_audio_data + i) = 1;
		printf("%p \n" ,*speex_audio_data + i);
	}
	if(*speex_audio_data == NULL){
		printf("malloc failed .\n");
	}
	printf("*speex_audio_data %p \n" ,*speex_audio_data);
	HRESULT hr=dsound_capture->m_cap->RecordCapturedData(bufsize,buffer);
	if(hr!=S_OK)
	{
		return -1;
	}

	//int i;
	for (i = 0 ;i < 160 ;i++){
		to_encoded_data[i] = buffer[2 * i];
		to_encoded_data[i] += buffer[2 * i + 1] << 8 ;
	}
	//encode a frame audio data to speex format
	spx_encode_frame(dsound_capture->sppex_handle ,to_encoded_data ,dest_audio_data);
	printf("*speex_audio_data %p ,+38 = %p \n" ,*speex_audio_data  ,*speex_audio_data + 38);
	for(i = 0;i < 38 ; i ++){
		*(*speex_audio_data + i) = 1;
		printf("%p \n" ,*speex_audio_data + i);
	}
	memcpy(*speex_audio_data ,dest_audio_data ,38);

	(*speex_audio_data)[38] = '\0';
	*speex_audio_size = 38;
	//free(buffer);  //????
	return 0;
}