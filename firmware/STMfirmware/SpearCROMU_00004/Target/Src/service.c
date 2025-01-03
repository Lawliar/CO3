/*

Author: Joe Rogers <joe@cromulence.com>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "stdlib_cgc.h"
#include "common.h"
#include "service.h"

/*
Service that parses a pcm-encoded file on stdin
and decodes a morse-code-like symbol stream.
The resulting message is printed on stdout
*/

#define MAX_PCM_SIZE 8 * 1024
#define MAX_SAMPLES 2 * 1024
#define MAX_SYMBOLS 1024
#define MIN_VARIANCE 0.9
#define MAX_VARIANCE 1.1

extern struct language lh;

// used to identify mark and gap symbols in the data stream
struct time_bucket {
  short valid;
  unsigned int time;
  unsigned int count;
  char mark;
  char gap;
};
struct time_bucket Bucket[4];

/*
Once the pcm file has been decoded into dit and dah symbols
parse those into corresponding english letters
*/
int ParseCode(char *buf, char *output) {
  int i = 0;
  int o = 0;
  struct language *lp = &lh;

  while (buf[i] != '\0') {
    if (o > MAX_SYMBOLS-1) {
      puts_cgc("message too long\n");
      return UNABLE_TO_PARSE_PCM;
    }

    switch (buf[i]) {
      case '|':
        // end of a character
        if (lp) {
          output[o++] = lp->c;
          lp = &lh;
        } else {
          puts_cgc("unknown character\n");
          return UNABLE_TO_PARSE_PCM;
        }
        break;
      case ' ':
        // end of a word
        if (lp == &lh) {
          output[o++] = ' ';
        } else if (lp) {
          output[o++] = lp->c;
          lp = &lh;
          output[o++] = ' ';
        } else {
          puts_cgc("unknown character\n");
          return UNABLE_TO_PARSE_PCM;
        }
        break;
      case '.':
        // dit symbol
        if (lp) {
          lp = lp->dit;
        } else {
          puts_cgc("unknown character\n");
          return UNABLE_TO_PARSE_PCM;
        }
        break;
      case '-':
        // dah symbol
        if (lp) {
          lp = lp->dah;
        } else {
          puts_cgc("unknown character\n");
          return UNABLE_TO_PARSE_PCM;
        }
        break;
      case '\n':
        // newline...just ingore it
        break;

      default:
        puts_cgc("invalid symbol\n");
        return UNABLE_TO_PARSE_PCM;
    }

    i++;
  }
  if (lp) {
    output[o++] = lp->c;
    lp = &lh;
  } else {
    puts_cgc("unknown character\n");
    return UNABLE_TO_PARSE_PCM;
  }
  return 0;
}

/*
 * receive all requested bytes
 */
int recv(int fd, void *buf, size_t_cgc count, size_t_cgc *rx_bytes) {
	size_t_cgc total_bytes = 0;
	size_t_cgc rx;
	int rval;	

	while (total_bytes < count) {
		rval = receive_cgc( buf+total_bytes, 1, &rx);
		if (rval != 0) {
			return(rval);
		}
		if (rx != 1) {
			return(-1);
		}
		total_bytes++;
	}

	*rx_bytes = total_bytes;	
	return(0);
}

/*
Read the pcm file from stdin
*/
unsigned char *ReadWav(unsigned char *pcm, int *ret) {
  size_t_cgc size;
  unsigned int total_size;
  struct pcm_header *p;

  // read in the pcm header
  if (recv(STDIN, pcm, 12, &size) != 0) {
    puts_cgc("Read error\n");
    *ret = READ_ERROR;
    return NULL;
  }

  // use our pcm_header struct to check some input values
  p = (struct pcm_header *)pcm;
  if (p->ID != PCM) {
      puts_cgc("Invalid PCM format\n");
      *ret = (INVALID_PCM_FMT);
      return NULL;
  }
  if (p->NumSamples > MAX_SAMPLES) {
      puts_cgc("Invalid PCM length\n");
    *ret = (INVALID_PCM_LEN);
    return NULL;
  }
  if (p->NumSamples == 0) {
      puts_cgc("Invalid PCM format\n");
    *ret = (INVALID_PCM_FMT);
    return NULL;
  }

#ifdef PATCHED
  // make sure the SampleSize value is valid
  if (p->DataSize*8.0 / p->NumSamples != 16.0) {
    puts_cgc("Invalid PCM length\n");
    _terminate(INVALID_PCM_LEN);
  }
#else
  // make sure the SampleSize value is valid
  // BUG. Check is done with integer division instead of
  // floating point which leads to an incorrectly
  // checked DataSize
  if (p->DataSize*8/p->NumSamples != 16) {
    puts_cgc("Invalid PCM length\n");
    *ret = (INVALID_PCM_LEN);
    return NULL;
  }
#endif

  // read in the data portion of the pcm
  total_size = 0;
  while (total_size != p->DataSize) {
    if (receive_cgc(pcm+12+total_size, (p->DataSize-total_size), &size) != 0) {
      puts_cgc("Read error\n");
      *ret = (READ_ERROR);
      return NULL;
      }
    if (size == 0) {
      puts_cgc("Read error\n");
      *ret = (READ_ERROR);
      return NULL;
    }
    total_size +=  size;
  }
  *ret = 0;
  return(pcm);
}

  /*
For a given symbol detected in the pcm file,
assign it to a paritcular bucket of similar
symbol length.
*/
int AssignToBucket(unsigned int symbol_time, int * ret) {
  int i;

  // find a valid bucket to hold this symbol
  for (i = 1; i < 4; i++) {
    if (Bucket[i].valid) {
      if (Bucket[i].time > symbol_time*MIN_VARIANCE && Bucket[i].time < symbol_time*MAX_VARIANCE) {
        // found match, count it
        Bucket[i].count++;
        *ret = 0;
        return(i);
      }
    }
  }
  // didn't find one, so allocate a new one (if possible)
  for (i = 1; i < 4; i++) {
      if (Bucket[i].valid == 0) {
        // found emtpy bucket, populate it
        Bucket[i].valid = 1;
        Bucket[i].time = symbol_time;
        Bucket[i].count = 1;
        *ret = 0;
        return(i);
      }
  }

  puts_cgc("Couldn't find a bucket\n");
  * ret = (UNABLE_TO_PARSE_PCM);
  return 0;
}

/*
Check that the symbol bucket times are
at the correct ratios
*/
int CheckLimits(int dit, int dah, int word) {
  if (Bucket[dah].time < Bucket[dit].time*3*MIN_VARIANCE || Bucket[dah].time > Bucket[dit].time*3*MAX_VARIANCE) {
    // Bucket dah is outside of our limits
    puts_cgc("Too much variance in symbol times\n");
    return (UNABLE_TO_PARSE_PCM);
  }
  if (word) {
    if (Bucket[word].time < Bucket[dit].time*7*MIN_VARIANCE || Bucket[word].time > Bucket[dit].time*7*MAX_VARIANCE) {
      // Bucket word is outside of our limits
      puts_cgc("Too much variance in symbol times\n");
      return (UNABLE_TO_PARSE_PCM);
    }
  }
  return 0;
}

/*
Figure out how the pcm symbol timings correlate
to morse-like symbols
*/
int ValidateBuckets(void) {
  int dit = 0;
  int dah = 0;
  int word = 0;

  // if all three buckets are valid,
  // the largest bucket is word gap
  // second largest is inter-char gap or dah
  // third largest is intra-char gap or dit
  if (Bucket[1].valid && Bucket[2].valid && Bucket[3].valid) {
    if (Bucket[1].time > Bucket[2].time && Bucket[1].time > Bucket[3].time) {
      word = 1;
      if (Bucket[2].time > Bucket[3].time) {
        dah = 2;
        dit = 3;
      } else {
        dah = 3;
        dit = 2;
      }
    } else if (Bucket[2].time > Bucket[1].time && Bucket[2].time > Bucket[3].time) {
      word = 2;
      if (Bucket[1].time > Bucket[3].time) {
        dah = 1;
        dit = 3;
      } else {
        dah = 3;
        dit = 1;
      }
    } else if (Bucket[3].time > Bucket[1].time && Bucket[3].time > Bucket[2].time) {
      word = 3;
      if (Bucket[1].time > Bucket[2].time) {
        dah = 1;
        dit = 2;
      } else {
        dah = 2;
        dit = 1;
      }
    }

  // if only two are valid,
  // largest is the inter-char gap or dah
  // second largest is intra-char gap or dit
  } else if (Bucket[1].valid && Bucket[2].valid) {
    word = 0;
    if (Bucket[1].time > Bucket[2].time) {
      dah = 1;
      dit = 2;
    } else {
      dah = 2;
      dit = 1;
    }
  } else {
      puts_cgc("Insufficient symbol diversity\n");
      return (INVALID_PCM_FMT);
  }

  Bucket[word].mark = ' ';
  Bucket[word].gap = ' ';
  Bucket[dah].mark = '-';
  Bucket[dah].gap = '|';
  Bucket[dit].mark = '.';
  Bucket[dit].gap = '\0';
  int ret = CheckLimits(dit,dah,word);
  return ret;
}

/*
Read the samples in the pcm file looking
for periods of tones and silence
*/
int ParseWav(unsigned char *pcm) {
  unsigned char *pcm_pos;
  unsigned char *end_pcm;
  unsigned int symbol_start;
  unsigned int symbol_end;
  struct pcm_header *p;
  int silence;
  int count;
  short s;
  char Symbols[MAX_SYMBOLS];
  int sp = 0;
  unsigned int symbol_time;
  char output[MAX_SYMBOLS];
  char output2[MAX_SYMBOLS];
  int i;
  int b;
  int first = 1;
  int ret;

  // init some vars
  bzero_cgc(Symbols, MAX_SYMBOLS);
  bzero_cgc(output, MAX_SYMBOLS);
  sp = 0;
  Bucket[1].valid = 0;
  Bucket[2].valid = 0;
  Bucket[3].valid = 0;
  p = (struct pcm_header *)pcm;

  // calc the end byte of the pcm data
  end_pcm = pcm+12+(p->DataSize);

  // start with the first sample
  pcm_pos = pcm+12;
  symbol_start = 12;
  symbol_end = 0;
  silence = 1;
  count = 0;
  while (pcm_pos < end_pcm && sp < MAX_SYMBOLS-2) {
    // grab the sample's value
    s = *(short *)pcm_pos;

    // see if it's a new silence or a tone
    if ((s > -10 && s < 10) && (!silence || first)) {
      // got a new silence

      // check 50 sample to be sure
      if (++count < 50) {
        pcm_pos+=2;
        continue;
      }

      // see if this is our first symbol
      if (first) {
        silence = 1;
        count = 0;
        first = 0;
        pcm_pos = pcm+12;
        symbol_start = 12;
        symbol_end = 0;
        continue;
      }

      // yep, found new silence, reset our flag vars
      silence = 1;
      count = 0;

      // calc the length of the previous tone symbol
      symbol_end = (int)(pcm_pos-pcm);
      symbol_time = ((symbol_end-(50*2)-symbol_start))/2;

      // get ready for the next symbol
      symbol_start = symbol_end-(50*2);

      // Assign to a symbol bucket
      Symbols[sp++] = AssignToBucket(symbol_time, &ret);
      if(ret != 0){
        return UNABLE_TO_PARSE_PCM;
      }

    } else if ((s < -10 || s > 10) && (silence || first)) {
      // got a new tone

      // check 50 sample to be sure
      if (++count < 50) {
        pcm_pos+=2;
        continue;
      }

      // see if this is our first symbol
      if (first) {
        silence = 0;
        count = 0;
        first = 0;
        pcm_pos = pcm+12;
        symbol_start = 12;
        symbol_end = 0;
        continue;
      }

      // yep, found new tone, reset our flag vars
      silence = 0;
      count = 0;

      // calc the length of the previous tone symbol
      symbol_end = (int)(pcm_pos-pcm);
      symbol_time = ((symbol_end-(50*2)-symbol_start))/2;

      // get ready for the next symbol
      symbol_start = symbol_end-(50*2);

      // Assign to a symbol bucket
      Symbols[sp++] = AssignToBucket(symbol_time, &ret) * -1;
      if(ret != 0){
        return UNABLE_TO_PARSE_PCM;
      }
    } else {
      count = 0;
    }
    pcm_pos+=2;
  }

  // handle the last symbol we were processing
  symbol_end = (int)(pcm_pos-pcm);
  symbol_time = ((short)(symbol_end-symbol_start))/2;
  if (!silence) {
    Symbols[sp++] = AssignToBucket(symbol_time,  &ret);
    if(ret != 0){
        return UNABLE_TO_PARSE_PCM;
      }
  }

  // validate the buckets we discovered
  ret = ValidateBuckets();
  if(ret != 0){
    return ret;
  }
  // now that we've parsed the tones and silences
  // use that data to build the symbol string
  sp = 0;
  for (i = 0; i < MAX_SYMBOLS-1; i++) {
    if (Symbols[i] == 0) {
      continue;
    }
    // silence
    if (Symbols[i] < 0) {
      b = Symbols[i]*-1;
      if (Bucket[b].gap != '\0') {
        output[sp++] = Bucket[b].gap;
      }

    // tone
    } else {
      b = Symbols[i];
      if (Bucket[b].mark != '\0') {
        output[sp++] = Bucket[b].mark;
      }
    }
  }
  output[sp++] = '\n';

  // parse the symbols back to characters
  ret = ParseCode(output, output2);
  if(ret != 0){
    return ret;
  }
  // output the resulting message
  puts_cgc(output2);
  puts_cgc("\n");
  return 0;
}

int test(void) {
    unsigned char pcm[MAX_PCM_SIZE];

    // zero the pcm var
    bzero_cgc(pcm, MAX_PCM_SIZE);

    // init the language data structures
    InitLang(&lh, NULL);

    // read in the pcm file
    int ret;
    ReadWav(pcm,&ret);
    if(ret != 0){
    	whole_free();
      return -1;
    }
    // Read in each symbol
    ret = ParseWav(pcm);
    if(ret != 0){
    	whole_free();
      return -1;
    }
	whole_free();
    return(0);
}
