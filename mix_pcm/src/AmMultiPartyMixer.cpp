/*
 * AmMultiPartyMixer.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: chris
 */
#include "AmMultiPartyMixer.h"

#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "speex_decode.h"
#include "speex_encode.h"
#include "yyt_error.h"

#define PCM16_B2S(b) ((b) >> 1)
#define PCM16_S2B(s) ((s) << 1)

#define SYSTEM_SAMPLERATE 8000 // fixme: sr per session

// PCM16 range: [-32767:32768]
#define MAX_LINEAR_SAMPLE 32737

// the internal delay of the mixer (between put and get)
#define MIXER_DELAY_MS 40		//20
#define MIXER_DELAY    MIXER_DELAY_MS*SYSTEM_SAMPLERATE/1000	 //is 160 samples

AmMultiPartyMixer::AmMultiPartyMixer(long long ts)
  : channels(),
    cur_channel_id(0),
    scaling_factor(16)
{
//	//set the mixer base_ts
//	struct timeval tv;
//	gettimeofday(&tv ,NULL);
//	base_ts = tv.tv_sec * 1000 + tv.tv_usec /1000 ; //in millisecond
    base_ts = ts;
    fp_recv_spx  = fopen("/tmp/recv_spx1.spx" ,"wb+");
    if(fp_recv_spx == NULL){
    		printf("####1\n####1\n####1\n####1\n####1\n####1\n####1\n");
    }
    speex_encode_handle  = spx_encode_init();
	if(speex_encode_handle <= 0){
		printf("THE MAIN speex_encode_handle initialize failed .\n");
		exit(1);		//????
	}

	fp_mix_spx = fopen("/tmp/mix.spx" ,"wb+");
}

AmMultiPartyMixer::~AmMultiPartyMixer()
{

	ChannelMap::iterator channel_it;
	//free memory
	for(channel_it = channels.begin();channel_it!=channels.end();)
	{
		//cout<<iter->first<<":"<<iter->second<<endl;
		channel_handle_t *channel_handle1 = channel_handle.find(channel_it->first)->second;
		free(channel_handle1);

		delete channel_it->second;
	}
}

bool AmMultiPartyMixer::isNewChannel(unsigned int channel_id){

	if(channels.find(channel_id) == channels.end()){ //not found ,is a new channel
		printf("channel_id :%d is not found ,and this is a new channel !\n" ,channel_id);
		return true;
	}
	return false;
}

int  AmMultiPartyMixer::addChannel(unsigned int channel_id , unsigned char* buffer ,long  long ts)
{
	channels.insert(std::make_pair(channel_id,new SampleArrayShort()));


	//the channel_id is the key
	channel_handle_t *channel_handle1 = (channel_handle_t *)malloc(sizeof(channel_handle_t));
	if(channel_handle1 == NULL){
		printf("malloc failed ..FILE:%s. %d\n" ,__FILE__ ,__LINE__);
		return -1;
	}

	//init the speex_decode_handle
	channel_handle1->speex_decode_handle = spx_decode_init();
	if( channel_handle1->speex_decode_handle <= 0){
		printf("speex_decode_handle initialize failed .\n");
		return SPEEX_DECODE_INIT_FAIL;
	}

	//init the speex_encode_handle
	channel_handle1->speex_encode_handle = spx_encode_init();
	if(channel_handle1->speex_encode_handle <= 0){
		printf("speex_encode_handle initialize failed .\n");
		return SPEEX_ENCODE_INIT_FAIL;

	}

//	//set the ts_base of ts_system
//	struct timeval tv;
//	gettimeofday(&tv ,NULL);
//	channel_handle1->ts_system_base = tv.tv_sec * 1000 + tv.tv_usec /1000 ; //in millisecond
	channel_handle1->ts_system_base = ts;

	//set ts_rtp
	channel_handle1->ts_rtp_base = get_rtp_stamp(buffer);

	//map set
	channel_handle.insert(std::make_pair(channel_id ,channel_handle1));
	return 0;
}

SampleArrayShort* AmMultiPartyMixer::get_channel(unsigned int channel_id)
{
  ChannelMap::iterator channel_it = channels.find(channel_id);
  if(channel_it == channels.end()){
    //printf("channel #%i does not exist\n",channel_id);	//error
    return NULL;
  }

  return channel_it->second;
}

void AmMultiPartyMixer::PutChannelPacket(unsigned int   channel_id,
                                         long long   timestamp,
                                         unsigned char* buffer,
                                         unsigned int   size)	 //spx data
{


  if(!size) return;
  assert(size <= AUDIO_BUFFER_SIZE);

//  fwrite( (char *)buffer ,1 ,50 ,fp_recv_spx);
  if(isNewChannel(channel_id)){
	  addChannel(channel_id, buffer ,timestamp);
  }
  SampleArrayShort* channel = 0;
  if((channel = get_channel(channel_id)) != 0){
	    channel_handle_t *channel_handle1 = channel_handle.find(channel_id)->second;
	    unsigned int   ts = get_rtp_stamp(buffer);
	    ts = (ts - channel_handle1->ts_rtp_base )
	    		+ ( channel_handle1->ts_system_base - base_ts )  * SYSTEM_SAMPLERATE/1000; //result is in samples

	    //decode and obtain pcm data
	    short pcm_data_buf[FRAME_SIZE] = {0};
	    int ret = spx_decode_frame(channel_handle1->speex_decode_handle  ,(char*)buffer ,pcm_data_buf);
	    if(ret < 0) return ;

	    fwrite( pcm_data_buf ,2 ,160 ,fp_recv_spx);
	    //put pcm data
		//unsigned samples = PCM16_B2S(size);
	    unsigned samples = FRAME_SIZE; // PCM16_B2S(FRAME_SIZE);
		unsigned int put_ts = ts + MIXER_DELAY;

		channel->put(put_ts,pcm_data_buf,samples);
		mixed_channel.get(put_ts,tmp_buffer,samples);

		mix_add(tmp_buffer,tmp_buffer,pcm_data_buf,samples);
		mixed_channel.put(put_ts,tmp_buffer,samples);
  }
  else {
    printf("MultiPartyMixer::PutChannelPacket: "
          "++channel #%i doesn't exist\n",channel_id);
  }

}

void AmMultiPartyMixer::GetChannelPacket(unsigned int   channel_id,
                                         long long   ts,		//ts is system
                                         unsigned char* buffer,
                                         unsigned int   size)   //size is the buffer size
{
  if(!size) return;
  assert(size <= AUDIO_BUFFER_SIZE);

  SampleArrayShort* channel = 0;
  if((channel = get_channel(channel_id)) != 0){
	channel_handle_t *channel_handle1 = channel_handle.find(channel_id)->second;
    unsigned int samples = FRAME_SIZE; //PCM16_B2S(FRAME_SIZE);  //PCM16_S2B this  multiply 2 //sampes in sample
    unsigned int user_ts = (ts - base_ts)* SYSTEM_SAMPLERATE/1000;

    short pcm_data_buf[FRAME_SIZE] = {0};
    mixed_channel.get(user_ts,tmp_buffer,samples);
    channel->get(user_ts,pcm_data_buf,samples);

    mix_sub(tmp_buffer,tmp_buffer,pcm_data_buf,samples);
    scale(pcm_data_buf,tmp_buffer,samples);

    //encode
    spx_encode_frame( channel_handle1->speex_encode_handle ,pcm_data_buf ,/*out*/(char *)buffer);
  }
  else {
	if(channel_id == 0){
		unsigned int samples = FRAME_SIZE; //PCM16_B2S(FRAME_SIZE);
		unsigned int user_ts = (ts - base_ts)* SYSTEM_SAMPLERATE/1000;
//		static int iiii = 0;
//		if(iiii == 0){
//			last_user_ts = user_ts;
//			iiii ++;
//		}
//
//		if(user_ts > last_user_ts){
//			user_ts = last_user_ts + 160;
//		}

		short pcm_data_buf[FRAME_SIZE] = {0};
		mixed_channel.get(user_ts,tmp_buffer,samples);
		scale(pcm_data_buf,tmp_buffer,samples);
		fwrite( pcm_data_buf ,2 ,160 ,fp_mix_spx);

		//encode
		spx_encode_frame( speex_encode_handle ,pcm_data_buf ,/*out*/(char *)buffer);

//		last_user_ts = user_ts;
	}else {
		printf("MultiPartyMixer::GetChannelPacket: "
				"--channel #%i doesn't exist\n",channel_id);
	}
  }
}

// int   dest[size/2]
// int   src1[size/2]
// short src2[size/2]
//
void AmMultiPartyMixer::mix_add(int* dest,int* src1,short* src2,unsigned int size)
{
  int* end_dest = dest + size;

  while(dest != end_dest)
    *(dest++) = *(src1++) + int(*(src2++));
}

void AmMultiPartyMixer::mix_sub(int* dest,int* src1,short* src2,unsigned int size)
{
  int* end_dest = dest + size;

  while(dest != end_dest)
    *(dest++) = *(src1++) - int(*(src2++));
}

void AmMultiPartyMixer::scale(short* buffer,int* tmp_buf,unsigned int size)
{
  short* end_dest = buffer + size;

  if(scaling_factor<64)
    scaling_factor++;

  while(buffer != end_dest){

    int s = (*tmp_buf * scaling_factor) >> 6;
    if(abs(s) > MAX_LINEAR_SAMPLE){
      scaling_factor = abs( (MAX_LINEAR_SAMPLE<<6) / (*tmp_buf) );
      if(s < 0)
        s = -MAX_LINEAR_SAMPLE;
      else
        s = MAX_LINEAR_SAMPLE;
    }
    *(buffer++) = short(s);
    tmp_buf++;
  }
}



