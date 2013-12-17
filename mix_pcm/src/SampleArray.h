/*
 * SampleArray.h
 *
 *  Created on: Dec 8, 2013
 *      Author: chris
 */

#ifndef _SampleArray_h_
#define _SampleArray_h_

/* MUST be a power of 2 */
#define SIZE_MIX_BUFFER   (1<<14)
/** \brief comparator for user timestamps */
struct ts_less
{
  bool operator()(const unsigned int& l,
		  const unsigned int& r) const;
};

/** \brief comparator for system timestamps
 * Note that system timestamps overflow at 48 bit boundaries.
 */
struct sys_ts_less
{
  bool operator()(const unsigned long long& l,
		  const unsigned long long& r) const;
};

/** \brief timed array of samples */
template <typename T>
class SampleArray
{
public:
  //protected:

  T samples[SIZE_MIX_BUFFER];
  unsigned int last_ts;
  bool         init;

  void clear_all();
  void clear(unsigned int start_ts,unsigned int end_ts);
  void write(unsigned int ts, T* buffer, unsigned int size);
  void read(unsigned int ts, T* buffer, unsigned int size);

  //public:
  SampleArray();

  /**
   * @param size buffer size in [samples].
   */
  void put(unsigned int ts, T* buffer, unsigned int size);

  /**
   * @param buf_size buffer size in [samples].
   */
  void get(unsigned int ts, T* buffer, unsigned int buf_size);
};

// 32 bit sample
typedef int IntSample;

// 32 bit sample array
typedef SampleArray<IntSample> SampleArrayInt;

// 16 bit sample
typedef short ShortSample;

// 16 bit sample array
typedef SampleArray<ShortSample> SampleArrayShort;

#include "SampleArray.cc"

#endif
