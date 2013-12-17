///*********************************************************************************
//  *Copyright(C),2013-2014,Yinyuetai.com
//  *FileName:  // jitter_buffer.cpp
//  *Author:  //chris
//  *Version:  //0.0.1
//  *Date:  //2013/12/13
//  *Description:  //implement rtp jitter buffer
//**********************************************************************************/
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#include "jitter_buffer.h"
#include "rtp_header.h"

void jitter_buffer_init(jitter_buffer_t * jitter_buffer ){

	jitter_buffer->m_curosr = DEFAULT_BUFFER_ELEMENTS / 2;

	int i ;
	for(i = 0; i < DEFAULT_BUFFER_ELEMENTS ;i++){
		jitter_buffer->have_data[i] = FALSE; //false
	}
}

//push rtp  packet in jitter buffer
//in our rtp_data_size is 50
int jitter_buffer_push(jitter_buffer_t * jitter_buffer, char *rtp_data ,int rtp_data_size){

	//assert
	if(rtp_data_size != DEFAULT_RTP_LENTH){ return -1;}

	//parse rtp packet and get the sequnce number
	rtp_header_t rtp_header;
	memcpy(&rtp_header ,rtp_data ,RTP_HEADER_SIZE);

	//compute the  jitter_buffer_index
	int jitter_buffer_index = ntohs(rtp_header.seq_number) & (DEFAULT_BUFFER_ELEMENTS - 1);

	//memcpy rtp_data
	memcpy(&(jitter_buffer->jitter_buf[jitter_buffer_index]) ,rtp_data ,rtp_data_size );
	//set mark
	jitter_buffer->have_data[jitter_buffer_index] = TRUE;

	return 0;

}


//pop rtp packet from jitter buffer
//rtp_data_dest[out] use to storage data read from jitter buffer
//size[out]	size is the size of the rtp_data_dest
int jitter_buffer_pop(jitter_buffer_t * jitter_buffer ,char *rtp_data_dest ,int size){

	//assert
	if(size < DEFAULT_RTP_LENTH) {return -1;}

	if(jitter_buffer->have_data[jitter_buffer->m_curosr] == FALSE){
		//no data
		//update the cursor
		jitter_buffer->m_curosr = (jitter_buffer->m_curosr + 1) % DEFAULT_BUFFER_ELEMENTS;
		//printf("jitter buffer no data .\n");
		return NO_JITTER_BUFFER_DATA;
	}
	//memcpy rtp_data
	memcpy(rtp_data_dest ,&(jitter_buffer->jitter_buf[jitter_buffer->m_curosr]) ,size);
	//set mark
	jitter_buffer->have_data[jitter_buffer->m_curosr] = FALSE;

	//update the cursor
	jitter_buffer->m_curosr = (jitter_buffer->m_curosr + 1) % DEFAULT_BUFFER_ELEMENTS;

	return 0;
}
