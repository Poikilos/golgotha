/********************************************************************** <BR>
   This file is part of Crack dot Com's free source code release of
   Golgotha. <a href="http://www.crack.com/golgotha_release"> <BR> for
   information about compiling & licensing issues visit this URL</a>
   <PRE> If that doesn't help, contact Jonathan Clark at
   golgotha_source@usa.net (Subject should have "GOLG" in it)
 ***********************************************************************/

#include "pch.h"
//very fast, but very bad random number generator.
//I think this is done so badly, because we want to get shure that the "random" sequence
//is reproduceable. But we should better save the seed in the save-file, so we can use a
//real random seed if appropriate.
enum {
	TSIZE=1024
};
static w32 w32_rtable[TSIZE];
static float f_rtable[TSIZE];
static int seed=0;
static int initialized=0;

void next_seed()
{
	if (!initialized)
	{
		w32 inc=0xacd2d391, mul=0xf34f9201, x=0;
		for (int i=0; i<TSIZE; i++)
		{
			w32_rtable[i]=x;
			f_rtable[i]=(float)((double)x/(double)0xffffffff);
			x=(x+inc)*mul;
		}
		initialized=1;
	}

	seed=(seed +1)%TSIZE ;
}

w32 i4_rand()
{
	next_seed();
	return w32_rtable[seed];
}

float i4_float_rand()
{
	next_seed();
	return f_rtable[seed];
}

void i4_rand_reset(w32 _seed)
{
	seed=_seed % TSIZE;
}
