/*
 * jitter_buffer.h
 *
 *  Created on: Dec 13, 2013
 *      Author: chris
 */

#ifndef JITTER_BUFFER_H_
#define JITTER_BUFFER_H_

#include <stdint.h>

#define NO_JITTER_BUFFER_DATA				12						//error number

#define DEFAULT_BUFFER_ELEMENTS						32//16				// 320ms given 20ms packets
#define	DEFAULT_RTP_LENTH							50				// the length of the rtp packet
static const int DEFAULT_MS_PER_PACKET   = 20;

typedef struct {
	unsigned char jitter_buf[DEFAULT_BUFFER_ELEMENTS][DEFAULT_RTP_LENTH];
	BOOL have_data[DEFAULT_BUFFER_ELEMENTS];

	unsigned char m_curosr;		//mark the next data to read



}jitter_buffer_t;


/*	member function	*/
//init
void jitter_buffer_init(jitter_buffer_t * jitter_buffer );

//push rtp  packet in jitter buffer
//return value :success return 0
int jitter_buffer_push(jitter_buffer_t * jitter_buffer,char *rtp_data ,int rtp_data_size);


//pop rtp packet from jitter buffer
//rtp_data_dest[out] use to storage data read from jitter buffer
//size[out]	size is the size of the rtp_data_dest
int jitter_buffer_pop(jitter_buffer_t * jitter_buffer, char *rtp_data_dest ,int size);


#endif /* JITTER_BUFFER_H_ */
