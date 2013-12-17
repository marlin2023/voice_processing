/*
 * rtp_header.h
 *
 *  Created on: Dec 5, 2013
 *      Author: chris
 */

#ifndef RTP_HEADER_H_
#define RTP_HEADER_H_
#include <stdint.h>
#define RTP_HEADER_SIZE                12

typedef struct rtp_header
{
#ifdef  ORTP_BIGENDIAN
        uint16_t version:2;
        uint16_t padbit:1;
        uint16_t extbit:1;
        uint16_t cc:4;
        uint16_t markbit:1;
        uint16_t paytype:7;
#else
        uint16_t cc:4;
        uint16_t extbit:1;
        uint16_t padbit:1;
        uint16_t version:2;
        uint16_t paytype:7;
        uint16_t markbit:1;
#endif

        uint16_t seq_number;		//here ,just use to test
        uint32_t timestamp;
        uint32_t ssrc;
        uint32_t csrc[16];
} rtp_header_t;


#endif /* RTP_HEADER_H_ */
