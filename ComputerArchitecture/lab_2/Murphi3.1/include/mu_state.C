/* -*- C++ -*-
 * mu_state.C
 * @(#) Auxiliary routines related to states in the verifier
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
 * Extracted from mu_epilog.inc and mu_prolog.inc
 * by C. Norris Ip
 * Created: 21 April 93
 *
 * Update:
 *
 */ 

/****************************************
  There are 3 groups of implementations:
  1) bit vector
  2) class StatePtr and state related stuff
  3) state queue and stack
  4) state set
  ****************************************/

void
state::print()
{
  theworld.print();
};

/****************************************
  Bit vector - copied straight from Andreas. 
  ****************************************/
dynBitVec::dynBitVec( unsigned long nBits )
: numBits( nBits )
{
  v = new unsigned char[ NumBytes() ]; /* Allocate and clear vector. */
  ErrAlloc(v);
  memset( v, 0, NumBytes() );
};

dynBitVec::~dynBitVec()
{
  delete[ OLD_GPP(NumBytes()) ] v; // should be delete[].
}


/****************************************
  class StatePtr and state related stuff.
  ****************************************/

inline void StatePtr::sCheck() {
#ifdef HASHC
  if (args->trace_file.value)
    Error.Notrace("Internal: Illegal Access to StatePtr.");
#endif
}

inline void StatePtr::lCheck() {
#ifdef HASHC
  if (!args->trace_file.value)
    Error.Notrace("Internal: Illegal Access to StatePtr.");
#endif
}

StatePtr::StatePtr(state* s) { sCheck(); sp=s; }
StatePtr::StatePtr(unsigned long l) { lCheck(); lv=l; }

void StatePtr::set(state* s) { sCheck(); sp=s; }
void StatePtr::set(unsigned long l) { lCheck(); lv=l; }
void StatePtr::clear() {
#ifdef HASHC
  if (args->trace_file.value) 
    lv=0;
  else
#endif 
    sp=NULL;
} 
state* StatePtr::sVal() { sCheck(); return sp; }
unsigned long StatePtr::lVal() { lCheck(); return lv; }

StatePtr StatePtr::previous() {   // return StatePtr to previous state
#ifdef HASHC
  if (args->trace_file.value)
    return TraceFile->read(lv)->previous;
  else
#endif
    return sp->previous.sp;
}

bool StatePtr::isStart() {   // check if I point to a startstate
#ifdef HASHC
  if (args->trace_file.value) {
    if (TraceFile->read(lv)->previous==0) return TRUE;
    return FALSE;
  }
  else 
#endif
  {
    if (sp->previous.sp==NULL) return TRUE;
    return FALSE;
  }
}

bool StatePtr::compare(state* s) {   // compare the state I point to with s
#ifdef HASHC
  if (args->trace_file.value) {
    unsigned long *key = h3->hash(s, FALSE);
    unsigned long c1 = key[1] &
      ((~0) << (args->num_bits.value>32 ? 0 : 32-args->num_bits.value));
    unsigned long c2 = key[2] &
      (args->num_bits.value>32 ? (~0)<<(64-args->num_bits.value) : 0UL);

    return ( c1==TraceFile->read(lv)->c1 &&
             c2==TraceFile->read(lv)->c2 );
  }
  else
#endif
    return (StateCmp(sp,s)==0);
}


void 
StateCopy(state * l, state * r)
// Uli: uses default assignment operator
{
  *l = *r;
}

int 
StateCmp(state * l, state * r)
{
  int i = BLOCKS_IN_WORLD/4;
  register int *d = (int *)l->bits, *s = (int *)r->bits;

  while (i--)
      if( *d > *s)
	  return 1;
      else if( *d++ < *s++)
	  return -1;
  return 0;
}

void 
copy_state(state *& s)
{
  state *h;

  if ( (h = new state) == NULL )
    Error.Notrace
      ("New failed. Swap space probably too small for state queue.");
  *h = *s;
  s = h;
}

bool 
StateEquivalent(state * l, StatePtr r)
{
  return match(l, r);
}

/****************************************
  class state_queue for searching the state space.
  ****************************************/
state_queue::state_queue( unsigned long mas )
:max_active_states(mas), num_elts(0), front(0), rear(0)
{
  stateArray = new state*[ max_active_states ];

  for ( long i = 0; i < max_active_states; i++)   // Uli: avoid bzero
    stateArray[i] = NULL;
};

state_queue::~state_queue()
{
  delete[ OLD_GPP(max_active_states) ] stateArray; // Should be delete[].
}

int 
state_queue::BytesForOneState( void ) {
  
#ifdef VER_PSEUDO
  // Pseudo ver: ptr + malloced state + approx. malloc&new overhead.
  return sizeof(state*) + sizeof(state) + 8;    
#else
  return sizeof(state*);				/* Full ver: only a ptr to state. */
#endif
}

void 
state_queue::Print( void )
{
  unsigned long i;
  unsigned long ind = front;
  for( i = 1; i < num_elts; i++ )
    {
      // convert to print in unsigned long format?
      cout << "State " << i << " [" << ind << "]:\n";
      stateArray[ ind ]->print();
      ind = (ind + 1) % max_active_states;
    }
}

void 
state_queue::enqueue( state* e )
{
  if( num_elts < max_active_states )
    {
      stateArray[ rear ] = e;
      rear = (rear + 1) % max_active_states;
      num_elts++;
    }
  else
    {
      cout << num_elts << " " << max_active_states;
      Error.Notrace( "Internal Error: Too many active states.", "num_elts", max_active_states );
    }
} 

state* 
state_queue::dequeue( void )
{ 
  state* retval;
  if( num_elts > 0 )
    {
      retval = stateArray[ front ];
      front = (front + 1) % max_active_states;
      num_elts--;
    }
  else
    {
      Error.Notrace( "Internal: Attempt to dequeue from empty state queue.", "", "" );
    }
  return retval;
}

state* 
state_queue::top( void )
{
  if ( num_elts>0 )
    {
      return stateArray[ front ];
    }
  else
    {
      Error.Notrace( "Internal: Attempt to top() empty state queue.", "", "" );
      return NULL;
    }
}

void
state_stack::enqueue( state* e )
{
  if( num_elts < max_active_states )
    {
      front = front == 0 ? max_active_states-1 : front-1;
      stateArray[ front ] = e;
      nextrule_to_try[ front ] = 0;
      num_elts++;
    }
  else
    {
      cout << num_elts << " " << max_active_states;
      Error.Notrace( "Internal: Too many active states.", "num_elts", max_active_states );
    }
}

/****************************************   // changes by Uli
  The Stateset implementation for recording all the states found.
  ****************************************/

int state_set::bits_per_state() 
{
#ifndef HASHC
  return 8*sizeof(state);
#else
  return args->num_bits.value;
#endif
}
  
state_set::state_set (unsigned long table_size )
: table_size (table_size), num_elts(0), num_elts_reduced(0), num_collisions(0)
{
#ifndef HASHC
  table = new state [table_size];
#else 
  assert (sizeof(Unsigned32)==4);   // the implementation is pretty depen-
                                    // dent on the 32 bits
  unsigned long size =
    (unsigned long)((double)table_size*args->num_bits.value/32) + 3;
    // higher precision necessary to avoid overflow
    // two extra elements needed in table
  table = new Unsigned32 [size];
  for (unsigned long i=0; i<size; i++)
    table[i]=0UL;
#endif
  Full = new dynBitVec ( table_size );
}

state_set::~state_set()
{
  delete[] table;   // only works for newer g++ versions
  delete Full;
}

// Uli: the two following routines were deleted because they were not called
//      any more and required changes
//void state_set::clear_state_set()
//void copy_state_set( state_set * set1, state_set * set2)

bool 
state_set::simple_was_present( state *& in, bool valid, bool permanent )
/* changes in to point to the first state found with that pattern. */
/* returns true iff the state was present in the hash table;
 * Otherwise, returns false and inserts the state. */
/* Algorithms directly from Andreas\' code. He cites CLR 235, 236. */
// Uli: pitfall: shift operators yield undefined values if the right
//               operand is equal to the length in bits of the left
//               operand (see ARM, pg.74)
// Uli: table_size must be prime
{
#ifndef HASHC
  unsigned long key = in->hashkey();
  unsigned long h1 = key % table_size;
  unsigned long h2 = 1 + key % ( table_size - 1 );
  unsigned long h = h1;
#else
  unsigned long *key = h3->hash(in, valid);
  unsigned long h1 = key[0] % table_size;
  unsigned long h2;
  register unsigned long h = h1;
  register unsigned long num_bits = args->num_bits.value;
  register unsigned long mask1 = (~0) << (num_bits>32 ? 0 : 32-num_bits);
  register unsigned long mask2 = num_bits>32 ? (~0)<<(64-num_bits) : 0UL;
  register unsigned long addr, offset;
  register unsigned long c1 = key[1]&mask1;
  register unsigned long c2 = key[2]&mask2;
  register unsigned long t1, t2;
#endif

#ifdef VER_PSEUDO
  if( is_empty(h) )
    {
      Full->set(h);
      num_elts++;
      return FALSE;
    }
  return TRUE;

#else
  unsigned long probe = 0;

#ifndef HASHC   
// no hash compaction, uses double hashing

  bool empty, equal= FALSE;

  while ( !(empty = is_empty(h)) &&
	  !(equal = ( *in == table[h] )) &&
	  ( probe < table_size ) )
    {
      h = (h1 + probe * h2) % table_size;   // double hashing
      num_collisions++;
      probe++;
    }
  if (empty)  /* Go ahead and insert the element. */
    {
      table[h] = *in;
      in = &table[h];
      Full->set(h);
      num_elts++;
      return FALSE;
    }
  else if (equal)
    {
      in = &table[h];
      return TRUE;
    }
  else
    {
      Error.Notrace("Closed hash table full.");
      return FALSE; /* it doesn\'t matter, but it shuts up g++. */
    }

#else   
// hash compaction, uses ordered hashing
// the state-insertion is done in two steps: search and insertion

  // search - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  h2 = 1 + c1 % (table_size-1);   // calculation uses compressed value

  do 
  {
    // calculate address and offset in table
    // 32 bit arithmetic not sufficient and may cause overflow
    // addr = (h*num_bits) / 32
    // offset = (h*num_bits) % 32
    offset = (h&0xffffUL)*num_bits;
    addr = ((unsigned int((h>>16)*num_bits))<<11) + (offset>>5);
    offset &= 0x1fUL;

    if (is_empty(h))
      break;   // search unsuccessful

    // read compressed value from table
    t1 = (table[addr]<<offset | (offset==0 ? 0 : table[addr+1]>>(32-offset))) 
         & mask1;
    t2 = (table[addr+1]<<offset | (offset==0 ? 0 : table[addr+2]>>(32-offset))) 
         & mask2;

    if (t1==c1 ? t2 < c2 : t1 < c1)
      break;    // search unsuccessful
    
    if (t1==c1 && t2==c2)
      return TRUE;   // search successful

    h = (h+h2) % table_size;
    num_collisions++;
    probe++;
    if (probe==table_size)
      Error.Notrace("Closed hash table full.");
  } while (TRUE);

  // write trace info
  if (args->trace_file.value)
    TraceFile->write(c1, c2, in->previous.lVal());

  // insertion - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (num_elts==table_size)
    Error.Notrace("Closed hash table full.");

  while ( !is_empty(h) )   // until empty slot found
  {
    if (t1==c1 ? t2 < c2 : t1 < c1) {
      table[addr]   ^= (c1^t1)>>offset;
      table[addr+1] ^= (offset==0 ? 0 : (unsigned int(c1^t1))<<(32-offset))
                       | (c2^t2)>>offset;
      table[addr+2] ^= (offset==0 ? 0 : (unsigned int(c2^t2))<<(32-offset));
      c1 = t1; c2 = t2; 
    }

    h = (h + 1 + c1 % (table_size-1)) % table_size;
    offset = (h&0xffffUL)*num_bits;
    addr = ((unsigned int((h>>16)*num_bits))<<11) + (offset>>5);
    offset &= 0x1fUL;
    t1 = (table[addr]<<offset | (offset==0 ? 0 : table[addr+1]>>(32-offset)))
         & mask1;
    t2 = (table[addr+1]<<offset | (offset==0 ? 0 : table[addr+2]>>(32-offset)))
         & mask2;
  }

  table[addr]   |= c1>>offset;   // insertion
  table[addr+1] |= (offset==0 ? 0 : ((unsigned int) c1)<<(32-offset)) | c2>>offset;
  table[addr+2] |= (offset==0 ? 0 : ((unsigned int) c2)<<(32-offset));

  copy_state(in);   // make copy of state
  Full->set(h);
  num_elts++;
  if (permanent)
    num_elts_reduced++;
  return FALSE;

#endif

#endif
};

bool 
state_set::was_present( state *& in, bool valid, bool permanent )
{
  if (args->symmetry_reduction.value)
    in->Normalize();
  if (args->multiset_reduction.value
      && !args->symmetry_reduction.value)
      in->MultisetSort();
  return simple_was_present( in, valid, permanent );
}

void 
state_set::print_capacity( void )
{
  cout << "\t* The memory allocated for the hash table and state queue is\n\t  ";
  if (args->mem.value > 1000000)
    cout << (args->mem.value/1000000) << " Mbytes.\n";
  else
    cout << (args->mem.value/1000) << " kbytes.\n";

#ifndef HASHC  
  cout <<   "\t  With two words of overhead per state, the maximum size of\n"
       <<   "\t  the state space is " 
       << table_size << " states.\n"
       << "\t   * Use option \"-k\" or \"-m\" to increase this, if necessary.\n"; 
#else
  cout <<   "\t  With states hash-compressed to " 
       << args->num_bits.value << " bits, the maximum size of\n"
       <<   "\t  the state space is " 
       << table_size << " states.\n"
       << "\t   * Use option \"-k\" or \"-m\" to increase this, if necessary.\n"; 
#endif
}

/****************************************
  Modification:
  1) 1 Dec 93 Norris Ip: 
  check -sym option when checking was_present()
  add StateCmp(state l, state r)
  2) 24 Feb 94 Norris Ip:
  added -debugsym option to run two hash tables in parallel
  for debugging purpose
  3) 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
  $Log: mu_state.C,v $
  Revision 1.3  1999/01/29 08:28:09  uli
  efficiency improvements for security protocols

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
