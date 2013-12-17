/*
 * mix_pcm.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: chris
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "AmMultiPartyMixer.h"

FILE *ptr_input1;
FILE *ptr_input2;

FILE *ptr_output1;
FILE *ptr_output2;

int main(int argc ,char *argv[]){

	ptr_input1 = fopen("capture.spx" ,"rb+");
	ptr_input2 = fopen("capture2.spx" ,"rb+");

	ptr_output1 = fopen("output1.spx" ,"wb+");

	AmMultiPartyMixer mAmMultiParyMixer(0);	//set system_ts_base 0.

	unsigned char spx_buffer[50];
	unsigned char spx_buffer1[50];
	unsigned char output_buffer[50];
	long long sys_ts = 0;
	sys_ts = 40;
	while(1){

		if(feof(ptr_input1)) {
			printf("file 1.spx is end ...\n");
			fclose(ptr_output1);
			break;
		}

		fread(spx_buffer ,1 ,50 ,ptr_input1);
		mAmMultiParyMixer.PutChannelPacket(1 ,0 ,spx_buffer ,50);

//		fread(spx_buffer ,1 ,50 ,ptr_input2);
//		mAmMultiParyMixer.PutChannelPacket(2 ,0 ,spx_buffer ,50);

//		sys_ts += 20;

//		mAmMultiParyMixer.GetChannelPacket(1,sys_ts ,output_buffer ,50);
//mAmMultiParyMixer.GetChannelPacket(0,sys_ts ,output_buffer ,50);
		mAmMultiParyMixer.GetChannelPacket(0,sys_ts ,output_buffer ,50);
		sys_ts += 20;
		fwrite(output_buffer ,1 ,50 ,ptr_output1);
	}

	while(1);

}
//#include "mix_pcm.h"
//#include "speex_decode.h"
//#include "speex_encode.h"
//#include "yyt_error.h"
//
//
//FILE *test_pcm = NULL;
//FILE *test_spx = NULL;
//
//FILE *pcm_channel_1 = NULL;
//FILE *pcm_channel_2 = NULL;
//
//FILE *mixed_channel_1 = NULL;
//FILE *mixed_channel_2 = NULL;
//
//FILE *spx_channel_1 = NULL;
//FILE *spx_channel_2 = NULL;
//
///*
//* if channel_id is new ,return true
//*/
//static bool is_new_channel( mix_process_t *mix_process_handle ,int channel_id){
//
//	//traverse and find the channel_id
//	int i;
//	for(i = 0; i < mix_process_handle->chnnel_num ;i ++){
//		if(mix_process_handle->one_channel_speech[i].channel_id == channel_id){
//			mix_process_handle->speech_index = i ;
//			return false;
//		}
//	}
//
//	return true;
//}
//
//
//
///*
//*interface 1:initialize function
//*sucess return 0;
//**/
//int mix_init(mix_process_t *mix_process_handle ,void  (*mixed_data_snd)(char *mixer_data ,int size ,int channel_id ,void *userdata) ,void *userdata){
//
//	mix_process_handle->chnnel_num = 0;
//	mix_process_handle->mixed_data_send = mixed_data_snd;	 //register the call back function
//	mix_process_handle->priv_data = userdata;
//	mix_process_handle->speech_index = -1;					 //have no speech data
//	mix_process_handle->max_num_packet = 0;
//	mix_process_handle->scaling_factor = 16;
//
//	//initialize the one_channel_speech_t
//	int i = 0;
//	for(i = 0; i < 32; i ++){
//		ringq_init(&mix_process_handle->one_channel_speech[i].ringq_data);
//		mix_process_handle->one_channel_speech[i].channel_id = 0;
//		mix_process_handle->one_channel_speech[i].have_data_mark = false;
//
//	}
//
//	//init the speex_encode_handle
//	mix_process_handle->speex_encode_handle = spx_encode_init();
//	if(mix_process_handle->speex_encode_handle <= 0){
//		printf("speex_encode_handle initialize failed .\n");
//		return -1;
//	}
//
//	test_pcm = fopen("/tmp/test.pcm" ,"wb+");
//	test_spx = fopen("/tmp/test.spx" ,"wb+");
//	if(test_pcm == NULL || test_spx == NULL){
//		printf("...................,errno vlaue :%d ,it means:%s\n" ,errno ,strerror(errno));
//		return -1;
//	}
//	pcm_channel_1 = fopen("/tmp/pcm_channel_1.pcm" ,"wb+");
//	pcm_channel_2 = fopen("/tmp/pcm_channel_2.pcm" ,"wb+");
//
//	mixed_channel_1 = fopen("/tmp/mixed_channel_1.pcm" ,"wb+");
//	mixed_channel_2 = fopen("/tmp/mixed_channel_2.pcm" ,"wb+");
//
//	spx_channel_1 = fopen("/tmp/spx_channel_1.pcm" ,"wb+");
//	spx_channel_2 = fopen("/tmp/spx_channel_2.pcm" ,"wb+");;
//	return 0;
//}
//
//
///*
//*interface 2:receive the server send data
//*
//*success return 0
//**/
//int  put_unmix_data( mix_process_t *mix_process_handle ,char *unmix_data ,int size ,int channel_id){
//	int ret;
//	//1.judge the channel_id is exsit or not
//	if(is_new_channel(mix_process_handle ,channel_id) == true){
//		//mean this a new channel
//		mix_process_handle->chnnel_num ++;
//		mix_process_handle->speech_index = mix_process_handle->chnnel_num - 1;
//		printf("is a new channel ,mix_process_handle->speech_index = %d \n" ,mix_process_handle->speech_index);
//
//		/*	set one_channel_speech struct	*/
//		mix_process_handle->one_channel_speech[mix_process_handle->speech_index].channel_id = channel_id;  //set channel_id
//		mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speech_index = mix_process_handle->speech_index;
//		//init the speex_decode_handle
//		mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_decode_handle = spx_decode_init();
//		if(mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_decode_handle <= 0){
//			printf("speex_decode_handle initialize failed .\n");
//			return SPEEX_DECODE_INIT_FAIL;
//		}
//
//		//init the speex_encode_handle
//		mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_encode_handle = spx_encode_init();
//		if(mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_encode_handle <= 0){
//			printf("speex_encode_handle initialize failed .\n");
//			return SPEEX_ENCODE_INIT_FAIL;
//
//		}
//	}
//	//2.decode encoded speech data int spx_decode_frame(int handle ,char *src_speex_data ,short *target_pcm_data_buffer)
//	short pcm_data_buf[FRAME_SIZE] = {0};
//	ret = spx_decode_frame(mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_decode_handle  ,unmix_data ,pcm_data_buf);  //�����յ��������û������ġ�û�н�������unmix_data .
//	//**********set rtp header seq_number
//	//mix_process_handle->one_channel_speech[mix_process_handle->speech_index].dec_seq_number
//	mix_process_handle->one_channel_speech[mix_process_handle->speech_index].dec_seq_number = ((speex_decode_union_t *)mix_process_handle->one_channel_speech[mix_process_handle->speech_index].speex_decode_handle)->rtp_header.seq_number;
//
//
//	if(ret < 0){return SPX_DECODE_FAIL;}
//	//3.store pcm data
//	ret = ringq_push(&mix_process_handle->one_channel_speech[mix_process_handle->speech_index].ringq_data ,(char *)pcm_data_buf ,320
//			,mix_process_handle->one_channel_speech[mix_process_handle->speech_index].dec_seq_number);	// mix_process_handle->speech_index ;320 is 20ms pcm data
//	if(ret == -1) {return RINGQ_PUSH_FAIL;}
////	fwrite(unmix_data ,1 ,50 ,test_spx);
////	fwrite(pcm_data_buf ,2 ,160 ,test_pcm);
//	if(mix_process_handle->speech_index == 0){
//		fwrite(pcm_data_buf ,2 ,160 ,pcm_channel_1);
//	}else if (mix_process_handle->speech_index == 1){
//		fwrite(pcm_data_buf ,2 ,160 ,pcm_channel_2);
//	}
//
//	mix_process_handle->one_channel_speech[mix_process_handle->speech_index].have_data_mark = true;
//	if(mix_process_handle->one_channel_speech[mix_process_handle->speech_index].ringq_data.num > mix_process_handle->max_num_packet){
//		mix_process_handle->max_num_packet = mix_process_handle->one_channel_speech[mix_process_handle->speech_index].ringq_data.num;
//	}
//
//
//	return 0;
//}
//
//
//// int   dest[size/2]
//// int   src1[size/2]
//// short src2[size/2]
////
//static void mix_add(int* dest,int* src1,short* src2,unsigned int size)
//{
//	int* end_dest = dest + size;
//
//	while(dest != end_dest)
//		*(dest++) = *(src1++) + int(*(src2++));
//}
//
//static void mix_sub(int* dest,int* src1,short* src2,unsigned int size)
//{
//	int* end_dest = dest + size;
//
//	while(dest != end_dest)
//		*(dest++) = *(src1++) - int(*(src2++));
//}
//
//static void scale(mix_process_t *mix_process_handle ,short* buffer,int* tmp_buf,unsigned int size)
//{
//	short* end_dest = buffer + size;
//
//	if(mix_process_handle->scaling_factor<64)
//		mix_process_handle->scaling_factor++;
//
//	while(buffer != end_dest){
//
//		int s = (*tmp_buf * mix_process_handle->scaling_factor) >> 6;
//		if(abs(s) > MAX_LINEAR_SAMPLE){
//			mix_process_handle->scaling_factor = abs( (MAX_LINEAR_SAMPLE<<6) / (*tmp_buf) );
//			if(s < 0)
//				s = -MAX_LINEAR_SAMPLE;
//			else
//				s = MAX_LINEAR_SAMPLE;
//		}
//		*(buffer++) = short(s);
//		tmp_buf++;
//	}
//}
//
///*
//*interface 3: mix all channel data and send mixed data out
//* after mix_data function ,must to do some work to alter the status
//*
//**/
//void mix_data(mix_process_t *mix_process_handle){   //cb function use to send mixed data
//	//1.mix ,encode ,send encoded data
//	int mixed_data_all[160];
//	int mixed_data_others[160];
//	int src1[160] = {0};
//	short src2[160];
//
//	short target_pcm[160];				//store the scale pcm data
//	char speex_data[50];				//store the encoded speech data ,speex data.
//	int i ,m ,n;
//	for(i = 0; i < mix_process_handle->max_num_packet; i ++){
//
//		memset(src1 ,0 ,sizeof(src1));
//		memset(mixed_data_all ,0 ,sizeof(mixed_data_all));
//		if(1 == mix_process_handle->chnnel_num){ /**only one channel speech ,do not have necessary to mix  **/
//			int ret = ringq_poll(&mix_process_handle->one_channel_speech[0].ringq_data, (char *)src2
//					,&mix_process_handle->one_channel_speech[0].dec_seq_number);
//			if(ret == -1) goto fail;
//
//			((speex_encode_union_t *)(mix_process_handle->speex_encode_handle))->rtp_header.seq_number
//										=mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[0].speech_index].dec_seq_number;
//			spx_encode_frame(mix_process_handle->speex_encode_handle ,src2 ,speex_data);//encode pcm data ,out data with 12 bytes rtp packet header
//			//send call cb function
//			mix_process_handle->mixed_data_send(speex_data ,sizeof(speex_data) ,0 ,mix_process_handle->priv_data); //0 mean to other people who dont have not authority
//			//fwrite(speex_data ,1 ,50 ,fp_mix1);
//		}else{ /**mix_process_handle->chnnel_num input ,and mix_process_handle->chnnel_num output **/
//
//			for(m = 0; m < mix_process_handle->chnnel_num; m ++){  //mix_add(int* dest,short* src1,short* src2,unsigned int size)
//				int ret = ringq_poll(&mix_process_handle->one_channel_speech[m].ringq_data, (char *)mix_process_handle->one_channel_speech[m].speech_data
//						,&mix_process_handle->one_channel_speech[m].dec_seq_number);
//				if(ret == -1){mix_process_handle->one_channel_speech[m].have_data_mark = false;  continue; }
//				mix_add(mixed_data_all,src1,mix_process_handle->one_channel_speech[m].speech_data,160);
//				memcpy(src1 ,mixed_data_all ,sizeof(src1));
//
//			}//end for m
//
//
//			for(n = 0; n < mix_process_handle->chnnel_num; n ++){  //mix_add(int* dest,short* src1,short* src2,unsigned int size)
////				//1.to mix the pcm data
////				if(mix_process_handle->one_channel_speech[n].have_data_mark == true){//only this channel have data ,then can call mix_sub function
////					mix_sub(mixed_data_others,mixed_data_all,mix_process_handle->one_channel_speech[n].speech_data,160);
////					scale(mix_process_handle ,target_pcm, mixed_data_others, 160);
////
////					((speex_encode_union_t *)(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle))->rtp_header.seq_number
////							=mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].dec_seq_number;
////					//2.encode pcm to speex
////					spx_encode_frame(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle ,target_pcm ,speex_data);
////
////					//3.send speex data call cb function
////					//the cb function I call only that who send me data .
////					mix_process_handle->mixed_data_send(speex_data ,sizeof(speex_data) ,mix_process_handle->one_channel_speech[n].channel_id ,mix_process_handle->priv_data); //send data.
////
////				}
//				//1.to mix the pcm data
//				if(mix_process_handle->one_channel_speech[n].have_data_mark == true){//only this channel have data ,then can call mix_sub function
//					mix_sub(mixed_data_others,mixed_data_all,mix_process_handle->one_channel_speech[n].speech_data,160);
//					scale(mix_process_handle ,target_pcm, mixed_data_others, 160);
//					((speex_encode_union_t *)(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle))->rtp_header.seq_number
//											=mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].dec_seq_number;
//
//					//2.encode pcm to speex
//					spx_encode_frame(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle ,target_pcm ,speex_data);
//
//				}else{ //add
//
//					//????? add scale function
//					scale(mix_process_handle ,target_pcm, mixed_data_all, 160);
//					spx_encode_frame(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle ,target_pcm ,speex_data);
//				}
//
//				if(n == 0){
//					fwrite(target_pcm ,2 ,160 ,mixed_channel_1);
//				}else if (n == 1){
//					fwrite(target_pcm ,2 ,160 ,mixed_channel_2);
//				}
////				//2.encode pcm to speex
////				spx_encode_frame(mix_process_handle->one_channel_speech[mix_process_handle->one_channel_speech[n].speech_index].speex_encode_handle ,target_pcm ,speex_data);
//
//				if(n == 0){
//					fwrite(speex_data ,1 ,50 ,spx_channel_1);
//				}else if (n == 1){
//					fwrite(speex_data ,1 ,50 ,spx_channel_2);
//				}
//
//				//3.send speex data call cb function
//				//the cb function I call only that who send me data .
//				mix_process_handle->mixed_data_send(speex_data ,sizeof(speex_data) ,mix_process_handle->one_channel_speech[n].channel_id ,mix_process_handle->priv_data); //send data.
//
//
//			}//end for m
//
//			scale(mix_process_handle ,target_pcm, mixed_data_all, 160);
//			spx_encode_frame(mix_process_handle->speex_encode_handle ,target_pcm ,speex_data);
//			mix_process_handle->mixed_data_send(speex_data ,sizeof(speex_data) ,0 ,mix_process_handle->priv_data); //0 mean to other people who dont have not authority
//			//fwrite(speex_data ,1 ,50 ,fp_mix);
//		}//end if...else
//
//	}//end for..i
//
//fail:
//	//2.do some work in the end of the mix_data function
//	int j = 0;
//	for(j = 0; j < mix_process_handle->chnnel_num; j ++){
//		ringq_init(&mix_process_handle->one_channel_speech[j].ringq_data);
//		mix_process_handle->one_channel_speech[j].have_data_mark = false;
//	}
//
//	//mix_process_handle->chnnel_num = 0;
//	//mix_process_handle->speech_index = -1;					 //have no speech data
//	mix_process_handle->max_num_packet = 0;
//
//}
//
///*
//*interface 4: mix_destroy function ,to release some resource
//*
//**/
//void mix_destroy(mix_process_t *mix_process_handle){
//
//	int i = 0;
//	for(i = 0; i < mix_process_handle->chnnel_num; i ++){
//
//		//decode destroy
//		spx_decode_destroy(mix_process_handle->one_channel_speech[i].speex_decode_handle);
//
//		//encode destroy
//		spx_encode_destroy(mix_process_handle->one_channel_speech[i].speex_encode_handle);
//	}
//	spx_encode_destroy(mix_process_handle->speex_encode_handle);
//}
