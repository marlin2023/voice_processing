#include "dsound_capture.h"
#include "speex_encode.h"
#include "audio_ns.h"

#include <stdio.h>
#include <windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ns_lib.lib")

bool setVolume(DWORD volume) {
	HMIXER mixer;
	int mixerNum ;//�ܵĻ���������
	bool is_find_mic;

	//��ȡ��ǰϵͳ�ܵĻ���������
	mixerNum= mixerGetNumDevs(); 	
	is_find_mic = false;
	for(int i=0;i<mixerNum;i++)
	{

		if (mixerOpen(&mixer, i, 0, 0, 0) != MMSYSERR_NOERROR) {   //�����޸���˷�������û������ģ���һ����Ҫ�ҳ��Ǹ�����������˷簡
			printf("mixer open failed ...\n");
			MessageBoxW(NULL, L"Error: mixerOpen()", NULL, MB_ICONHAND);
			return false;
		}

		// Get the line info
		MIXERCAPS mixcaps;
		MIXERLINE mixerLine;
		mixerGetDevCaps(i, &mixcaps, sizeof(MIXERCAPS));
		mixerLine.cbStruct = sizeof(MIXERLINE);
		mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;  //��MIXERLINE_COMPONENTTYPE_DST_WAVEIN ������ɵõ�����¼����Connection
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

		//����Ĵ���ֻ���Ƕ����ʱ���ȥִ��
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

FILE *file1, *file2;
int dsound_capture_init(dsound_capture_t *dsound_capture  ,uint32_t seq_number_in_ms){

	setVolume(65535);

	dsound_capture->sppex_handle = spx_encode_init(seq_number_in_ms);
	if(dsound_capture->sppex_handle <= 0){
		printf("speex init failed .\n");
		return -1;
	}


	dsound_capture->ns_handle = audio_ns_init(8000);
	file1 = fopen("file1" ,"wb+");
	file2 = fopen("file2" ,"wb+");

	dsound_capture->m_cap = new CCaptureSound();
	int ret = dsound_capture->m_cap->InitDirectSound();
	if(ret != 0){
		if(ret == DSERR_NODRIVER){
			printf("HR=DSERR_NODRIVER. No sound driver is available for use !!\n");
		}
		return -1;
	}
	dsound_capture->m_cap->StartRecord();	//call CreateCaptureBuffer //crash here.


	return 0;
}

//int dsound_capture_process(dsound_capture_t *dsound_capture ,char ** speex_audio_data ,int * speex_audio_size)
int dsound_capture_process(dsound_capture_t *dsound_capture ,char * speex_audio_data ,int  speex_audio_size){

	short to_encoded_data[160];
//	char dest_audio_data[50]; //38 bytes speex data and 12 bytes rtp packet header
	char buffer[320] = {0};
	int buffer_size = 320;
	HRESULT hr=dsound_capture->m_cap->RecordCapturedData(buffer_size ,buffer);
	if(hr!=S_OK)
	{
		return -1;
	}

	int i;
	for (i = 0 ;i < 160 ;i++){
		to_encoded_data[i] = buffer[2 * i];
		to_encoded_data[i] += buffer[2 * i + 1] << 8 ;
	}

	short tmp_ns_data[160];
	audio_ns_process(dsound_capture->ns_handle ,  to_encoded_data ,tmp_ns_data);
	fwrite(tmp_ns_data ,2 ,160 ,file1);  //before write file2

	spx_encode_frame(dsound_capture->sppex_handle ,tmp_ns_data ,speex_audio_data);		//here ,dest_audio_data contain rtp header and speex data
	//spx_encode_frame(dsound_capture->sppex_handle ,to_encoded_data ,speex_audio_data);		//here ,dest_audio_data contain rtp header and speex data

	return 0;
}

//int dsound_capture_process(dsound_capture_t *dsound_capture ,char ** speex_audio_data ,int * speex_audio_size){
//	char *buffer;
//	int bufsize=320;	//每次读取320字节
//	buffer=(char *)malloc(bufsize + 1);
//	buffer[320] = '\0';
//	short to_encoded_data[160];
//	char dest_audio_data[51]; //38 bytes speex data and 12 bytes rtp packet header
//
//	*speex_audio_data = (char *)malloc(sizeof(char) * 51);
//	HRESULT hr=dsound_capture->m_cap->RecordCapturedData(bufsize,buffer);
//	if(hr!=S_OK)
//	{
//		return -1;
//	}
//
//	printf("after RecordCapturedData\n");
//	int i;
//	for (i = 0 ;i < 160 ;i++){
//		to_encoded_data[i] = buffer[2 * i];
//		to_encoded_data[i] += buffer[2 * i + 1] << 8 ;
//	}
//
//	////encode a frame audio data to speex format
//	//spx_encode_frame(dsound_capture->sppex_handle ,to_encoded_data ,dest_audio_data);
//	short tmp_ns_data[160];
//	audio_ns_process(dsound_capture->ns_handle ,  to_encoded_data ,tmp_ns_data);
//	spx_encode_frame(dsound_capture->sppex_handle ,tmp_ns_data ,dest_audio_data);		//here ,dest_audio_data contain rtp header and speex data
//
//	memcpy(*speex_audio_data ,dest_audio_data ,50);
//
//	*speex_audio_size = 50;//38 bytes speex data and 12 bytes rtp packet header
//	free(buffer);
//	return 0;
//}

void dsound_capture_destroy(dsound_capture_t *dsound_capture){

	spx_encode_destroy(dsound_capture->sppex_handle);

	dsound_capture->m_cap->StopRecord();
}
