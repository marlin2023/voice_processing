// dsound_capture_demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dsound_capture.h"
#pragma comment (lib ,"dsound_captured.lib")
#pragma comment (lib ,"dsound.lib")
#pragma comment (lib ,"dxguid.lib")
#pragma comment (lib ,"ns_lib.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	dsound_capture_t *dsound_capture_u = (dsound_capture_t *)malloc(sizeof(dsound_capture_t));
	if(dsound_capture_u == NULL){
		printf("malloc failed ...\n");
		exit(1);
	}

	dsound_capture_init(dsound_capture_u);

	char *encoded_audio_data = NULL;
	int encoded_audio_size;

	/*FILE *fp_speex = NULL;
	fp_speex = fopen("speex_send.spx" ,"wb+");
	if(fp_speex_send == NULL){
	printf("open sdcard/fp_speex_send.spx file failed");
	}*/

	printf("before while ...\n");
	int ii = 0;
	while(1){
		ii ++;
		dsound_capture_process(dsound_capture_u ,&encoded_audio_data ,&encoded_audio_size);
		free(encoded_audio_data);
		encoded_audio_data = NULL;

		if(ii == 10000){
			break;
		}
	}

	dsound_capture_destroy(dsound_capture_u);

	while(1);
	return 0;
}

