/* -*- C++ -*- 
 * mu_statecl.h
 * @(#) header for class state in the verifier
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
 *
 */

/* 
 * Created by Denis Leroy
 * Created: Spring 95
 *
 * Update:
 *
 */ 

#ifndef ALIGN
#define BLOCKS_IN_WORLD (((BITS_IN_WORLD + (4*BITS( BIT_BLOCK )) - 1 ) / (4*BITS(BIT_BLOCK )))*4)
#else
#define BLOCKS_IN_WORLD (((BITS_IN_WORLD + (4*BITS( BIT_BLOCK )) - 1 ) / (4*BITS(BIT_BLOCK )))*4)
#endif

/****************************************   // added by Uli
  class for pointer to previous state
  ****************************************/

class state;

class StatePtr
{
private:
  union {
    state*        sp;   // real pointer
    unsigned long lv;   // state number used in trace info file
  };

  // StatePtr is a member of the class state, so try to avoid adding
  // data members

  inline void sCheck();
  inline void lCheck();

public:
  StatePtr() {}
  StatePtr(state* s);
  StatePtr(unsigned long l);

  void set(state* s);
  void set(unsigned long l);
  void clear();
  state* sVal();
  unsigned long lVal();

  StatePtr previous();      // return StatePtr to previous state
  bool isStart();           // check if I point to a startstate
  bool compare(state* s);   // compare the state I point to with s
 
};


/****************************************
  class state.
  ****************************************/
// Warning: DO NOT add member variables to this class unless
// you know exactly what you're doing. Since an instance of the
// state class is created for each table entry during
// initialization, adding stuff here will eat lots of memory.
// Uli: this is only true if one does not use hash compaction

#ifdef HASHC
hash_function *h3;
#endif

class state
{
public:
  BIT_BLOCK bits[ BLOCKS_IN_WORLD ];
  StatePtr previous;   // state from which this state was reached.
#ifdef HASHC
#ifdef ALIGN
  // Uli: only in the aligned version the hashkeys are stored with the state
  unsigned long hashkeys[3];
#endif
#endif

  state()
  : previous()
  {
    memset((void *)bits, 0, (size_t)BLOCKS_IN_WORLD);   // Uli: avoid bzero
  };
  state(state * s) 
  {
    StateCopy(this, s); 
  };

friend void StateCopy(state * l, state * r);
friend int StateCmp(state * l, state * r);
friend void copy_state(state *& s);
#ifdef HASHC
friend class hash_function;
#endif
//friend unsigned long* hash_function::hash(state*, bool);
// friend bool StateEquivalent(state * l, state * r);   // Uli: not necessary
  
  // get it with Horner\'s method. 
  // size  <= the number of bits in an integer - 1
#ifndef ALIGN
  inline int get(const position *w) const {   // Uli: const added
    unsigned int val, *l;
    l = (unsigned int *)(bits + w->longoffset);
    val = (l[0] & w->mask1) >> w->shift1;
    if (w->mask2)
	val |= (l[1] & w->mask2) << w->shift2;
    return (int)val;
  }
#else
  inline int get(int byteloc) const { return bits[byteloc]; };
  inline int getlong(int byteloc) const {
    unsigned int  val;
    unsigned char *p = (unsigned char *)&bits[byteloc];
    val = *p++;
    val |= *p++ << 8;
    val |= *p++ << 16;
    val |= *p++ << 24;
    return (int)val;
  }
#endif

  
  // set a field to value 
  // size  <= the number of bits in an integer - 1
#ifndef ALIGN
  inline void set(position *w, int value) {
    unsigned int val, *l;
    l = (unsigned int *)(bits + w->longoffset);
    val = value << w->shift1;
    l[0] = (l[0] & ~w->mask1) | val;
    if (w->mask2) {
      val = (value >> w->shift2) & w->mask2;
      l[1] = (l[1] & ~w->mask2) | val;
    }
  };
#else
  inline void setlong(int byteloc, int value) {
    unsigned char *p = (unsigned char *)&bits[byteloc];
    *p++ = (unsigned)value & 0xff;
    *p++ = ((unsigned)value & 0xff00) >> 8;
    *p++ = ((unsigned)value & 0xff0000) >> 16;
    *p++ = ((unsigned)value & 0xff000000) >> 24;
  };
#endif
  
  // key for hash function, changes by Uli
#ifndef HASHC
  unsigned long hashkey(void) const 
  {
    unsigned long sum = 0;
    unsigned long *pt = (unsigned long *)bits;

    for (int i = BLOCKS_IN_WORLD>>2; i>0; i--) {
      sum += *pt++;
    }
    return sum;
  }
#endif

  
  // operator== and operator!= only consider the bitvectors,
  // not the other components of a state. 
  inline bool operator == (state& other) const
  { return ( memcmp(&bits, &other.bits, BLOCKS_IN_WORLD) == 0); };
  inline bool operator != (state& other) const
  { return ( memcmp(&bits, &other.bits, BLOCKS_IN_WORLD) != 0); };
  
  // scribbles over the current world variables. 
  void print();

  // symmetry reduction
  void Normalize();

  // multiset reduction
  void MultisetSort();

};

/********************
  $Log: mu_statecl.h,v $
  Revision 1.2  1999/01/29 07:49:11  uli
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
