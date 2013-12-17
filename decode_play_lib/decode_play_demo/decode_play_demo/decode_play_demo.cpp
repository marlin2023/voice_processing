// decode_play_demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dsound_play.h"
#pragma comment (lib ,"decode_play.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	FILE * fp;

//	if((fp=fopen("mine.pcm","rb"))==NULL)//打开你的PCM 文件mine.spx
	if((fp=fopen("mine.spx","rb"))==NULL)//打开你的PCM 文件mine.spx
	{
		printf("cannot open this file\n");
	}
	char pcm_data[320];

	dsound_union_t * dsound_union_handle = (dsound_union_t *)malloc(sizeof(dsound_union_t));
	if(dsound_union_handle == NULL){
		printf("malloc failed .\n");
		exit(1);
	}

	init_audio(dsound_union_handle, 1, 16, 8000, 0);
	while(1){
		//fread((BYTE*)pcm_data,320,1,fp);
		if(feof(fp)){
			printf("over ....\n");
			destory_audio(dsound_union_handle);
			while(1);
			break;
		}
		/*fread((BYTE*)pcm_data,38,1,fp);
		play_audio(dsound_union_handle ,pcm_data, 38);*/
		fread((BYTE*)pcm_data,50,1,fp);
		play_audio(dsound_union_handle ,pcm_data, 50);
	}
	return 0;
}

