/*
 * AmMultiPartyMixer.h
 *
 *  Created on: Dec 8, 2013
 *      Author: chris
 */

#ifndef AMMULTIPARTYMIXER_H_
#define AMMULTIPARTYMIXER_H_

#include "SampleArray.h"
#include "mix_pcm.h"

#include <stdio.h>
#include <map>

/** AUDIO_BUFFER_SIZE must be a power of 2 */
#define AUDIO_BUFFER_SIZE (1<<12) /* 2 KB */


/**
 * \brief Mixer for one conference.
 *
 * AmMultiPartyMixer mixes the audio from all channels,
 * and returns the audio of all other channels.
 */
class AmMultiPartyMixer
{
  typedef std::map<int,SampleArrayShort*> ChannelMap;
  typedef std::map<int,channel_handle_t*> ChannelHandleMap;

  ChannelMap       channels;
  ChannelHandleMap	channel_handle;		//every channel has one channel_handle
  unsigned int     cur_channel_id;

  //test file
  FILE *fp_recv_spx;
  FILE *fp_mix_spx;
  SampleArrayInt   mixed_channel;
  int              scaling_factor;
  int              tmp_buffer[AUDIO_BUFFER_SIZE/2];

  long long     base_ts;						//in millisecond
  YYT_HANDLE_T 		speex_encode_handle;		//encode handle for all people who can speak

  //following two variable
  long long last_ts;
  unsigned int last_user_ts;

  SampleArrayShort* get_channel(unsigned int channel_id);

  void mix_add(int* dest,int* src1,short* src2,unsigned int size);
  void mix_sub(int* dest,int* src1,short* src2,unsigned int size);
  void scale(short* buffer,int* tmp_buf,unsigned int size);

public:
  AmMultiPartyMixer(long long ts);
  ~AmMultiPartyMixer();

  bool isNewChannel(unsigned int channel_id );

  int addChannel(unsigned int channel_id ,unsigned char * buffer ,long  long  ts);

  void PutChannelPacket(unsigned int   channel_id,
                        long long	   timestamp,
                        unsigned char* buffer,
                        unsigned int   size);

  void GetChannelPacket(unsigned int   channel,
                        long long   timestamp,
                        unsigned char* buffer,
                        unsigned int   size);
};



#endif /* AMMULTIPARTYMIXER_H_ */
