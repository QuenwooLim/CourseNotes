/* -*- C++ -*-
 * symtab.C
 * @(#) implementation for Murphi\'s symbol table.
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
 * 
 * Update:
 *
 * None
 *
 */ 

#include "mu.h"

// #define DEBUG_SYMTAB

/********************
  class ste
  ********************/
ste::ste(lexid *name, int scope, decl *value)
: name(name), scope(scope), value(value), next(NULL)
{
  /* a weirdness--we tell an object its name when we declare it. */
  if (name != NULL)
    value->name = name->getname();  /* loses a previous name for value. */
  if (value->getclass() != decl::Type)
    value->mu_name = tsprintf("mu_%s",value->name);
  else if (value!=booltype)
    value->mu_name = tsprintf("mu_%d_%s",scope,value->name);
}

ste *ste::search(lexid *name)
{
  ste *s = this;
  while (s != NULL && s->name != name)
    s = s->next;
  return s;
}

ste *ste::reverse()
{
  ste *in = this, *out = NULL, *temp = NULL;
  while (in != NULL)
    {
      temp = in;
      in = in->next;
      temp->next = out;
      out = temp;
    }
  return out;
}
  
/********************
  class symboltable
  ********************/
 symboltable::symboltable()
{
  constdecl *temp;
  int i = 0;
  for (i=0; i<MAXSCOPES; i++)
    {
      scopes[i] = 0;
      scopestack[i] = NULL;
    }
  curscope = 0;
  scopedepth = 0;
  
  /* enter useful stuff. */
  declare(ltable.enter("boolean"), booltype); 
  temp = new constdecl(1,booltype);
  declare(ltable.enter("true"), temp );
  temp = new constdecl(0,booltype);
  declare(ltable.enter("false"), temp );
  ::offset = 0;
}

ste *symboltable::find(lexid *name) const
{
  ste *p = topste();
  while ( p != NULL && p->name != name ) p = p->next;
  return (p);
}

ste *symboltable::lookup(lexid *name)
{
  ste *p = find(name);
  if ( p == NULL )
    {
      Error.Error("%s undeclared.", name->getname() );
      p = declare(name, errordecl);
    }
  return (p);
}

ste *symboltable::declare(lexid *name, decl *value)
{
  ste *p = find(name);
#ifdef DEBUG_SYMTAB
  printf("Declaring %s in scope %d, depth %d\n",
	 name->getname(),
	 scopes[scopedepth],
	 scopedepth);
#endif
  if ( p != NULL && p->getscope() == curscope )
    {
      Error.Error ("%s already defined in current scope.",name->getname() );
      return (p);
    }
  /* p is NULL here; no problem with reallocating it. */
  p = new ste(name, scopes[scopedepth], value);

  /* straightforward insertions. */
  p->next = topste();
  scopestack[scopedepth] = p;
  return (p);
}

ste *symboltable::declare_global(lexid *name, decl *value)
{
  ste *p = find(name); 
  // ste **pp = NULL;
#ifdef DEBUG_SYMTAB
  printf("Declaring %s globally in scope %d, depth %d\n",
	 name->getname(),
	 scopes[globalscope],
	 globalscope);
#endif
  if ( p != NULL && p->getscope() == globalscope )
    {
      Error.Error ("%s already defined in global scope.",name->getname() );
      return (p);
    }
  /* p is NULL here; no problem with reallocating it. */
  p = new ste(name, scopes[ globalscope ], value);

  /* not-quite-so-straightforward insertions. */
  // then insert it into the list.
  p->next = scopestack[ globalscope ];
  ste *oldtop = scopestack[ globalscope ];
  scopestack[ globalscope ] = p;

  // splice in the new ste into the list.  Ooh, this is going to be a pain.
  int i = globalscope + 1;
  ste *q = NULL;
  while ( scopestack[i] == oldtop )
    {
      scopestack[i] = p;
      i++;
    }
  if (i <= scopedepth )
    {
      q = scopestack[i];
      while ( q->next->scope == q->scope)
	{
	  q = q->next;
	}
      q->next = p;
    }
  return (p);
}

int symboltable::pushscope()
{
  scopedepth++;
  curscope++;
#ifdef TEST
  fprintf(stderr,"Pushing scope %d\n",curscope);
#endif
  scopes[scopedepth] = curscope;
  // class of the group of variables to be declared -- for dependency analysis
  scopestack[ scopedepth ] = scopestack[ scopedepth - 1 ];
  offsets[scopedepth - 1] = offset;
  offset = 0;
  return curscope;
}

ste *symboltable::popscope(bool cut )
/* take the ste's out of the hash table, but leave them in the linked list,
 * unless cut is set.*/
{
  ste **p;
#ifdef TEST
  fprintf(stderr,"Popping scope %d\n",scopes[scopedepth] );
#endif
  if ( scopedepth <= 0 )
    {
      Error.Error("Internal: Popped too many scopes. Giving up; sorry.");
      exit(1);
    }
  for (p = &scopestack[scopedepth]; ; p = &(*p)->next )
    /* Invariant: *p is the first element in the list for its bucket. */
    {
      if ( (*p)->scope != scopes[scopedepth] )
	{
	  if ( cut ) *p = NULL;
	  break;
	}
    };
  offset = offsets[ scopedepth - 1];
  return scopestack[ scopedepth-- ];
}

ste *symboltable::getscope() const
{
  if ( scopestack[scopedepth]->scope != scopes[scopedepth] )
    return NULL;
  else return scopestack[scopedepth];
}

ste *symboltable::dupscope() const
{
  ste *s = getscope();
  ste *beginning = NULL, **end = &beginning;
  for (;
       s != NULL && s->scope == scopes[scopedepth];
       s = s->next)
    {
      *end = new ste(s->name, s->scope, s->value);
      end = &(*end)->next;
    }
  return beginning;
}
  
const int symboltable::globalscope = 1;

/****************************************
  * 8 March 94 Norris Ip:
  merge with the latest rel2.6
  * 17 March 94 Norris Ip:
  fixed ruleset boolean segmentation fault
****************************************/

/********************
 $Log: symtab.C,v $
 Revision 1.2  1999/01/29 07:49:13  uli
 bugfixes

 Revision 1.4  1996/08/07 18:54:00  ip
 last bug fix on NextRule/SetNextEnabledRule has a bug; fixed this turn

 Revision 1.3  1996/08/07 00:59:13  ip
 Fixed bug on what_rule setting during guard evaluation; otherwise, bad diagnoistic message on undefine error on guard

 Revision 1.2  1996/08/06 23:57:39  ip
 fixed while code generation bug

 Revision 1.1  1996/08/06 23:56:55  ip
 Initial revision

 ********************/
