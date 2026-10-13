/* -*- C++ -*-
 * mu_io.h
 * @(#) header of the interface routines
 *      for the driver for Murphi verifiers.
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
  There are 3 groups of declarations:
  1) Error_handler
  2) argclass
  3) general printing routine (not belong to any class)
  4) trace info file
 ****************************************/

/****************************************
  Error handler
 ****************************************/

class Error_handler
{
  char buffer[BUFFER_SIZE]; // for vsprintf'ing error messages prior to cout'ing them.

  int num_errors;
  int phase;
  int oldphase;
  bool has_error; 
  int num_error_curstate;
  int phase_2_done;

public:
  Error_handler()
  : num_errors(0), phase(1) {};
  ~Error_handler() {};

  void StartCountingCurstate() { num_error_curstate = 0; }
  int ErrorNumCurstate() { return num_error_curstate; }
  bool Phase2Done() { return phase_2_done; }

  void SpecialPhase() { oldphase = phase; phase = 3; };
  void NormalPhase() { phase = oldphase; };

  void ResetErrorFlag() { phase = 3; has_error = FALSE; }
  bool NoError() { return !has_error; };

  int NumError() { return num_errors; };

  void Error( const char *fmt, ... );       /* called like printf. */
  void Deadlocked( const char *fmt, ... ); /* When we\'re not in a rule.
					       currently only in deadlock */
  void Notrace( const char *fmt, ... );     /* Doesn\'t print a trace. */
};

/****************************************
  iterator for argument class
 ****************************************/

/* abstract class for mapping over a list of strings. */
class string_iterator
{
  /* restrictions:
   * Also, you can\'t have more than one of these going at a time. */
public:
  virtual char *value() = 0;
  virtual char *nextvalue() = 0;
  virtual string_iterator& next() =0;
  virtual bool done() =0;
  virtual void start() =0;
};

class arg_iterator: public string_iterator
{
  int argc;
  char **argv;
  int index;
public:
  arg_iterator (int argc, char **argv)
  :argc(argc), argv(argv), index(1) {}; /* index(1) to skip the program name. */
  virtual char *value() { return argv[ index ]; };
  virtual char *nextvalue() { if (index+1>=argc) return ""; else return argv[ index+1 ]; };
  virtual string_iterator& next() { index++; return *this; }
  virtual bool done() { return (index >= argc); }
  virtual void start() { };
};

class strtok_iterator: public string_iterator
/* uses strtok() to break up into strings. */
{
  char *old;
  char *current;
public:
  strtok_iterator (char *s)
  :old(s), current(NULL) { start(); };
  virtual char *value() { return current; };
  virtual string_iterator& next()
  { current = strtok(NULL," "); return *this; }
  virtual bool done() { return (current == NULL); }
  virtual void start()
  { if (old != NULL) current = strtok(tsprintf("%s",old), " "); };
  /* we can\'t count on strdup() being there, unfortunately. */
};

/****************************************
  argument class
 ****************************************/
class argmain_alg
{
public:
  enum MainAlgorithmtype { Nothing, Simulate, Verify_bfs, Verify_dfs };
  MainAlgorithmtype mode; /* What to do. */
private:
  bool initialized;
  char * name;
public:
  argmain_alg(MainAlgorithmtype t, char *n) : mode(t), initialized(FALSE), name(n) {};
  ~argmain_alg() {};
  void set(MainAlgorithmtype t) 
  { if (!initialized) 
      { initialized = TRUE; mode = t; }
    else if (mode != t)
      Error.Notrace("Conflicting options to %s.", name);
  };
};

class argsym_alg
{
public:
  enum SymAlgorithmType {  Exhaustive_Fast_Canonicalize, 
			   Heuristic_Fast_Canonicalize,
			   Heuristic_Small_Mem_Canonicalize,
			   Heuristic_Fast_Normalize
			 };
  SymAlgorithmType mode; /* What to do. */
private:
  bool initialized;
  char * name;
public:
  argsym_alg(SymAlgorithmType t, char *n) : mode(t), initialized(FALSE), name(n) {};
  ~argsym_alg() {};
  void set(SymAlgorithmType t) 
  { if (!initialized) 
      { initialized = TRUE; mode = t; }
    else if (mode != t)
      Error.Notrace("Conflicting options to %s.", name);
  };
};

class argnum
{
public:
  unsigned long value;
private:
  bool initialized;
  char * name;
public:
  argnum(unsigned long val, char * n) : value(val), initialized(FALSE), name(n) {};
  ~argnum() {};
  void set(unsigned long val) 
  { if (!initialized) 
      { initialized = TRUE; value = val; }
    else if (val != value)
      Error.Notrace("Conflicting options to %s.", name);
  };
};

class argbool
{
public:
  bool value;
private:
  bool initialized;
  char * name;
public:
  argbool(bool val, char *n) : value(val), initialized(FALSE), name(n) {};
  ~argbool() {};
  void reset(bool val) 
  { initialized = TRUE; value = val; }
  void set(bool val) 
  { if (!initialized) 
      { initialized = TRUE; value = val; }
    else if (val != value)
      Error.Notrace("Conflicting options to %s.", name);
  };
};

/* Argclass inspired by Andreas\' code. */
class argclass
{
  int argc;
  char **argv;
public:
  
  // trace options
  argbool print_trace;
  argbool full_trace;
  argbool trace_all;
  argbool find_errors;
  argnum  max_errors;
  
  // memory options
  argnum mem;
  
  // progress report options
  argnum progress_count;
  argbool print_progress;
  
  // main algorithm options
  argmain_alg main_alg;

  // symmetry option
  argbool symmetry_reduction;
  argbool multiset_reduction;
  argsym_alg sym_alg;
  argnum perm_limit;
  argbool debug_sym;

  // Uli: hash compaction options
#ifdef HASHC
  argnum num_bits;
  argbool trace_file;
#endif

  // testing parameter
  argnum test_parameter1;
  argnum test_parameter2;

  // miscelleneous
  argnum  loopmax;
  argbool verbose;
  argbool no_deadlock;
  argbool print_options;
  argbool print_license;
  argbool print_rule;
  argbool print_hash;
  
  // supporting routines
  argclass(int ac, char** av);
  ~argclass() {};
  void ProcessOptions(string_iterator *options);
  bool Flag(char *arg);
  void PrintInfo( void );
  void PrintOptions( void );
  void PrintLicense( void );

};

/****************************************
  Printing functions.
 ****************************************/

class ReportManager
{
  void print_trace_aux(StatePtr p);   // changed by Uli
public:
  ReportManager();
  void CheckConsistentVersion();
  void StartSimulation();

  void print_algorithm();
  void print_warning();
  void print_header( void );
  void print_trace_with_theworld();
  void print_trace_with_curstate();
  void print_progress( void );
  void print_no_error( void );
  void print_summary(bool);   // print omission probabilities only if true
  void print_curstate( void );
  void print_dfs_deadlock( void );
  void print_retrack( void );
  void print_fire_startstate();
  void print_fire_rule();
  void print_fire_rule_diff(state * s);
  void print_trace_all();
  void print_verbose_header();
  void print_hashtable();
  void print_final_report();
};

/****************************************   // added by Uli
  trace info file.
 ****************************************/

#ifdef HASHC
class TraceFileManager
{
public:
  struct Buffer {            // buffer for read
    unsigned long previous;
    unsigned long c1;
    unsigned long c2;
  };

private:
  int numBytes;           // number of bytes for compressed values
  char name[256];         // filename for trace info file
  FILE* fp;               // file pointer
  Buffer buf;             // buffer for read
  unsigned long inBuf;    // number of state in buffer (0: empty)
  unsigned long last;     // number of last state written
  void writeLong(unsigned long l, int bytes);
  unsigned long readLong(int bytes);

public:
  TraceFileManager(char*);
  ~TraceFileManager();
  void setBytes(int bits);
  unsigned long numLast();
  void write(unsigned long c1, unsigned long c2, unsigned long previous);
  const Buffer* read(unsigned long number);
};
#endif


/****************************************
  1) 1 Dec 93 Norris Ip: 
  add symmetry_option field in IO to select symmetry reduction 
  add test parameter input for testing
  2) 8 Feb 94 Norris Ip:
  add print hashtable for debugging
  3) 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 14 April 94 Norris Ip:
  add nextvalue() to arg_iterator
  * 19 Oct 94 Norris Ip:
  change to object oriented
****************************************/

/********************
  $Log: mu_io.h,v $
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
