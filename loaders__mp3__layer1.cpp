#include "pch.h"

/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

/*
 * Mpeg Layer-1 audio decoder
 * --------------------------
 * copyright (c) 1995 by Michael Hipp, All rights reserved. See also 'README'
 * near unoptimzed ...
 *
 * may have a few bugs after last optimization ...
 *
 */

#include "loaders/mp3/mpg123.h"

void I_step_one(unsigned int balloc[], unsigned int scale_index[2][SBLIMIT],struct frame * fr)
{
	unsigned int * ba=balloc;
	unsigned int * sca = (unsigned int *) scale_index;

	if(fr->stereo)
	{
		int i;
		int jsbound = fr->jsbound;
		for (i=0; i<jsbound; i++)
		{
			*ba++ = getbits(4);
			*ba++ = getbits(4);
		}
		for (i=jsbound; i<SBLIMIT; i++)
		{
			*ba++ = getbits(4);
		}

		ba = balloc;

		for (i=0; i<jsbound; i++)
		{
			if ((*ba++))
			{
				*sca++ = getbits(6);
			}
			if ((*ba++))
			{
				*sca++ = getbits(6);
			}
		}
		for (i=jsbound; i<SBLIMIT; i++)
		{
			if ((*ba++))
			{
				*sca++ =  getbits(6);
				*sca++ =  getbits(6);
			}
		}
	}
	else
	{
		int i;
		for (i=0; i<SBLIMIT; i++)
		{
			*ba++ = getbits(4);
		}
		ba = balloc;
		for (i=0; i<SBLIMIT; i++)
		{
			if ((*ba++))
			{
				*sca++ = getbits(6);
			}
		}
	}
}

void I_step_two(real fraction[2][SBLIMIT],unsigned int balloc[2 * SBLIMIT],
				unsigned int scale_index[2][SBLIMIT],struct frame * fr)
{
	int i,n;
	int smpb[2*SBLIMIT];/* values: 0-65535 */
	int * sample;
	register unsigned int * ba;
	register unsigned int * sca = (unsigned int *) scale_index;

	if(fr->stereo)
	{
		int jsbound = fr->jsbound;
		register real * f0 = fraction[0];
		register real * f1 = fraction[1];
		ba = balloc;
		for (sample=smpb,i=0; i<jsbound; i++)
		{
			if ((n = *ba++))
			{
				*sample++ = getbits(n+1);
			}
			if ((n = *ba++))
			{
				*sample++ = getbits(n+1);
			}
		}
		for (i=jsbound; i<SBLIMIT; i++)
		{
			if ((n = *ba++))
			{
				*sample++ = getbits(n+1);
			}
		}

		ba = balloc;
		for (sample=smpb,i=0; i<jsbound; i++)
		{
			if((n=*ba++))
			{
				*f0++ = (real) ( ((-1)<<n) + (*sample++) + 1) * muls[n+1][*sca++];
			}
			else
			{
				*f0++ = 0.0;
			}
			if((n=*ba++))
			{
				*f1++ = (real) ( ((-1)<<n) + (*sample++) + 1) * muls[n+1][*sca++];
			}
			else
			{
				*f1++ = 0.0;
			}
		}
		for (sample=smpb,i=jsbound; i<SBLIMIT; i++)
		{
			if ((n=*ba++))
			{
				real samp = ( ((-1)<<n) + (*sample++) + 1);
				*f0++ = samp * muls[n+1][*sca++];
				*f1++ = samp * muls[n+1][*sca++];
			}
			else
			{
				*f0++ = *f1++ = 0.0;
			}
		}
		for(i=SBLIMIT>>fr->down_sample; i<32; i++)
		{
			fraction[0][i] = fraction[1][i] = 0.0;
		}
	}
	else
	{
		register real * f0 = fraction[0];
		ba = balloc;
		for (sample=smpb,i=0; i<SBLIMIT; i++)
		{
			if ((n = *ba++))
			{
				*sample++ = getbits(n+1);
			}
		}
		ba = balloc;
		for (sample=smpb,i=0; i<SBLIMIT; i++)
		{
			if((n=*ba++))
			{
				*f0++ = (real) ( ((-1)<<n) + (*sample++) + 1) * muls[n+1][*sca++];
			}
			else
			{
				*f0++ = 0.0;
			}
		}
		for(i=SBLIMIT>>fr->down_sample; i<32; i++)
		{
			fraction[0][i] = 0.0;
		}
	}
}

int do_layer1(struct frame * fr,int outmode,struct audio_info_struct * ai)
{
	int clip=0;
	int i,stereo = fr->stereo;
	unsigned int balloc[2*SBLIMIT];
	unsigned int scale_index[2][SBLIMIT];
	real fraction[2][SBLIMIT];
	int single = fr->single;

	if(stereo == 1 || single == 3)
	{
		single = 0;
	}

	I_step_one(balloc,scale_index,fr);

	for (i=0; i<SCALE_BLOCK; i++)
	{
		I_step_two(fraction,balloc,scale_index,fr);

		if(single >= 0)
		{
			clip += (fr->synth_mono)( (real *) fraction[single],pcm_sample+pcm_point);
		}
		else
		{
			clip += (fr->synth)( (real *) fraction[0],0,pcm_sample+pcm_point);
			clip += (fr->synth)( (real *) fraction[1],1,pcm_sample+pcm_point);
		}
		pcm_point += fr->block_size;

		if(pcm_point == audiobufsize)
		{
			audio_flush(outmode,ai);
		}
	}

	return clip;
}
