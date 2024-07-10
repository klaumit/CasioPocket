/*******************************************************************************
*******************************************************************************
*******************************************************************************

	[Title]	     qsort
	
	 	Perform a quick sort on an array starting at base.  The
		array is nel elements large and width is the size of a single
		element in chars.  Compare is a pointer to a comparison routine
		which will be passed pointers to two elements of the array.  It
		should return a negative number if the left-most argument is
		less than the rightmost, 0 if the two arguments are equal, a
		positive number if the left argument is greater than the right. 
		(That is, it acts like a "subtract" operator.).
				
		The 'far' version (far_qsort) performs the same operation over
		a far-allocated array. Pointers to array elements must be defined
		as far pointers also in the compare function.
		
	Author...
		Copyright (c) 1984  Allen I. Holub

		All rights reserved.
		This program may be copied for personal, non-profit use only.

		Published in Dr. Dobb's Journal #102 (Apr 85).

	Usage...

		Including a #define for DEBUG will make this file a stand-alone
		program which sorts argv and prints the result, along with all
		intermediate stages.  This is pretty instructive if you want to
		see how the quick sort works.


    Re-used by Fabio Fumi for the PVMail program
	f_fumi@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

		
******************************************************************************/

typedef int (*PFI)();	/* pointer to a function returning int */
static PFI Comp;		/* pointer to comparison routine */
static int Width;		/* width of an object in chars */
						
static char *a,*b;	  /* Used for exchanges, will not need to retain */
static int tmp,i;	  /* their values during the recursion so they	 */
static char far *f_a; /* can be static	                             */	
static char far *f_b;
	
static rqsort(char *low, char *high)
{
	/*	Workhorse function called by the access routine, qsort().
		Not externally accessible.										*/

	char *pivot;
	char *base;
	
    base=low;		/* remember base address of array	*/
	pivot=high;		/* partition off the pivot			*/
	high-=Width;

	do
		{while( low<high && (*Comp)(low,pivot) <= 0)  low += Width;
		while( low<high && (*Comp)(high,pivot) >= 0 ) high -= Width;
		if( low < high )	/* exchange low & high */
			{
			for ( b=low,a=high,i=Width ; --i>=0 ; )
				{tmp = *b;	/* Exchange *low and *high */
				*b++ = *a;
				*a++ = tmp;
				}
			}
		} while ( low<high );
		if( low < pivot && (*Comp)(low, pivot) > 0 )
			for ( b=low,a=pivot,i=Width ; --i >=0 ; )
				{tmp=*b ;	/* Exchange *low and *pivot */
				*b++=*a;
				*a++=tmp;
				}
		low += Width;
		if( high-base < pivot-low )
			{if( low < pivot ) rqsort( low , pivot );
			if(base < high ) rqsort( base , high );
			}
		else
			{if( base < high ) rqsort( base , high );
			if( low < pivot ) rqsort( low , pivot );
			}
}
		
char qsort(char *base,int nel,int width, int (*compare)())
{

	Width=width;
	Comp=compare;
	if(nel>1)
		rqsort(base,base+((nel-1)*width));

}

/******************************************************************************
    Far-memory version
******************************************************************************/
static far_rqsort(char far *low, char far *high)
{
	/*	Workhorse function called by the access routine, qsort().
		Not externally accessible.										*/

	char far *pivot;
	char far *base;
	
    base=low;		/* remember base address of array	*/
	pivot=high;		/* partition off the pivot			*/
	high -= Width;

	do
		{while( low<high && (*Comp)(low,pivot) <= 0)  low += Width;
		while( low<high && (*Comp)(high,pivot) >= 0 ) high -= Width;
		if( low < high )	/* exchange low & high */
			{
			for ( f_b=low,f_a=high,i=Width ; --i>=0 ; )
				{tmp = *f_b;	/* Exchange *low and *high */
				*f_b++ = *f_a;
				*f_a++ = tmp;
				}
			}
		} while ( low<high );
		if( low < pivot && (*Comp)(low, pivot) > 0 )
			for ( f_b=low,f_a=pivot,i=Width ; --i >=0 ; )
				{tmp=*f_b ;	/* Exchange *low and *pivot */
				*f_b++=*f_a;
				*f_a++=tmp;
				}
		low += Width;
		if( high-base < pivot-low )
			{if( low < pivot ) far_rqsort( low , pivot );
			if(base < high ) far_rqsort( base , high );
			}
		else
			{if( base < high ) far_rqsort( base , high );
			if( low < pivot ) far_rqsort( low , pivot );
			}
}
		
char far_qsort(char far *base,int nel,int width, int (*compare)())
{

	Width=width;
	Comp=compare;
	if(nel>1)
		far_rqsort(base, base+((nel-1)*width));

}

