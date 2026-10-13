/* -*- C++ -*- 
 * mu_state.h
 * @(#) header for routines related to states in the verifier
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
 * Original Author: Ralph Melton
 * Extracted from mu_epilog.inc and mu_prolog.inc
 * by C. Norris Ip
 * Created: 21 April 93
 *
 * Update:
 *
 */ 

#ifndef _STATE_
#define _STATE_

/****************************************
  There are three different declarations:
  1) state
  2) dynBitVec
  3) state queue
  4) state set
 ****************************************/

/****************************************
  The record for a single state.
  require : BITS_IN_WORLD in parameter file
 ****************************************/

/* BITS_IN_WORLD gets defined by the generated code. */
/* The extra addition is there so that we round up to the greater block. */

/****************************************
  Bit vector - copied straight from Andreas. 
 ****************************************/
class dynBitVec
{
  // data
  unsigned long numBits;
  unsigned char* v;
  
  // Inquiries
  inline unsigned int Index( unsigned long index ) { return index / 8; }
  inline unsigned int Shift( unsigned long index ) { return index % 8; }
  
public:
  // initializer
  dynBitVec( unsigned long nBits );
  // destructor
  virtual ~dynBitVec();
  
  // interface
  inline int NumBits( void ) { return numBits; }
  inline int NumBytes( void ) { return 1 + (numBits - 1) / 8; }
  inline void clear( unsigned long i ) { v[ Index(i) ] &= ~(1 << Shift(i)); }
  inline void set( unsigned long i ) { v[ Index(i) ] |=  (1 << Shift(i)); }
  inline int get( unsigned long i ) { return (v[ Index(i) ] >> Shift(i)) & 1; }
};

class statelist
{
  state * s;
  statelist * next;
public:
  statelist(state * s, statelist * next) 
  : s(s), next(next) {};
};

/****************************************
  The state queue.
 ****************************************/
class state_queue
{
protected:
  state** stateArray;                     /* The actual array. */
  const unsigned long max_active_states;  /* max size of queue */
  unsigned long front;                    /* index of first active state. */
  unsigned long rear;                     /* index of next free slot. */
  unsigned long num_elts;                  /* number of elements. */
  
public:
  // initializers
  state_queue( unsigned long mas );

  // destructor
  virtual ~state_queue();
  
  // information interface
  inline unsigned long MaxElts( void ) { return max_active_states; }
  unsigned long NumElts( void ) { return num_elts; }
  inline static int BytesForOneState( void ); 
  inline bool isempty( void ) { return num_elts == 0; }
  
  // storing and removing elements
  virtual void enqueue( state* e );
  virtual state* dequeue( void );
  virtual state * top( void );
  
  virtual unsigned NextRuleToTry()   // Uli: unsigned short -> unsigned
  {
    Error.Notrace("Internal: Getting next rule to try from a state queue instead of a state stack.");
    return 0;
  }
  virtual void NextRuleToTry(unsigned r)
  {
    Error.Notrace("Internal: Setting next rule to try from a state queue instead of a state stack.");
  }

  // printing routine
  void Print( void );
  virtual void print_capacity( void )
  {
    cout << "\t* Capacity in queue for breadth-first search: "
	 << max_active_states << " states.\n"
	 << "\t   * Change the constant gPercentActiveStates in mu_prolog.inc\n"
         << "\t     to increase this, if necessary.\n"; 
  }
};

class state_stack: public state_queue
{
  unsigned * nextrule_to_try;

public:
  // initializers
  state_stack( unsigned long mas )
  : state_queue(mas)
  {
    unsigned long i;
    nextrule_to_try = new unsigned [ mas ];
    for ( i = 0; i < mas; i++)
      nextrule_to_try[i] = 0;
  };

  // destructor
  virtual ~state_stack()
  {
    delete[ OLD_GPP(max_active_states) ] nextrule_to_try; // Should be delete[].
  };

  virtual void print_capacity( void )
  {
    cout << "\t* Capacity in queue for depth-first search: "
	 << max_active_states << " states.\n" 
         << "\t   * Change the constant gPercentActiveStates in mu_prolog.inc\n"
         << "\t     to increase this, if necessary.\n";   
  }
  virtual void enqueue( state* e );

  virtual unsigned NextRuleToTry()
  {
    return nextrule_to_try[ front ];
  }
  virtual void NextRuleToTry(unsigned r)
  {
    nextrule_to_try[ front ] = r;
  }
  
#ifdef partial_order_opt
  // special interface with sleepset
  virtual void enqueue( state *e, sleepset s );
#endif
};

/****************************************
  The state set
  represented as a large open-addressed hash table.
 ****************************************/

class state_set
{
#ifdef HASHC
  typedef unsigned long Unsigned32;    // basic building block of the hash 
                                       // table, slots may have different size
#endif

  // data
  unsigned long table_size;            /* max size of the hash table */
#ifndef HASHC
  state *table;                        /* pointer to the hash table */
#else
  Unsigned32 *table;
#endif
  dynBitVec *Full;                     /* whether element table[i] is used. */
  unsigned long num_elts;              /* number of elements in table */
  unsigned long num_elts_reduced;   // Uli
  unsigned long num_collisions;        /* number of collisions in hashing */ 

  // internal routines
  bool is_empty( unsigned long i )     /* check if element table[i] is empty */
  { return Full->get(i) == 0; };

public:
  // constructors
  state_set ( unsigned long table_size );
  state_set ( void );

  friend void copy_state_set( state_set * set1, state_set * set2);

  void clear_state_set();

  // destructor
  virtual ~state_set();

  // checking the presence of state "in"
  bool simple_was_present( state *&in, bool, bool );  
    /* old was_present without checking -sym */
  bool was_present( state *&in, bool, bool );
    /* checking -sym before calling simple_was_present() */
  
  // get the size of each state entry
#ifndef VER_PSEUDO
  static int bits_per_state(void);
#endif
  
  // get the number of elts in the state set
  inline unsigned long NumElts() { return num_elts; };

  inline unsigned long NumEltsReduced() { return num_elts_reduced; };   // Uli
  
  // printing information
  void print_capacity( void );

  // print hashtable       
  void print()
  {
    for (unsigned long i=0; i<table_size; i++)
      if (!is_empty(i))
	{
	  cout << "State " << i << "\n";
#ifdef HASHC
	  cout << "... compressed\n";
#else
	  StateCopy(workingstate,&table[i]);
	  theworld.print();
#endif
	  cout << "\n";
	}
  }
};

/****************************************
  1) 1 Dec 93 Norris Ip: 
  check -sym option when checking was_present()
  add Normalize() declaration in class state
  add friend StateCmp to class state
  2) 24 Feb 94 Norris Ip:
  added -debugsym option to run two hash tables in parallel
  for debugging purpose
  3) 8 March 94 Norris Ip:
  merge with the latest rel2.6
****************************************/

/********************
  $Log: mu_state.h,v $
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

#endif
