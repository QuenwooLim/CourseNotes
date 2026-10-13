/* -*- C++ -*-
 * mu_hash.C
 * @(#) Hash function class
 *
 * Copyright (C) 1992 - 1999 by the Board of Trustees of              
 * Leland Stanford Junior University.
 *
 * License to use, copy, modify, sell and/or distribute this software
 * and its documentation any purpose is hereby granted without royalty,
 * subject to the following terms and conditions:
 *
 * 1.  The above copyright notice and this permission notice must
 * appear in all copies of the software and related documentation.
 *
 * 2.  The name of Stanford University may not be used in advertising or
 * publicity pertaining to distribution of the software without the
 * specific, prior written permission of Stanford.
 *
 * 3.  This software may not be called "Murphi" if it has been modified
 * in any way, without the specific prior written permission of David L.
 * Dill.
 *
 * 4.  THE SOFTWARE IS PROVIDED "AS-IS" AND STANFORD MAKES NO
 * REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, BY WAY OF EXAMPLE,
 * BUT NOT LIMITATION.  STANFORD MAKES NO REPRESENTATIONS OR WARRANTIES
 * OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE
 * USE OF THE SOFTWARE WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS
 * TRADEMARKS OR OTHER RIGHTS. STANFORD SHALL NOT BE LIABLE FOR ANY
 * LIABILITY OR DAMAGES WITH RESPECT TO ANY CLAIM BY LICENSEE OR ANY
 * THIRD PARTY ON ACCOUNT OF, OR ARISING FROM THE LICENSE, OR ANY
 * SUBLICENSE OR USE OF THE SOFTWARE OR ANY SERVICE OR SUPPORT.
 *
 * LICENSEE shall indemnify, hold harmless and defend STANFORD and its
 * trustees, officers, employees, students and agents against any and all
 * claims arising out of the exercise of any rights under this Agreement,
 * including, without limiting the generality of the foregoing, against
 * any damages, losses or liabilities whatsoever with respect to death or
 * injury to person or damage to property arising from or out of the
 * possession, use, or operation of Software or Licensed Program(s) by
 * LICENSEE or its customers.
 *
 * Read the file "license" distributed with these sources, or call
 * Murphi with the -l switch for additional information.
 * 
 */

/* 
 * Original Author: Ralph Melton
 * Hash function made and added by Denis Leroy
 * Created: 21 May 95
 *
 * Update:
 *
 */ 

// changes by Uli
hash_function::hash_function(int vsize) {
  long int i,j,k;
  randomGen random;
  unsigned int r;

  vec_size = vsize;

  hashmatrix = (unsigned long *)
             malloc(vec_size*sizeof(unsigned long)*24);

  // initialize hashmatrix
  for (i=0; i<(vec_size*24); i++) {
    // generate dummy random numbers to get rid of dependencies
    k = long((r=random.next())%11 + 13);
    for (j=0; j<k; j++) {
      random.next();
    }
    hashmatrix[i] = random.next() ^ (r<<16);   // generator only yields 31 bit
                                               // random numbers
  }

  oldvec = new unsigned char[vec_size];
  for (i=0; i<vec_size; i++) {
    oldvec[i]=0;
  }
  key[0] = 0UL;
  key[1] = 0UL;
  key[2] = 0UL;
}

hash_function::~hash_function() {
  delete oldvec;
  free(hashmatrix);
}

// changes by Uli
inline unsigned long *
hash_function::hash(state* s, bool valid)
// Uli: calculates the hash function for a state
// - if valid is TRUE, curstate must point to a state and this state is 
//  used for differential hashing (the hashkeys[] must have been set cor-
//  rectly)
// - otherwise, buffer oldvec is used
{
  register unsigned long l0, l1, l2;
  register unsigned char qq;
  register unsigned char *q=s->bits, *qp;
  register unsigned char mask;
  register int ind = 0, i;
  int h = vec_size;

  // set the correct old values of vector and key
  // only in the aligned version the hashkeys are stored with the state
#ifdef ALIGN
  if (valid)
  {
    l0 = curstate->hashkeys[0];
    l1 = curstate->hashkeys[1];
    l2 = curstate->hashkeys[2];
    qp = curstate->bits;
  }
  else
#endif
  {
    l0 = key[0];
    l1 = key[1];
    l2 = key[2];
    qp = oldvec;
  }

  do {
    if (qq = *qp ^ *q)
      {
	mask = 1;
	for (i=ind; i<ind+24;i+=3) /* scan all bits of current byte */
	  {
	    if (qq & mask)
	      {
		l0 ^= hashmatrix[i];
		l1 ^= hashmatrix[i+1];
		l2 ^= hashmatrix[i+2];
	      }
	    mask = mask<<1;
	  }
#ifdef ALIGN
        if (!valid)
#endif
          *qp = *q;   // set the oldvec
      }
    q++; qp++;
    ind += 24;
  } while (--h > 0);

#ifdef ALIGN
  s->hashkeys[0] = l0;
  s->hashkeys[1] = l1;
  s->hashkeys[2] = l2;
  if (!valid)
#endif
  {
    key[0] = l0;
    key[1] = l1;
    key[2] = l2;
  }

#ifdef ALIGN
  return s->hashkeys;
#else
  return key;
#endif
}

/********************
  $Log: mu_hash.C,v $
  Revision 1.2  1999/01/29 07:49:10  uli
  bugfixes

  Revision 1.4  1996/08/07 18:54:33  ip
  last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

  Revision 1.3  1996/08/07 01:00:18  ip
  Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

  Revision 1.2  1996/08/07 00:15:26  ip
  fixed while code generation bug

  Revision 1.1  1996/08/07 00:14:46  ip
  Initial revision

********************/
