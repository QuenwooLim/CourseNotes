/******************************
  Program "adash.m" compiled by "Murphi Release 3.1"

  Murphi Last Modefied Date: "Jan 29 1999"
  Murphi Last Compiled date: "Feb 17 2011"
 ******************************/

/********************
  Parameter
 ********************/
#define MURPHI_VERSION "Murphi Release 3.1"
#define MURPHI_DATE "Jan 29 1999"
#define PROTOCOL_NAME "adash"
#define BITS_IN_WORLD 4192
#define ALIGN

/********************
  Include
 ********************/
#include "mu_prolog.inc"

/********************
  Decl declaration
 ********************/

class mu_1_Home: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_Home& val){ return value(val.value());};
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_Home& val)
    {
      if (val.defined())
        return ( s << mu_1_Home::values[ int(val) - 1 ] );
      else
        return ( s << "Undefined" );
    };

  mu_1_Home (char *name, int os): mu__byte(1, 1, 1, name, os) {};
  mu_1_Home (void): mu__byte(1, 1, 1) {};
  mu_1_Home (int val): mu__byte(1, 1, 1, "Parameter or function result.", 0)
    { operator=(val); };
  char * Name() { return values[ value() -1]; };
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ value() - 1] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
friend int CompareWeight(mu_1_Home& a, mu_1_Home& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
};
char *mu_1_Home::values[] =
  { "Home_1",NULL };

/*** end scalarset declaration ***/
mu_1_Home mu_1_Home_undefined_var;

class mu_1_Remote: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_Remote& val){ return value(val.value());};
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_Remote& val)
    {
      if (val.defined())
        return ( s << mu_1_Remote::values[ int(val) - 2 ] );
      else
        return ( s << "Undefined" );
    };

  mu_1_Remote (char *name, int os): mu__byte(2, 3, 2, name, os) {};
  mu_1_Remote (void): mu__byte(2, 3, 2) {};
  mu_1_Remote (int val): mu__byte(2, 3, 2, "Parameter or function result.", 0)
    { operator=(val); };
  char * Name() { return values[ value() -2]; };
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ value() - 2] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
friend int CompareWeight(mu_1_Remote& a, mu_1_Remote& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
};
char *mu_1_Remote::values[] =
  { "Remote_1","Remote_2",NULL };

/*** end scalarset declaration ***/
mu_1_Remote mu_1_Remote_undefined_var;

class mu_1_Proc: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_Proc& val) { return value(val.value()); };
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_Proc& val)
    {
      if (val.defined())
        return ( s << mu_1_Proc::values[ val.indexvalue() ] );
      else
        return ( s << "Undefined" );
    };

  // note thate lb and ub are not used if we have byte compacted state.
  mu_1_Proc (char *name, int os): mu__byte(0, 2, 2, name, os) {};
  mu_1_Proc (void): mu__byte(0, 2, 2) {};
  mu_1_Proc (int val): mu__byte(0, 2, 2, "Parameter or function result.", 0)
    { operator=(val); };
  int indexvalue()
  {
    if ((value() >= 1) && (value() <= 1)) return (value() - 1);
    if ((value() >= 2) && (value() <= 3)) return (value() - 1);
  };
  inline int unionassign(int val)
  {
    if (val >= 0 && val <= 0) return value(val+1);
    if (val >= 1 && val <= 2) return value(val+1);
  };
  char * Name() { return values[ indexvalue() ]; };
friend int CompareWeight(mu_1_Proc& a, mu_1_Proc& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ indexvalue() ] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
};
char *mu_1_Proc::values[] = {"Home_1","Remote_1","Remote_2",NULL };

/*** end union declaration ***/
mu_1_Proc mu_1_Proc_undefined_var;

class mu_1_DirIndex: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1_DirIndex& val) { return mu__byte::operator=((int) val); };
  mu_1_DirIndex (char *name, int os): mu__byte(0, 1, 2, name, os) {};
  mu_1_DirIndex (void): mu__byte(0, 1, 2) {};
  mu_1_DirIndex (int val): mu__byte(0, 1, 2, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1_DirIndex mu_1_DirIndex_undefined_var;

class mu_1_NodeCount: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1_NodeCount& val) { return mu__byte::operator=((int) val); };
  mu_1_NodeCount (char *name, int os): mu__byte(0, 2, 2, name, os) {};
  mu_1_NodeCount (void): mu__byte(0, 2, 2) {};
  mu_1_NodeCount (int val): mu__byte(0, 2, 2, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1_NodeCount mu_1_NodeCount_undefined_var;

class mu_1_Address: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_Address& val){ return value(val.value());};
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_Address& val)
    {
      if (val.defined())
        return ( s << mu_1_Address::values[ int(val) - 4 ] );
      else
        return ( s << "Undefined" );
    };

  mu_1_Address (char *name, int os): mu__byte(4, 4, 1, name, os) {};
  mu_1_Address (void): mu__byte(4, 4, 1) {};
  mu_1_Address (int val): mu__byte(4, 4, 1, "Parameter or function result.", 0)
    { operator=(val); };
  char * Name() { return values[ value() -4]; };
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ value() - 4] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
friend int CompareWeight(mu_1_Address& a, mu_1_Address& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
};
char *mu_1_Address::values[] =
  { "Address_1",NULL };

/*** end scalarset declaration ***/
mu_1_Address mu_1_Address_undefined_var;

class mu_1_Value: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_Value& val){ return value(val.value());};
  inline operator int() const { return value(); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_Value& val)
    {
      if (val.defined())
        return ( s << mu_1_Value::values[ int(val) - 5 ] );
      else
        return ( s << "Undefined" );
    };

  mu_1_Value (char *name, int os): mu__byte(5, 6, 2, name, os) {};
  mu_1_Value (void): mu__byte(5, 6, 2) {};
  mu_1_Value (int val): mu__byte(5, 6, 2, "Parameter or function result.", 0)
    { operator=(val); };
  char * Name() { return values[ value() -5]; };
  virtual void print()
    {
      if (defined()) cout << name << ':' << values[ value() - 5] << '\n';
      else cout << name << ":Undefined\n";
    };
  void print_statistic() {};
friend int CompareWeight(mu_1_Value& a, mu_1_Value& b)
{
  if (!a.defined() && b.defined())
    return -1;
  else if (a.defined() && !b.defined())
    return 1;
  else
    return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
};
char *mu_1_Value::values[] =
  { "Value_1","Value_2",NULL };

/*** end scalarset declaration ***/
mu_1_Value mu_1_Value_undefined_var;

class mu_1_RequestType: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_RequestType& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_RequestType& val)
  {
    if (val.defined())
      return ( s << mu_1_RequestType::values[ int(val) - 7] );
    else return ( s << "Undefined" );
  };

  mu_1_RequestType (char *name, int os): mu__byte(7, 19, 4, name, os) {};
  mu_1_RequestType (void): mu__byte(7, 19, 4) {};
  mu_1_RequestType (int val): mu__byte(7, 19, 4, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -7]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -7] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_RequestType::values[] = {"RD_H","RD_RAC","RDX_H","RDX_RAC","INV","WB","SHWB","DXFER","DRD_H","DRD_RAC","DWR_H","DWR_RAC","DUP",NULL };

/*** end of enum declaration ***/
mu_1_RequestType mu_1_RequestType_undefined_var;

class mu_1_ReplyType: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_ReplyType& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_ReplyType& val)
  {
    if (val.defined())
      return ( s << mu_1_ReplyType::values[ int(val) - 20] );
    else return ( s << "Undefined" );
  };

  mu_1_ReplyType (char *name, int os): mu__byte(20, 23, 3, name, os) {};
  mu_1_ReplyType (void): mu__byte(20, 23, 3) {};
  mu_1_ReplyType (int val): mu__byte(20, 23, 3, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -20]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -20] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_ReplyType::values[] = {"ACK","NAK","IACK","SACK",NULL };

/*** end of enum declaration ***/
mu_1_ReplyType mu_1_ReplyType_undefined_var;

class mu_1_Request
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_RequestType mu_Mtype;
  mu_1_Proc mu_Aux;
  mu_1_Address mu_Addr;
  mu_1_Value mu_Value;
  mu_1_Request ( char *n, int os ) { set_self(n,os); };
  mu_1_Request ( void ) {};

  virtual ~mu_1_Request(); 
friend int CompareWeight(mu_1_Request& a, mu_1_Request& b)
  {
    int w;
    w = CompareWeight(a.mu_Mtype, b.mu_Mtype);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Aux, b.mu_Aux);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Addr, b.mu_Addr);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_Request& a, mu_1_Request& b)
  {
    int w;
    w = Compare(a.mu_Mtype, b.mu_Mtype);
    if (w!=0) return w;
    w = Compare(a.mu_Aux, b.mu_Aux);
    if (w!=0) return w;
    w = Compare(a.mu_Addr, b.mu_Addr);
    if (w!=0) return w;
    w = Compare(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Mtype.MultisetSort();
    mu_Aux.MultisetSort();
    mu_Addr.MultisetSort();
    mu_Value.MultisetSort();
  }
  void print_statistic()
  {
    mu_Mtype.print_statistic();
    mu_Aux.print_statistic();
    mu_Addr.print_statistic();
    mu_Value.print_statistic();
  }
  void clear() {
    mu_Mtype.clear();
    mu_Aux.clear();
    mu_Addr.clear();
    mu_Value.clear();
 };
  void undefine() {
    mu_Mtype.undefine();
    mu_Aux.undefine();
    mu_Addr.undefine();
    mu_Value.undefine();
 };
  void reset() {
    mu_Mtype.reset();
    mu_Aux.reset();
    mu_Addr.reset();
    mu_Value.reset();
 };
  void print() {
    mu_Mtype.print();
    mu_Aux.print();
    mu_Addr.print();
    mu_Value.print();
  };
  void print_diff(state *prevstate) {
    mu_Mtype.print_diff(prevstate);
    mu_Aux.print_diff(prevstate);
    mu_Addr.print_diff(prevstate);
    mu_Value.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Mtype.to_state(thestate);
    mu_Aux.to_state(thestate);
    mu_Addr.to_state(thestate);
    mu_Value.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_Request& operator= (const mu_1_Request& from) {
    mu_Mtype.value(from.mu_Mtype.value());
    mu_Aux.value(from.mu_Aux.value());
    mu_Addr.value(from.mu_Addr.value());
    mu_Value.value(from.mu_Value.value());
    return *this;
  };
};

  void mu_1_Request::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_Request::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_Request::set_self(char *n, int os)
{
  name = n;
  mu_Mtype.set_self_2(name, ".Mtype", os + 0 );
  mu_Aux.set_self_2(name, ".Aux", os + 8 );
  mu_Addr.set_self_2(name, ".Addr", os + 16 );
  mu_Value.set_self_2(name, ".Value", os + 24 );
}

mu_1_Request::~mu_1_Request()
{
}

/*** end record declaration ***/
mu_1_Request mu_1_Request_undefined_var;

class mu_1_Reply
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_ReplyType mu_Mtype;
  mu_1_Proc mu_Aux;
  mu_1_Address mu_Addr;
  mu_1_NodeCount mu_InvCount;
  mu_1_Value mu_Value;
  mu_1_Reply ( char *n, int os ) { set_self(n,os); };
  mu_1_Reply ( void ) {};

  virtual ~mu_1_Reply(); 
friend int CompareWeight(mu_1_Reply& a, mu_1_Reply& b)
  {
    int w;
    w = CompareWeight(a.mu_Mtype, b.mu_Mtype);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Aux, b.mu_Aux);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Addr, b.mu_Addr);
    if (w!=0) return w;
    w = CompareWeight(a.mu_InvCount, b.mu_InvCount);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_Reply& a, mu_1_Reply& b)
  {
    int w;
    w = Compare(a.mu_Mtype, b.mu_Mtype);
    if (w!=0) return w;
    w = Compare(a.mu_Aux, b.mu_Aux);
    if (w!=0) return w;
    w = Compare(a.mu_Addr, b.mu_Addr);
    if (w!=0) return w;
    w = Compare(a.mu_InvCount, b.mu_InvCount);
    if (w!=0) return w;
    w = Compare(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Mtype.MultisetSort();
    mu_Aux.MultisetSort();
    mu_Addr.MultisetSort();
    mu_InvCount.MultisetSort();
    mu_Value.MultisetSort();
  }
  void print_statistic()
  {
    mu_Mtype.print_statistic();
    mu_Aux.print_statistic();
    mu_Addr.print_statistic();
    mu_InvCount.print_statistic();
    mu_Value.print_statistic();
  }
  void clear() {
    mu_Mtype.clear();
    mu_Aux.clear();
    mu_Addr.clear();
    mu_InvCount.clear();
    mu_Value.clear();
 };
  void undefine() {
    mu_Mtype.undefine();
    mu_Aux.undefine();
    mu_Addr.undefine();
    mu_InvCount.undefine();
    mu_Value.undefine();
 };
  void reset() {
    mu_Mtype.reset();
    mu_Aux.reset();
    mu_Addr.reset();
    mu_InvCount.reset();
    mu_Value.reset();
 };
  void print() {
    mu_Mtype.print();
    mu_Aux.print();
    mu_Addr.print();
    mu_InvCount.print();
    mu_Value.print();
  };
  void print_diff(state *prevstate) {
    mu_Mtype.print_diff(prevstate);
    mu_Aux.print_diff(prevstate);
    mu_Addr.print_diff(prevstate);
    mu_InvCount.print_diff(prevstate);
    mu_Value.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Mtype.to_state(thestate);
    mu_Aux.to_state(thestate);
    mu_Addr.to_state(thestate);
    mu_InvCount.to_state(thestate);
    mu_Value.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_Reply& operator= (const mu_1_Reply& from) {
    mu_Mtype.value(from.mu_Mtype.value());
    mu_Aux.value(from.mu_Aux.value());
    mu_Addr.value(from.mu_Addr.value());
    mu_InvCount.value(from.mu_InvCount.value());
    mu_Value.value(from.mu_Value.value());
    return *this;
  };
};

  void mu_1_Reply::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_Reply::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_Reply::set_self(char *n, int os)
{
  name = n;
  mu_Mtype.set_self_2(name, ".Mtype", os + 0 );
  mu_Aux.set_self_2(name, ".Aux", os + 8 );
  mu_Addr.set_self_2(name, ".Addr", os + 16 );
  mu_InvCount.set_self_2(name, ".InvCount", os + 24 );
  mu_Value.set_self_2(name, ".Value", os + 32 );
}

mu_1_Reply::~mu_1_Reply()
{
}

/*** end record declaration ***/
mu_1_Reply mu_1_Reply_undefined_var;

class mu_1_RAC_State: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_RAC_State& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_RAC_State& val)
  {
    if (val.defined())
      return ( s << mu_1_RAC_State::values[ int(val) - 24] );
    else return ( s << "Undefined" );
  };

  mu_1_RAC_State (char *name, int os): mu__byte(24, 32, 4, name, os) {};
  mu_1_RAC_State (void): mu__byte(24, 32, 4) {};
  mu_1_RAC_State (int val): mu__byte(24, 32, 4, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -24]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -24] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_RAC_State::values[] = {"INVAL","WRD","WRDO","WRDX","WINV","RRD","WDMAR","WUP","WDMAW",NULL };

/*** end of enum declaration ***/
mu_1_RAC_State mu_1_RAC_State_undefined_var;

class mu_1_CacheState: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1_CacheState& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1_CacheState& val)
  {
    if (val.defined())
      return ( s << mu_1_CacheState::values[ int(val) - 33] );
    else return ( s << "Undefined" );
  };

  mu_1_CacheState (char *name, int os): mu__byte(33, 35, 2, name, os) {};
  mu_1_CacheState (void): mu__byte(33, 35, 2) {};
  mu_1_CacheState (int val): mu__byte(33, 35, 2, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -33]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -33] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1_CacheState::values[] = {"Non_Locally_Cached","Locally_Shared","Locally_Exmod",NULL };

/*** end of enum declaration ***/
mu_1_CacheState mu_1_CacheState_undefined_var;

class mu_1__type_0
{
 public:
  mu_1_Value array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_0 (char *n, int os) { set_self(n, os); };
  mu_1__type_0 ( void ) {};
  virtual ~mu_1__type_0 ();
  mu_1_Value& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 4 ) && ( index <= 4 ) )
      return array[ index - 4 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Address is internally represented from 4 to 4.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 4 ];
#endif
  };
  mu_1__type_0& operator= (const mu_1__type_0& from)
  {
      array[0].value(from.array[0].value());
    return *this;
  }

friend int CompareWeight(mu_1__type_0& a, mu_1__type_0& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_0& a, mu_1__type_0& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_0::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_0::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_0::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Address_1", i * 8 + os);i++;
}
mu_1__type_0::~mu_1__type_0()
{
}
/*** end array declaration ***/
mu_1__type_0 mu_1__type_0_undefined_var;

class mu_1__type_1: public mu__byte
{
 public:
  inline int operator=(int val) { return value(val); };
  inline int operator=(const mu_1__type_1& val) { return value(val.value()); };
  static char *values[];
  friend ostream& operator<< (ostream& s, mu_1__type_1& val)
  {
    if (val.defined())
      return ( s << mu_1__type_1::values[ int(val) - 36] );
    else return ( s << "Undefined" );
  };

  mu_1__type_1 (char *name, int os): mu__byte(36, 38, 2, name, os) {};
  mu_1__type_1 (void): mu__byte(36, 38, 2) {};
  mu_1__type_1 (int val): mu__byte(36, 38, 2, "Parameter or function result.", 0)
  {
     operator=(val);
  };
  char * Name() { return values[ value() -36]; };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
  virtual void print()
  {
    if (defined())
      cout << name << ":" << values[ value() -36] << '\n';
    else
      cout << name << ":Undefined\n";
  };
};

char *mu_1__type_1::values[] = {"Uncached","Shared_Remote","Dirty_Remote",NULL };

/*** end of enum declaration ***/
mu_1__type_1 mu_1__type_1_undefined_var;

class mu_1__type_2: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1__type_2& val) { return mu__byte::operator=((int) val); };
  mu_1__type_2 (char *name, int os): mu__byte(0, 2, 2, name, os) {};
  mu_1__type_2 (void): mu__byte(0, 2, 2) {};
  mu_1__type_2 (int val): mu__byte(0, 2, 2, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1__type_2 mu_1__type_2_undefined_var;

class mu_1__type_3
{
 public:
  mu_1_Proc array[ 2 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_3 (char *n, int os) { set_self(n, os); };
  mu_1__type_3 ( void ) {};
  virtual ~mu_1__type_3 ();
  mu_1_Proc& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 0 ) && ( index <= 1 ) )
      return array[ index - 0 ];
    else {
      if (index==UNDEFVAL) 
        Error.Error("Indexing to %s using an undefined value.", name);
      else
        Error.Error("%d not in index range of %s.", index, name);
      return array[0];
    }
#else
    return array[ index - 0 ];
#endif
  };
  mu_1__type_3& operator= (const mu_1__type_3& from)
  {
    for (int i = 0; i < 2; i++)
      array[i].value(from.array[i].value());
    return *this;
  }

friend int CompareWeight(mu_1__type_3& a, mu_1__type_3& b)
  {
    int w;
    for (int i=0; i<2; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_3& a, mu_1__type_3& b)
  {
    int w;
    for (int i=0; i<2; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<2; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<2; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 2; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 2; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 2; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 2; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 2; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 2; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_3::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_3::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_3::set_self( char *n, int os)
{
  char* s;
  name = n;
  for(int i = 0; i < 2; i++) {
    array[i].set_self_ar(n, s=tsprintf("%d",i + 0), i * 8 + os);
    delete[] s;
  }
};
mu_1__type_3::~mu_1__type_3()
{
}
/*** end array declaration ***/
mu_1__type_3 mu_1__type_3_undefined_var;

class mu_1__type_4
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1__type_1 mu_State;
  mu_1__type_2 mu_SharedCount;
  mu_1__type_3 mu_Entries;
  mu_1__type_4 ( char *n, int os ) { set_self(n,os); };
  mu_1__type_4 ( void ) {};

  virtual ~mu_1__type_4(); 
friend int CompareWeight(mu_1__type_4& a, mu_1__type_4& b)
  {
    int w;
    w = CompareWeight(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = CompareWeight(a.mu_SharedCount, b.mu_SharedCount);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Entries, b.mu_Entries);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1__type_4& a, mu_1__type_4& b)
  {
    int w;
    w = Compare(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = Compare(a.mu_SharedCount, b.mu_SharedCount);
    if (w!=0) return w;
    w = Compare(a.mu_Entries, b.mu_Entries);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_State.MultisetSort();
    mu_SharedCount.MultisetSort();
    mu_Entries.MultisetSort();
  }
  void print_statistic()
  {
    mu_State.print_statistic();
    mu_SharedCount.print_statistic();
    mu_Entries.print_statistic();
  }
  void clear() {
    mu_State.clear();
    mu_SharedCount.clear();
    mu_Entries.clear();
 };
  void undefine() {
    mu_State.undefine();
    mu_SharedCount.undefine();
    mu_Entries.undefine();
 };
  void reset() {
    mu_State.reset();
    mu_SharedCount.reset();
    mu_Entries.reset();
 };
  void print() {
    mu_State.print();
    mu_SharedCount.print();
    mu_Entries.print();
  };
  void print_diff(state *prevstate) {
    mu_State.print_diff(prevstate);
    mu_SharedCount.print_diff(prevstate);
    mu_Entries.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_State.to_state(thestate);
    mu_SharedCount.to_state(thestate);
    mu_Entries.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1__type_4& operator= (const mu_1__type_4& from) {
    mu_State.value(from.mu_State.value());
    mu_SharedCount.value(from.mu_SharedCount.value());
    mu_Entries = from.mu_Entries;
    return *this;
  };
};

  void mu_1__type_4::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_4::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_4::set_self(char *n, int os)
{
  name = n;
  mu_State.set_self_2(name, ".State", os + 0 );
  mu_SharedCount.set_self_2(name, ".SharedCount", os + 8 );
  mu_Entries.set_self_2(name, ".Entries", os + 16 );
}

mu_1__type_4::~mu_1__type_4()
{
}

/*** end record declaration ***/
mu_1__type_4 mu_1__type_4_undefined_var;

class mu_1__type_5
{
 public:
  mu_1__type_4 array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_5 (char *n, int os) { set_self(n, os); };
  mu_1__type_5 ( void ) {};
  virtual ~mu_1__type_5 ();
  mu_1__type_4& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 4 ) && ( index <= 4 ) )
      return array[ index - 4 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Address is internally represented from 4 to 4.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 4 ];
#endif
  };
  mu_1__type_5& operator= (const mu_1__type_5& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_5& a, mu_1__type_5& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_5& a, mu_1__type_5& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_5::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_5::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_5::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Address_1", i * 32 + os);i++;
}
mu_1__type_5::~mu_1__type_5()
{
}
/*** end array declaration ***/
mu_1__type_5 mu_1__type_5_undefined_var;

class mu_1_HomeState
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1__type_0 mu_Mem;
  mu_1__type_5 mu_Dir;
  mu_1_HomeState ( char *n, int os ) { set_self(n,os); };
  mu_1_HomeState ( void ) {};

  virtual ~mu_1_HomeState(); 
friend int CompareWeight(mu_1_HomeState& a, mu_1_HomeState& b)
  {
    int w;
    w = CompareWeight(a.mu_Mem, b.mu_Mem);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Dir, b.mu_Dir);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_HomeState& a, mu_1_HomeState& b)
  {
    int w;
    w = Compare(a.mu_Mem, b.mu_Mem);
    if (w!=0) return w;
    w = Compare(a.mu_Dir, b.mu_Dir);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Mem.MultisetSort();
    mu_Dir.MultisetSort();
  }
  void print_statistic()
  {
    mu_Mem.print_statistic();
    mu_Dir.print_statistic();
  }
  void clear() {
    mu_Mem.clear();
    mu_Dir.clear();
 };
  void undefine() {
    mu_Mem.undefine();
    mu_Dir.undefine();
 };
  void reset() {
    mu_Mem.reset();
    mu_Dir.reset();
 };
  void print() {
    mu_Mem.print();
    mu_Dir.print();
  };
  void print_diff(state *prevstate) {
    mu_Mem.print_diff(prevstate);
    mu_Dir.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Mem.to_state(thestate);
    mu_Dir.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_HomeState& operator= (const mu_1_HomeState& from) {
    mu_Mem = from.mu_Mem;
    mu_Dir = from.mu_Dir;
    return *this;
  };
};

  void mu_1_HomeState::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_HomeState::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_HomeState::set_self(char *n, int os)
{
  name = n;
  mu_Mem.set_self_2(name, ".Mem", os + 0 );
  mu_Dir.set_self_2(name, ".Dir", os + 8 );
}

mu_1_HomeState::~mu_1_HomeState()
{
}

/*** end record declaration ***/
mu_1_HomeState mu_1_HomeState_undefined_var;

class mu_1__type_6
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_CacheState mu_State;
  mu_1_Value mu_Value;
  mu_1__type_6 ( char *n, int os ) { set_self(n,os); };
  mu_1__type_6 ( void ) {};

  virtual ~mu_1__type_6(); 
friend int CompareWeight(mu_1__type_6& a, mu_1__type_6& b)
  {
    int w;
    w = CompareWeight(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1__type_6& a, mu_1__type_6& b)
  {
    int w;
    w = Compare(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = Compare(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_State.MultisetSort();
    mu_Value.MultisetSort();
  }
  void print_statistic()
  {
    mu_State.print_statistic();
    mu_Value.print_statistic();
  }
  void clear() {
    mu_State.clear();
    mu_Value.clear();
 };
  void undefine() {
    mu_State.undefine();
    mu_Value.undefine();
 };
  void reset() {
    mu_State.reset();
    mu_Value.reset();
 };
  void print() {
    mu_State.print();
    mu_Value.print();
  };
  void print_diff(state *prevstate) {
    mu_State.print_diff(prevstate);
    mu_Value.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_State.to_state(thestate);
    mu_Value.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1__type_6& operator= (const mu_1__type_6& from) {
    mu_State.value(from.mu_State.value());
    mu_Value.value(from.mu_Value.value());
    return *this;
  };
};

  void mu_1__type_6::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_6::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_6::set_self(char *n, int os)
{
  name = n;
  mu_State.set_self_2(name, ".State", os + 0 );
  mu_Value.set_self_2(name, ".Value", os + 8 );
}

mu_1__type_6::~mu_1__type_6()
{
}

/*** end record declaration ***/
mu_1__type_6 mu_1__type_6_undefined_var;

class mu_1__type_7
{
 public:
  mu_1__type_6 array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_7 (char *n, int os) { set_self(n, os); };
  mu_1__type_7 ( void ) {};
  virtual ~mu_1__type_7 ();
  mu_1__type_6& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 4 ) && ( index <= 4 ) )
      return array[ index - 4 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Address is internally represented from 4 to 4.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 4 ];
#endif
  };
  mu_1__type_7& operator= (const mu_1__type_7& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_7& a, mu_1__type_7& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_7& a, mu_1__type_7& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_7::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_7::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_7::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Address_1", i * 16 + os);i++;
}
mu_1__type_7::~mu_1__type_7()
{
}
/*** end array declaration ***/
mu_1__type_7 mu_1__type_7_undefined_var;

class mu_1__type_8
{
 public:
  mu_1__type_7 array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_8 (char *n, int os) { set_self(n, os); };
  mu_1__type_8 ( void ) {};
  virtual ~mu_1__type_8 ();
  mu_1__type_7& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - 1 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Home is internally represented from 1 to 1.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 1 ];
#endif
  };
  mu_1__type_8& operator= (const mu_1__type_8& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_8& a, mu_1__type_8& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_8& a, mu_1__type_8& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_8::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_8::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_8::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 16 + os);i++;
}
mu_1__type_8::~mu_1__type_8()
{
}
/*** end array declaration ***/
mu_1__type_8 mu_1__type_8_undefined_var;

class mu_1__type_9
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1_RAC_State mu_State;
  mu_1_Value mu_Value;
  mu_1_NodeCount mu_InvCount;
  mu_1__type_9 ( char *n, int os ) { set_self(n,os); };
  mu_1__type_9 ( void ) {};

  virtual ~mu_1__type_9(); 
friend int CompareWeight(mu_1__type_9& a, mu_1__type_9& b)
  {
    int w;
    w = CompareWeight(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
    w = CompareWeight(a.mu_InvCount, b.mu_InvCount);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1__type_9& a, mu_1__type_9& b)
  {
    int w;
    w = Compare(a.mu_State, b.mu_State);
    if (w!=0) return w;
    w = Compare(a.mu_Value, b.mu_Value);
    if (w!=0) return w;
    w = Compare(a.mu_InvCount, b.mu_InvCount);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_State.MultisetSort();
    mu_Value.MultisetSort();
    mu_InvCount.MultisetSort();
  }
  void print_statistic()
  {
    mu_State.print_statistic();
    mu_Value.print_statistic();
    mu_InvCount.print_statistic();
  }
  void clear() {
    mu_State.clear();
    mu_Value.clear();
    mu_InvCount.clear();
 };
  void undefine() {
    mu_State.undefine();
    mu_Value.undefine();
    mu_InvCount.undefine();
 };
  void reset() {
    mu_State.reset();
    mu_Value.reset();
    mu_InvCount.reset();
 };
  void print() {
    mu_State.print();
    mu_Value.print();
    mu_InvCount.print();
  };
  void print_diff(state *prevstate) {
    mu_State.print_diff(prevstate);
    mu_Value.print_diff(prevstate);
    mu_InvCount.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_State.to_state(thestate);
    mu_Value.to_state(thestate);
    mu_InvCount.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1__type_9& operator= (const mu_1__type_9& from) {
    mu_State.value(from.mu_State.value());
    mu_Value.value(from.mu_Value.value());
    mu_InvCount.value(from.mu_InvCount.value());
    return *this;
  };
};

  void mu_1__type_9::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_9::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_9::set_self(char *n, int os)
{
  name = n;
  mu_State.set_self_2(name, ".State", os + 0 );
  mu_Value.set_self_2(name, ".Value", os + 8 );
  mu_InvCount.set_self_2(name, ".InvCount", os + 16 );
}

mu_1__type_9::~mu_1__type_9()
{
}

/*** end record declaration ***/
mu_1__type_9 mu_1__type_9_undefined_var;

class mu_1__type_10
{
 public:
  mu_1__type_9 array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_10 (char *n, int os) { set_self(n, os); };
  mu_1__type_10 ( void ) {};
  virtual ~mu_1__type_10 ();
  mu_1__type_9& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 4 ) && ( index <= 4 ) )
      return array[ index - 4 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Address is internally represented from 4 to 4.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 4 ];
#endif
  };
  mu_1__type_10& operator= (const mu_1__type_10& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_10& a, mu_1__type_10& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_10& a, mu_1__type_10& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_10::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_10::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_10::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Address_1", i * 24 + os);i++;
}
mu_1__type_10::~mu_1__type_10()
{
}
/*** end array declaration ***/
mu_1__type_10 mu_1__type_10_undefined_var;

class mu_1__type_11
{
 public:
  mu_1__type_10 array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_11 (char *n, int os) { set_self(n, os); };
  mu_1__type_11 ( void ) {};
  virtual ~mu_1__type_11 ();
  mu_1__type_10& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - 1 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Home is internally represented from 1 to 1.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 1 ];
#endif
  };
  mu_1__type_11& operator= (const mu_1__type_11& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_11& a, mu_1__type_11& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_11& a, mu_1__type_11& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_11::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_11::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_11::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 24 + os);i++;
}
mu_1__type_11::~mu_1__type_11()
{
}
/*** end array declaration ***/
mu_1__type_11 mu_1__type_11_undefined_var;

class mu_1_ProcState
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1__type_8 mu_Cache;
  mu_1__type_11 mu_RAC;
  mu_1_ProcState ( char *n, int os ) { set_self(n,os); };
  mu_1_ProcState ( void ) {};

  virtual ~mu_1_ProcState(); 
friend int CompareWeight(mu_1_ProcState& a, mu_1_ProcState& b)
  {
    int w;
    w = CompareWeight(a.mu_Cache, b.mu_Cache);
    if (w!=0) return w;
    w = CompareWeight(a.mu_RAC, b.mu_RAC);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1_ProcState& a, mu_1_ProcState& b)
  {
    int w;
    w = Compare(a.mu_Cache, b.mu_Cache);
    if (w!=0) return w;
    w = Compare(a.mu_RAC, b.mu_RAC);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Cache.MultisetSort();
    mu_RAC.MultisetSort();
  }
  void print_statistic()
  {
    mu_Cache.print_statistic();
    mu_RAC.print_statistic();
  }
  void clear() {
    mu_Cache.clear();
    mu_RAC.clear();
 };
  void undefine() {
    mu_Cache.undefine();
    mu_RAC.undefine();
 };
  void reset() {
    mu_Cache.reset();
    mu_RAC.reset();
 };
  void print() {
    mu_Cache.print();
    mu_RAC.print();
  };
  void print_diff(state *prevstate) {
    mu_Cache.print_diff(prevstate);
    mu_RAC.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Cache.to_state(thestate);
    mu_RAC.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1_ProcState& operator= (const mu_1_ProcState& from) {
    mu_Cache = from.mu_Cache;
    mu_RAC = from.mu_RAC;
    return *this;
  };
};

  void mu_1_ProcState::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1_ProcState::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1_ProcState::set_self(char *n, int os)
{
  name = n;
  mu_Cache.set_self_2(name, ".Cache", os + 0 );
  mu_RAC.set_self_2(name, ".RAC", os + 16 );
}

mu_1_ProcState::~mu_1_ProcState()
{
}

/*** end record declaration ***/
mu_1_ProcState mu_1_ProcState_undefined_var;

class mu_1__type_12: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1__type_12& val) { return mu__byte::operator=((int) val); };
  mu_1__type_12 (char *name, int os): mu__byte(0, 6, 3, name, os) {};
  mu_1__type_12 (void): mu__byte(0, 6, 3) {};
  mu_1__type_12 (int val): mu__byte(0, 6, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1__type_12 mu_1__type_12_undefined_var;

class mu__subrange_30: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_30& val) { return mu__byte::operator=((int) val); };
  mu__subrange_30 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_30 (void): mu__byte(0, 5, 3) {};
  mu__subrange_30 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_30 mu__subrange_30_undefined_var;

class mu_1__type_13
{
 public:
  mu_1_Request array[ 6 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_13 (char *n, int os) { set_self(n, os); };
  mu_1__type_13 ( void ) {};
  virtual ~mu_1__type_13 ();
  mu_1_Request& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 0 ) && ( index <= 5 ) )
      return array[ index - 0 ];
    else {
      if (index==UNDEFVAL) 
        Error.Error("Indexing to %s using an undefined value.", name);
      else
        Error.Error("%d not in index range of %s.", index, name);
      return array[0];
    }
#else
    return array[ index - 0 ];
#endif
  };
  mu_1__type_13& operator= (const mu_1__type_13& from)
  {
    for (int i = 0; i < 6; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_13& a, mu_1__type_13& b)
  {
    int w;
    for (int i=0; i<6; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_13& a, mu_1__type_13& b)
  {
    int w;
    for (int i=0; i<6; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<6; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<6; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 6; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 6; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 6; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 6; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 6; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 6; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_13::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_13::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_13::set_self( char *n, int os)
{
  char* s;
  name = n;
  for(int i = 0; i < 6; i++) {
    array[i].set_self_ar(n, s=tsprintf("%d",i + 0), i * 32 + os);
    delete[] s;
  }
};
mu_1__type_13::~mu_1__type_13()
{
}
/*** end array declaration ***/
mu_1__type_13 mu_1__type_13_undefined_var;

class mu_1__type_14
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1__type_12 mu_Count;
  mu_1__type_13 mu_Messages;
  mu_1__type_14 ( char *n, int os ) { set_self(n,os); };
  mu_1__type_14 ( void ) {};

  virtual ~mu_1__type_14(); 
friend int CompareWeight(mu_1__type_14& a, mu_1__type_14& b)
  {
    int w;
    w = CompareWeight(a.mu_Count, b.mu_Count);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Messages, b.mu_Messages);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1__type_14& a, mu_1__type_14& b)
  {
    int w;
    w = Compare(a.mu_Count, b.mu_Count);
    if (w!=0) return w;
    w = Compare(a.mu_Messages, b.mu_Messages);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Count.MultisetSort();
    mu_Messages.MultisetSort();
  }
  void print_statistic()
  {
    mu_Count.print_statistic();
    mu_Messages.print_statistic();
  }
  void clear() {
    mu_Count.clear();
    mu_Messages.clear();
 };
  void undefine() {
    mu_Count.undefine();
    mu_Messages.undefine();
 };
  void reset() {
    mu_Count.reset();
    mu_Messages.reset();
 };
  void print() {
    mu_Count.print();
    mu_Messages.print();
  };
  void print_diff(state *prevstate) {
    mu_Count.print_diff(prevstate);
    mu_Messages.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Count.to_state(thestate);
    mu_Messages.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1__type_14& operator= (const mu_1__type_14& from) {
    mu_Count.value(from.mu_Count.value());
    mu_Messages = from.mu_Messages;
    return *this;
  };
};

  void mu_1__type_14::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_14::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_14::set_self(char *n, int os)
{
  name = n;
  mu_Count.set_self_2(name, ".Count", os + 0 );
  mu_Messages.set_self_2(name, ".Messages", os + 8 );
}

mu_1__type_14::~mu_1__type_14()
{
}

/*** end record declaration ***/
mu_1__type_14 mu_1__type_14_undefined_var;

class mu_1__type_15
{
 public:
  mu_1__type_14 array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_15 (char *n, int os) { set_self(n, os); };
  mu_1__type_15 ( void ) {};
  virtual ~mu_1__type_15 ();
  mu_1__type_14& operator[] (int index) /* const */
  {
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - (1) ];
    if ( ( index >= 2 ) && ( index <= 3 ) )
      return array[ index - (1) ];
    if (index==UNDEFVAL) 
      Error.Error("Indexing to %s using an undefined value.", name);
    else
      Error.Error("Funny index value %d for %s. (Internal Error in Type Checking.",index, name);
    return array[0];
  }
  mu_1__type_15& operator= (const mu_1__type_15& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_15& a, mu_1__type_15& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_15& a, mu_1__type_15& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_15::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_15::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_15::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 200 + os);i++;
array[i].set_self_ar(n,"Remote_1", i * 200 + os);i++;
array[i].set_self_ar(n,"Remote_2", i * 200 + os);i++;
}
mu_1__type_15::~mu_1__type_15()
{
}
/*** end array declaration ***/
mu_1__type_15 mu_1__type_15_undefined_var;

class mu_1__type_16
{
 public:
  mu_1__type_15 array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_16 (char *n, int os) { set_self(n, os); };
  mu_1__type_16 ( void ) {};
  virtual ~mu_1__type_16 ();
  mu_1__type_15& operator[] (int index) /* const */
  {
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - (1) ];
    if ( ( index >= 2 ) && ( index <= 3 ) )
      return array[ index - (1) ];
    if (index==UNDEFVAL) 
      Error.Error("Indexing to %s using an undefined value.", name);
    else
      Error.Error("Funny index value %d for %s. (Internal Error in Type Checking.",index, name);
    return array[0];
  }
  mu_1__type_16& operator= (const mu_1__type_16& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_16& a, mu_1__type_16& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_16& a, mu_1__type_16& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_16::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_16::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_16::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 600 + os);i++;
array[i].set_self_ar(n,"Remote_1", i * 600 + os);i++;
array[i].set_self_ar(n,"Remote_2", i * 600 + os);i++;
}
mu_1__type_16::~mu_1__type_16()
{
}
/*** end array declaration ***/
mu_1__type_16 mu_1__type_16_undefined_var;

class mu_1__type_17: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu_1__type_17& val) { return mu__byte::operator=((int) val); };
  mu_1__type_17 (char *name, int os): mu__byte(0, 6, 3, name, os) {};
  mu_1__type_17 (void): mu__byte(0, 6, 3) {};
  mu_1__type_17 (int val): mu__byte(0, 6, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu_1__type_17 mu_1__type_17_undefined_var;

class mu__subrange_36: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_36& val) { return mu__byte::operator=((int) val); };
  mu__subrange_36 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_36 (void): mu__byte(0, 5, 3) {};
  mu__subrange_36 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_36 mu__subrange_36_undefined_var;

class mu_1__type_18
{
 public:
  mu_1_Reply array[ 6 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_18 (char *n, int os) { set_self(n, os); };
  mu_1__type_18 ( void ) {};
  virtual ~mu_1__type_18 ();
  mu_1_Reply& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 0 ) && ( index <= 5 ) )
      return array[ index - 0 ];
    else {
      if (index==UNDEFVAL) 
        Error.Error("Indexing to %s using an undefined value.", name);
      else
        Error.Error("%d not in index range of %s.", index, name);
      return array[0];
    }
#else
    return array[ index - 0 ];
#endif
  };
  mu_1__type_18& operator= (const mu_1__type_18& from)
  {
    for (int i = 0; i < 6; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_18& a, mu_1__type_18& b)
  {
    int w;
    for (int i=0; i<6; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_18& a, mu_1__type_18& b)
  {
    int w;
    for (int i=0; i<6; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<6; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<6; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 6; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 6; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 6; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 6; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 6; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 6; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_18::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_18::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_18::set_self( char *n, int os)
{
  char* s;
  name = n;
  for(int i = 0; i < 6; i++) {
    array[i].set_self_ar(n, s=tsprintf("%d",i + 0), i * 40 + os);
    delete[] s;
  }
};
mu_1__type_18::~mu_1__type_18()
{
}
/*** end array declaration ***/
mu_1__type_18 mu_1__type_18_undefined_var;

class mu_1__type_19
{
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  void set_self(char *n, int os);
  mu_1__type_17 mu_Count;
  mu_1__type_18 mu_Messages;
  mu_1__type_19 ( char *n, int os ) { set_self(n,os); };
  mu_1__type_19 ( void ) {};

  virtual ~mu_1__type_19(); 
friend int CompareWeight(mu_1__type_19& a, mu_1__type_19& b)
  {
    int w;
    w = CompareWeight(a.mu_Count, b.mu_Count);
    if (w!=0) return w;
    w = CompareWeight(a.mu_Messages, b.mu_Messages);
    if (w!=0) return w;
  return 0;
}
friend int Compare(mu_1__type_19& a, mu_1__type_19& b)
  {
    int w;
    w = Compare(a.mu_Count, b.mu_Count);
    if (w!=0) return w;
    w = Compare(a.mu_Messages, b.mu_Messages);
    if (w!=0) return w;
  return 0;
}
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    mu_Count.MultisetSort();
    mu_Messages.MultisetSort();
  }
  void print_statistic()
  {
    mu_Count.print_statistic();
    mu_Messages.print_statistic();
  }
  void clear() {
    mu_Count.clear();
    mu_Messages.clear();
 };
  void undefine() {
    mu_Count.undefine();
    mu_Messages.undefine();
 };
  void reset() {
    mu_Count.reset();
    mu_Messages.reset();
 };
  void print() {
    mu_Count.print();
    mu_Messages.print();
  };
  void print_diff(state *prevstate) {
    mu_Count.print_diff(prevstate);
    mu_Messages.print_diff(prevstate);
  };
  void to_state(state *thestate) {
    mu_Count.to_state(thestate);
    mu_Messages.to_state(thestate);
  };
virtual bool isundefined() { Error.Error("Checking undefinedness of a non-base type"); return TRUE;}
virtual bool ismember() { Error.Error("Checking membership for a non-base type"); return TRUE;}
  mu_1__type_19& operator= (const mu_1__type_19& from) {
    mu_Count.value(from.mu_Count.value());
    mu_Messages = from.mu_Messages;
    return *this;
  };
};

  void mu_1__type_19::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_19::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_19::set_self(char *n, int os)
{
  name = n;
  mu_Count.set_self_2(name, ".Count", os + 0 );
  mu_Messages.set_self_2(name, ".Messages", os + 8 );
}

mu_1__type_19::~mu_1__type_19()
{
}

/*** end record declaration ***/
mu_1__type_19 mu_1__type_19_undefined_var;

class mu_1__type_20
{
 public:
  mu_1__type_19 array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_20 (char *n, int os) { set_self(n, os); };
  mu_1__type_20 ( void ) {};
  virtual ~mu_1__type_20 ();
  mu_1__type_19& operator[] (int index) /* const */
  {
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - (1) ];
    if ( ( index >= 2 ) && ( index <= 3 ) )
      return array[ index - (1) ];
    if (index==UNDEFVAL) 
      Error.Error("Indexing to %s using an undefined value.", name);
    else
      Error.Error("Funny index value %d for %s. (Internal Error in Type Checking.",index, name);
    return array[0];
  }
  mu_1__type_20& operator= (const mu_1__type_20& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_20& a, mu_1__type_20& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_20& a, mu_1__type_20& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_20::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_20::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_20::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 248 + os);i++;
array[i].set_self_ar(n,"Remote_1", i * 248 + os);i++;
array[i].set_self_ar(n,"Remote_2", i * 248 + os);i++;
}
mu_1__type_20::~mu_1__type_20()
{
}
/*** end array declaration ***/
mu_1__type_20 mu_1__type_20_undefined_var;

class mu_1__type_21
{
 public:
  mu_1__type_20 array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_21 (char *n, int os) { set_self(n, os); };
  mu_1__type_21 ( void ) {};
  virtual ~mu_1__type_21 ();
  mu_1__type_20& operator[] (int index) /* const */
  {
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - (1) ];
    if ( ( index >= 2 ) && ( index <= 3 ) )
      return array[ index - (1) ];
    if (index==UNDEFVAL) 
      Error.Error("Indexing to %s using an undefined value.", name);
    else
      Error.Error("Funny index value %d for %s. (Internal Error in Type Checking.",index, name);
    return array[0];
  }
  mu_1__type_21& operator= (const mu_1__type_21& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_21& a, mu_1__type_21& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_21& a, mu_1__type_21& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_21::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_21::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_21::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 744 + os);i++;
array[i].set_self_ar(n,"Remote_1", i * 744 + os);i++;
array[i].set_self_ar(n,"Remote_2", i * 744 + os);i++;
}
mu_1__type_21::~mu_1__type_21()
{
}
/*** end array declaration ***/
mu_1__type_21 mu_1__type_21_undefined_var;

class mu_1__type_22
{
 public:
  mu_1_ProcState array[ 3 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_22 (char *n, int os) { set_self(n, os); };
  mu_1__type_22 ( void ) {};
  virtual ~mu_1__type_22 ();
  mu_1_ProcState& operator[] (int index) /* const */
  {
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - (1) ];
    if ( ( index >= 2 ) && ( index <= 3 ) )
      return array[ index - (1) ];
    if (index==UNDEFVAL) 
      Error.Error("Indexing to %s using an undefined value.", name);
    else
      Error.Error("Funny index value %d for %s. (Internal Error in Type Checking.",index, name);
    return array[0];
  }
  mu_1__type_22& operator= (const mu_1__type_22& from)
  {
    for (int i = 0; i < 3; i++)
      array[i] = from.array[i];
    return *this;
  }

friend int CompareWeight(mu_1__type_22& a, mu_1__type_22& b)
  {
    return 0;
  }
friend int Compare(mu_1__type_22& a, mu_1__type_22& b)
  {
    int w;
    for (int i=0; i<3; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<3; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<3; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 3; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 3; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 3; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 3; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 3; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 3; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_22::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_22::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_22::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 40 + os);i++;
array[i].set_self_ar(n,"Remote_1", i * 40 + os);i++;
array[i].set_self_ar(n,"Remote_2", i * 40 + os);i++;
}
mu_1__type_22::~mu_1__type_22()
{
}
/*** end array declaration ***/
mu_1__type_22 mu_1__type_22_undefined_var;

class mu_1__type_23
{
 public:
  mu_1_HomeState array[ 1 ];
 public:
  char *name;
  char longname[BUFFER_SIZE/4];
  void set_self( char *n, int os);
  void set_self_2( char *n, char *n2, int os);
  void set_self_ar( char *n, char *n2, int os);
  mu_1__type_23 (char *n, int os) { set_self(n, os); };
  mu_1__type_23 ( void ) {};
  virtual ~mu_1__type_23 ();
  mu_1_HomeState& operator[] (int index) /* const */
  {
#ifndef NO_RUN_TIME_CHECKING
    if ( ( index >= 1 ) && ( index <= 1 ) )
      return array[ index - 1 ];
    else
      {
        if (index==UNDEFVAL) 
          Error.Error("Indexing to %s using an undefined value.", name);
        else
          Error.Error("Funny index value %d for %s: Home is internally represented from 1 to 1.\nInternal Error in Type checking.",index, name);
        return array[0];
      }
#else
    return array[ index - 1 ];
#endif
  };
  mu_1__type_23& operator= (const mu_1__type_23& from)
  {
      array[0] = from.array[0];
    return *this;
  }

friend int CompareWeight(mu_1__type_23& a, mu_1__type_23& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = CompareWeight(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
friend int Compare(mu_1__type_23& a, mu_1__type_23& b)
  {
    int w;
    for (int i=0; i<1; i++) {
      w = Compare(a.array[i], b.array[i]);
      if (w!=0) return w;
    }
    return 0;
  }
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort()
  {
    for (int i=0; i<1; i++)
      array[i].MultisetSort();
  }
  void print_statistic()
  {
    for (int i=0; i<1; i++)
      array[i].print_statistic();
  }
  void clear() { for (int i = 0; i < 1; i++) array[i].clear(); };

  void undefine() { for (int i = 0; i < 1; i++) array[i].undefine(); };

  void reset() { for (int i = 0; i < 1; i++) array[i].reset(); };

  void to_state(state *thestate)
  {
    for (int i = 0; i < 1; i++)
      array[i].to_state(thestate);
  };

  void print()
  {
    for (int i = 0; i < 1; i++)
      array[i].print(); };

  void print_diff(state *prevstate)
  {
    for (int i = 0; i < 1; i++)
      array[i].print_diff(prevstate);
  };
};

  void mu_1__type_23::set_self_ar( char *n1, char *n2, int os ) {
    int l1 = strlen(n1), l2 = strlen(n2);
    strcpy( longname, n1 );
    longname[l1] = '[';
    strcpy( longname+l1+1, n2 );
    longname[l1+l2+1] = ']';
    longname[l1+l2+2] = 0;
    set_self( longname, os );
  };
  void mu_1__type_23::set_self_2( char *n1, char *n2, int os ) {
    strcpy( longname, n1 );
    strcat( longname, n2 );
    set_self( longname, os );
  };
void mu_1__type_23::set_self( char *n, int os)
  {
    int i=0;
    name = n;
array[i].set_self_ar(n,"Home_1", i * 40 + os);i++;
}
mu_1__type_23::~mu_1__type_23()
{
}
/*** end array declaration ***/
mu_1__type_23 mu_1__type_23_undefined_var;

class mu__subrange_43: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_43& val) { return mu__byte::operator=((int) val); };
  mu__subrange_43 (char *name, int os): mu__byte(0, 1, 2, name, os) {};
  mu__subrange_43 (void): mu__byte(0, 1, 2) {};
  mu__subrange_43 (int val): mu__byte(0, 1, 2, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_43 mu__subrange_43_undefined_var;

class mu__subrange_44: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_44& val) { return mu__byte::operator=((int) val); };
  mu__subrange_44 (char *name, int os): mu__byte(0, 4, 3, name, os) {};
  mu__subrange_44 (void): mu__byte(0, 4, 3) {};
  mu__subrange_44 (int val): mu__byte(0, 4, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_44 mu__subrange_44_undefined_var;

class mu__subrange_45: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_45& val) { return mu__byte::operator=((int) val); };
  mu__subrange_45 (char *name, int os): mu__byte(0, 4, 3, name, os) {};
  mu__subrange_45 (void): mu__byte(0, 4, 3) {};
  mu__subrange_45 (int val): mu__byte(0, 4, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_45 mu__subrange_45_undefined_var;

class mu__subrange_46: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_46& val) { return mu__byte::operator=((int) val); };
  mu__subrange_46 (char *name, int os): mu__byte(0, 1, 2, name, os) {};
  mu__subrange_46 (void): mu__byte(0, 1, 2) {};
  mu__subrange_46 (int val): mu__byte(0, 1, 2, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_46 mu__subrange_46_undefined_var;

class mu__subrange_47: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_47& val) { return mu__byte::operator=((int) val); };
  mu__subrange_47 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_47 (void): mu__byte(0, 5, 3) {};
  mu__subrange_47 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_47 mu__subrange_47_undefined_var;

class mu__subrange_48: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_48& val) { return mu__byte::operator=((int) val); };
  mu__subrange_48 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_48 (void): mu__byte(0, 5, 3) {};
  mu__subrange_48 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_48 mu__subrange_48_undefined_var;

class mu__subrange_49: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_49& val) { return mu__byte::operator=((int) val); };
  mu__subrange_49 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_49 (void): mu__byte(0, 5, 3) {};
  mu__subrange_49 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_49 mu__subrange_49_undefined_var;

class mu__subrange_50: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_50& val) { return mu__byte::operator=((int) val); };
  mu__subrange_50 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_50 (void): mu__byte(0, 5, 3) {};
  mu__subrange_50 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_50 mu__subrange_50_undefined_var;

class mu__subrange_51: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_51& val) { return mu__byte::operator=((int) val); };
  mu__subrange_51 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_51 (void): mu__byte(0, 5, 3) {};
  mu__subrange_51 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_51 mu__subrange_51_undefined_var;

class mu__subrange_52: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_52& val) { return mu__byte::operator=((int) val); };
  mu__subrange_52 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_52 (void): mu__byte(0, 5, 3) {};
  mu__subrange_52 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_52 mu__subrange_52_undefined_var;

class mu__subrange_53: public mu__byte
{
 public:
  inline int operator=(int val) { return mu__byte::operator=(val); };
  inline int operator=(const mu__subrange_53& val) { return mu__byte::operator=((int) val); };
  mu__subrange_53 (char *name, int os): mu__byte(0, 5, 3, name, os) {};
  mu__subrange_53 (void): mu__byte(0, 5, 3) {};
  mu__subrange_53 (int val): mu__byte(0, 5, 3, "Parameter or function result.", 0)
  {
    operator=(val);
  };
  char * Name() { return tsprintf("%d",value()); };
  virtual void Permute(PermSet& Perm, int i);
  virtual void SimpleCanonicalize(PermSet& Perm);
  virtual void Canonicalize(PermSet& Perm);
  virtual void SimpleLimit(PermSet& Perm);
  virtual void ArrayLimit(PermSet& Perm);
  virtual void Limit(PermSet& Perm);
  virtual void MultisetLimit(PermSet& Perm);
  virtual void MultisetSort() {};
  void print_statistic() {};
};

/*** end of subrange decl ***/
mu__subrange_53 mu__subrange_53_undefined_var;

const int mu_HomeCount = 1;
const int mu_RemoteCount = 2;
const int mu_ProcCount = 3;
const int mu_AddressCount = 1;
const int mu_ValueCount = 2;
const int mu_DirMax = 2;
const int mu_ChanMax = 6;
const int mu_bug1 = 0;
const int mu_nohome = 1;
const int mu_Home_1 = 1;
const int mu_Remote_1 = 2;
const int mu_Remote_2 = 3;
const int mu_Address_1 = 4;
const int mu_Value_1 = 5;
const int mu_Value_2 = 6;
const int mu_RD_H = 7;
const int mu_RD_RAC = 8;
const int mu_RDX_H = 9;
const int mu_RDX_RAC = 10;
const int mu_INV = 11;
const int mu_WB = 12;
const int mu_SHWB = 13;
const int mu_DXFER = 14;
const int mu_DRD_H = 15;
const int mu_DRD_RAC = 16;
const int mu_DWR_H = 17;
const int mu_DWR_RAC = 18;
const int mu_DUP = 19;
const int mu_ACK = 20;
const int mu_NAK = 21;
const int mu_IACK = 22;
const int mu_SACK = 23;
const int mu_INVAL = 24;
const int mu_WRD = 25;
const int mu_WRDO = 26;
const int mu_WRDX = 27;
const int mu_WINV = 28;
const int mu_RRD = 29;
const int mu_WDMAR = 30;
const int mu_WUP = 31;
const int mu_WDMAW = 32;
const int mu_Non_Locally_Cached = 33;
const int mu_Locally_Shared = 34;
const int mu_Locally_Exmod = 35;
const int mu_Uncached = 36;
const int mu_Shared_Remote = 37;
const int mu_Dirty_Remote = 38;
/*** Variable declaration ***/
mu_1__type_16 mu_ReqNet("ReqNet",0);

/*** Variable declaration ***/
mu_1__type_21 mu_ReplyNet("ReplyNet",1800);

/*** Variable declaration ***/
mu_1__type_22 mu_Procs("Procs",4032);

/*** Variable declaration ***/
mu_1__type_23 mu_Homes("Homes",4152);

void mu_Set_Dir_1st_Entry(const mu_1_Home& mu_h, const mu_1_Address& mu_a, const mu_1_Proc& mu_n)
{
mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries.undefine();
if (mu_n.isundefined())
  mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[0].undefine();
else
  mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[0] = mu_n;
};
/*** end procedure declaration ***/

void mu_Add_to_Dir_Entries(const mu_1_Home& mu_h, const mu_1_Address& mu_a, const mu_1_Proc& mu_n)
{
{
  mu_1__type_2& mu_SharedCount = mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount;
bool mu__quant24; 
mu__quant24 = TRUE;
{
for(int mu_i = 0; mu_i <= 1; mu_i++) {
bool mu__boolexpr25;
  if (!((mu_i) < (mu_SharedCount))) mu__boolexpr25 = TRUE ;
  else {
  mu__boolexpr25 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_i]) != (mu_n)) ; 
}
if ( !(mu__boolexpr25) )
  { mu__quant24 = FALSE; break; }
};
};
if ( mu__quant24 )
{
if (mu_n.isundefined())
  mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_SharedCount].undefine();
else
  mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_SharedCount] = mu_n;
mu_SharedCount = (mu_SharedCount) + (1);
}
}
};
/*** end procedure declaration ***/

void mu_Send_Req(const mu_1_RequestType& mu_t, const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
if (mu_t.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_t;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Consume_Request(const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Dst)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
{
for(int mu_i = 0; mu_i <= 4; mu_i++) {
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_i] = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[(mu_i) + (1)];
};
};
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[(mu_Count) - (1)].undefine();
mu_Count = (mu_Count) - (1);
}
};
/*** end procedure declaration ***/

void mu_Send_Reply(const mu_1_ReplyType& mu_t, const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val, const mu_1_NodeCount& mu_InvCount)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
if (mu_t.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_t;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
if (mu_InvCount.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = mu_InvCount;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Consume_Reply(const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Dst)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
{
for(int mu_i = 0; mu_i <= 4; mu_i++) {
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_i] = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[(mu_i) + (1)];
};
};
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[(mu_Count) - (1)].undefine();
mu_Count = (mu_Count) - (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Req_H(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_RD_H;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Req_RAC(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_RD_RAC;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_SH_WB_Req(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_SHWB;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_Inv_Req(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_INV;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Ex_Req_RAC(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_RDX_RAC;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Ex_Req_H(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_RDX_H;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_Dirty_Transfer_Req(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DXFER;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_WB_Req(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_WB;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Reply(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Home, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_ACK;
if (mu_Home.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Home;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_NAK(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_NAK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_Inv_Ack(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_SACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_R_Ex_Reply(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val, const mu_1_NodeCount& mu_InvCount)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
if ( (mu_InvCount) == (0) )
{
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_ACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
else
{
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_IACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
if (mu_InvCount.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = mu_InvCount;
mu_Count = (mu_Count) + (1);
}
}
};
/*** end procedure declaration ***/

void mu_Send_Dirty_Transfer_Ack(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_SACK;
if (mu_Src.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Src;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_R_Req_H(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DRD_H;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_R_Req_RAC(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DRD_RAC;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_W_Req_H(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DWR_H;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_Update_Count(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Address& mu_Addr, const mu_1_NodeCount& mu_SharedCount)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_IACK;
if (mu_Src.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Src;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
if (mu_SharedCount.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = mu_SharedCount;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_Update_Req(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DUP;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_W_Req_RAC(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_12& mu_Count = mu_ReqNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Request Channel is full");
mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_DWR_RAC;
if (mu_Aux.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReqNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_R_Reply(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr, const mu_1_Value& mu_Val)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_ACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
if (mu_Val.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value = mu_Val;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_W_Ack(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_ACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/

void mu_Send_DMA_Update_Ack(const mu_1_Proc& mu_Dst, const mu_1_Proc& mu_Src, const mu_1_Proc& mu_Aux, const mu_1_Address& mu_Addr)
{
{
  mu_1__type_17& mu_Count = mu_ReplyNet[mu_Src][mu_Dst].mu_Count;
if ( !((mu_Count) != (mu_ChanMax)) ) Error.Error("Assertion failed: Reply Channel is full");
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Mtype = mu_SACK;
if (mu_Aux.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Aux = mu_Aux;
if (mu_Addr.isundefined())
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr.undefine();
else
  mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Addr = mu_Addr;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_Value.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[mu_Count].mu_InvCount = 0;
mu_Count = (mu_Count) + (1);
}
};
/*** end procedure declaration ***/





/********************
  The world
 ********************/
void world_class::clear()
{
  mu_ReqNet.clear();
  mu_ReplyNet.clear();
  mu_Procs.clear();
  mu_Homes.clear();
}
void world_class::undefine()
{
  mu_ReqNet.undefine();
  mu_ReplyNet.undefine();
  mu_Procs.undefine();
  mu_Homes.undefine();
}
void world_class::reset()
{
  mu_ReqNet.reset();
  mu_ReplyNet.reset();
  mu_Procs.reset();
  mu_Homes.reset();
}
void world_class::print()
{
  static int num_calls = 0; /* to ward off recursive calls. */
  if ( num_calls == 0 ) {
    num_calls++;
  mu_ReqNet.print();
  mu_ReplyNet.print();
  mu_Procs.print();
  mu_Homes.print();
    num_calls--;
}
}
void world_class::print_statistic()
{
  static int num_calls = 0; /* to ward off recursive calls. */
  if ( num_calls == 0 ) {
    num_calls++;
  mu_ReqNet.print_statistic();
  mu_ReplyNet.print_statistic();
  mu_Procs.print_statistic();
  mu_Homes.print_statistic();
    num_calls--;
}
}
void world_class::print_diff( state *prevstate )
{
  if ( prevstate != NULL )
  {
    mu_ReqNet.print_diff(prevstate);
    mu_ReplyNet.print_diff(prevstate);
    mu_Procs.print_diff(prevstate);
    mu_Homes.print_diff(prevstate);
  }
  else
print();
}
void world_class::to_state(state *newstate)
{
  mu_ReqNet.to_state( newstate );
  mu_ReplyNet.to_state( newstate );
  mu_Procs.to_state( newstate );
  mu_Homes.to_state( newstate );
}
void world_class::setstate(state *thestate)
{
}


/********************
  Rule declarations
 ********************/
/******************** RuleBase0 ********************/
class RuleBase0
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    return tsprintf("modifying value at cache, v:%s, a:%s, n:%s, h:%s", mu_v.Name(), mu_a.Name(), mu_n.Name(), mu_h.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
  return (mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Exmod);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 0;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    while (what_rule < 6 )
      {
        if ( ( TRUE  ) ) {
              if ((mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Exmod)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 2;
        }
        else
          what_rule += 2;
    r = what_rule - 0;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_Value = mu_v;
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase1 ********************/
class RuleBase1
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle Supplementary Acknowledgement, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr26;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr26 = FALSE ;
  else {
  mu__boolexpr26 = ((mu_Reply) == (mu_SACK)) ; 
}
  return mu__boolexpr26;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 6;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 15 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr27;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr27 = FALSE ;
  else {
  mu__boolexpr27 = ((mu_Reply) == (mu_SACK)) ; 
}
              if (mu__boolexpr27) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 6;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Aux][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
Error.Error("Error: Invalidate acknowledge in INVAL RAC state");
break;
case mu_WINV:
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) - (1);
if ( (mu_RAC.mu_InvCount) == (0) )
{
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WRDX:
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) + (1);
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WUP:
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) - (1);
if ( (mu_RAC.mu_InvCount) == (0) )
{
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAW:
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) + (1);
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
default:
Error.Error("Error: Invalidate acknowledge in funny RAC state.");
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase2 ********************/
class RuleBase2
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle Indirect Acknowledgement, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr28;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr28 = FALSE ;
  else {
  mu__boolexpr28 = ((mu_Reply) == (mu_IACK)) ; 
}
  return mu__boolexpr28;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 15;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 24 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr29;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr29 = FALSE ;
  else {
  mu__boolexpr29 = ((mu_Reply) == (mu_IACK)) ; 
}
              if (mu__boolexpr29) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 15;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Aux][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Aux][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
Error.Error("Error: Read exclusive Reply in INVAL RAC state");
break;
case mu_WRDX:
mu_Cache.mu_State = mu_Locally_Exmod;
mu_Cache.mu_Value = mu_v;
if ( (mu_Dst) == (mu_Aux) )
{
{
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
Error.Error("Error: already sent invalidations to copy ??");
mu_Dir.mu_State = mu_Uncached;
mu_Dir.mu_SharedCount = 0;
mu_Dir.mu_Entries.undefine();
}
}
mu_RAC.mu_InvCount = (mu_ICount) - (mu_RAC.mu_InvCount);
mu_RAC.mu_State = mu_WINV;
if ( (mu_RAC.mu_InvCount) == (0) )
{
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAW:
mu_RAC.mu_State = mu_WUP;
mu_RAC.mu_InvCount = (mu_ICount) - (mu_RAC.mu_InvCount);
if ( (mu_RAC.mu_InvCount) == (0) )
{
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
default:
Error.Error("Error: Read exclusive reply in funny RAC state.");
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase3 ********************/
class RuleBase3
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle negative Acknowledgement, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr30;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr30 = FALSE ;
  else {
  mu__boolexpr30 = ((mu_Reply) == (mu_NAK)) ; 
}
  return mu__boolexpr30;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 24;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 33 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr31;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr31 = FALSE ;
  else {
  mu__boolexpr31 = ((mu_Reply) == (mu_NAK)) ; 
}
              if (mu__boolexpr31) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 24;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Aux][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Aux][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
Error.Error("Error: NAK in INVAL RAC state");
break;
case mu_WRD:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WRDO:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WRDX:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_RRD:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAR:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAW:
if ( (mu_Dst) == (mu_Aux) )
{
{
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
if ( (mu_Cache.mu_State) != (mu_Non_Locally_Cached) )
{
mu_Cache.mu_Value = mu_RAC.mu_Value;
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
mu_Mem = mu_RAC.mu_Value;
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
break;
case mu_Shared_Remote:
mu_Mem = mu_RAC.mu_Value;
{
for(int mu_i = 0; mu_i <= 2; mu_i++) {
if ( (mu_i) < (mu_Dir.mu_SharedCount) )
{
mu_Send_DMA_Update_Req ( mu_Dir.mu_Entries[mu_i], mu_Dst, mu_Dst, mu_Addr, mu_RAC.mu_Value );
}
};
};
mu_RAC.mu_State = mu_WUP;
mu_RAC.mu_InvCount = mu_Dir.mu_SharedCount;
mu_RAC.mu_Value.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
break;
case mu_Dirty_Remote:
mu_RAC.mu_State = mu_WDMAW;
mu_Send_DMA_W_Req_RAC ( mu_Dir.mu_Entries[0], mu_Dst, mu_Dst, mu_Addr, mu_RAC.mu_Value );
break;
}
}
}
else
{
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
Error.Error("Error: Cache changed to Exmod while WDMAW outstanding");
mu_Cache.mu_Value = mu_RAC.mu_Value;
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
break;
default:
mu_RAC.mu_State = mu_WDMAW;
mu_Send_DMA_W_Req_H ( mu_Aux, mu_Dst, mu_Addr, mu_RAC.mu_Value );
break;
}
}
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
default:
Error.Error("Error: NAK in funny RAC state");
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase4 ********************/
class RuleBase4
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle Acknowledgement, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr32;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr32 = FALSE ;
  else {
  mu__boolexpr32 = ((mu_Reply) == (mu_ACK)) ; 
}
  return mu__boolexpr32;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 33;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 42 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
bool mu__boolexpr33;
  if (!((mu_ReplyChan.mu_Count) > (0))) mu__boolexpr33 = FALSE ;
  else {
  mu__boolexpr33 = ((mu_Reply) == (mu_ACK)) ; 
}
              if (mu__boolexpr33) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 33;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_19& mu_ReplyChan = mu_ReplyNet[mu_Src][mu_Dst];
  mu_1_ReplyType& mu_Reply = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
  mu_1_Value& mu_v = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1_NodeCount& mu_ICount = mu_ReplyNet[mu_Src][mu_Dst].mu_Messages[0].mu_InvCount;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Aux][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Aux][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
Error.Error("Error: ACK in INVAL RAC state");
break;
case mu_WRD:
mu_Cache.mu_State = mu_Locally_Shared;
mu_Cache.mu_Value = mu_v;
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WRDO:
mu_Cache.mu_State = mu_Locally_Shared;
mu_Cache.mu_Value = mu_v;
mu_Homes[mu_Dst].mu_Mem[mu_Addr] = mu_v;
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WRDX:
mu_Cache.mu_State = mu_Locally_Exmod;
mu_Cache.mu_Value = mu_v;
if ( (mu_Dst) == (mu_Aux) )
{
{
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
mu_Dir.mu_State = mu_Uncached;
mu_Dir.mu_SharedCount = 0;
mu_Dir.mu_Entries.undefine();
}
}
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_RRD:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAR:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
case mu_WDMAW:
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
mu_Consume_Reply ( mu_Src, mu_Dst );
break;
default:
Error.Error("Error: ACK in funny RAC state");
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase5 ********************/
class RuleBase5
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle DMA write request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr34;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr34 = FALSE ;
  else {
  mu__boolexpr34 = ((mu_Request) == (mu_DWR_RAC)) ; 
}
  return mu__boolexpr34;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 42;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 51 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr35;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr35 = FALSE ;
  else {
  mu__boolexpr35 = ((mu_Request) == (mu_DWR_RAC)) ; 
}
              if (mu__boolexpr35) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 42;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1_Value& mu_v = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted.");
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Cache.mu_Value = mu_v;
mu_Send_DMA_W_Ack ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase6 ********************/
class RuleBase6
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle DMA update request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr36;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr36 = FALSE ;
  else {
  mu__boolexpr36 = ((mu_Request) == (mu_DUP)) ; 
}
  return mu__boolexpr36;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 51;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 60 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr37;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr37 = FALSE ;
  else {
  mu__boolexpr37 = ((mu_Request) == (mu_DUP)) ; 
}
              if (mu__boolexpr37) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 51;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1_Value& mu_v = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted.");
mu_Cache.mu_Value = mu_v;
break;
case mu_WRD:
mu_RAC.mu_State = mu_RRD;
break;
case mu_RRD:
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO in remote cluster");
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Shared:
mu_Cache.mu_Value = mu_v;
break;
case mu_Non_Locally_Cached:
break;
case mu_Locally_Exmod:
mu_Cache.mu_Value = mu_v;
break;
}
break;
}
mu_Send_DMA_Update_Ack ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase7 ********************/
class RuleBase7
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle DMA read request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr38;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr38 = FALSE ;
  else {
  mu__boolexpr38 = ((mu_Request) == (mu_DRD_RAC)) ; 
}
  return mu__boolexpr38;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 60;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 69 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr39;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr39 = FALSE ;
  else {
  mu__boolexpr39 = ((mu_Request) == (mu_DRD_RAC)) ; 
}
              if (mu__boolexpr39) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 60;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO in remote cluster");
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Send_DMA_R_Reply ( mu_Aux, mu_Dst, mu_Src, mu_Addr, mu_Cache.mu_Value );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase8 ********************/
class RuleBase8
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle read exclusive request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr40;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr40 = FALSE ;
  else {
  mu__boolexpr40 = ((mu_Request) == (mu_RDX_RAC)) ; 
}
  return mu__boolexpr40;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 69;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 78 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr41;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr41 = FALSE ;
  else {
  mu__boolexpr41 = ((mu_Request) == (mu_RDX_RAC)) ; 
}
              if (mu__boolexpr41) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 69;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted.");
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO in remote cluster");
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
if ( (mu_Src) == (mu_Aux) )
{
mu_Send_R_Ex_Reply ( mu_Aux, mu_Dst, mu_Src, mu_Addr, mu_Cache.mu_Value, 0 );
}
else
{
mu_Send_R_Ex_Reply ( mu_Aux, mu_Dst, mu_Src, mu_Addr, mu_Cache.mu_Value, 1 );
mu_Send_Dirty_Transfer_Req ( mu_Src, mu_Dst, mu_Aux, mu_Addr );
}
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase9 ********************/
class RuleBase9
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle Invalidate request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr42;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr42 = FALSE ;
  else {
  mu__boolexpr42 = ((mu_Request) == (mu_INV)) ; 
}
  return mu__boolexpr42;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 78;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 87 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr43;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr43 = FALSE ;
  else {
  mu__boolexpr43 = ((mu_Request) == (mu_INV)) ; 
}
              if (mu__boolexpr43) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 78;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
if ( !((mu_Dst) != (mu_Src)) ) Error.Error("Assertion failed: Invalidation to Local Memory");
if ( (mu_Dst) == (mu_Aux) )
{
if ( (mu_Cache.mu_State) == (mu_Locally_Shared) )
{
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
}
if ( (mu_RAC.mu_State) == (mu_WINV) )
{
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) - (1);
}
else
{
mu_RAC.mu_InvCount = (mu_RAC.mu_InvCount) + (1);
}
bool mu__boolexpr44;
  if (!((mu_RAC.mu_InvCount) == (0))) mu__boolexpr44 = FALSE ;
  else {
  mu__boolexpr44 = ((mu_RAC.mu_State) == (mu_WINV)) ; 
}
if ( mu__boolexpr44 )
{
mu_RAC.undefine();
mu_RAC.mu_State = mu_INVAL;
mu_RAC.mu_InvCount = 0;
}
mu_Consume_Request ( mu_Src, mu_Dst );
}
else
{
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
Error.Error("Error: invalidation cannot be for this copy!");
break;
case mu_WRD:
mu_RAC.mu_State = mu_RRD;
break;
case mu_RRD:
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: Inconsistent RAC with invalidation");
switch ((int) mu_Cache.mu_State) {
case mu_Non_Locally_Cached:
break;
case mu_Locally_Shared:
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
break;
case mu_Locally_Exmod:
Error.Error("Error: Invalidate request to master remote block.");
break;
}
break;
}
mu_Send_Inv_Ack ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase10 ********************/
class RuleBase10
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle read request to remote cluster, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr45;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr45 = FALSE ;
  else {
  mu__boolexpr45 = ((mu_Request) == (mu_RD_RAC)) ; 
}
  return mu__boolexpr45;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 87;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 96 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr46;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr46 = FALSE ;
  else {
  mu__boolexpr46 = ((mu_Request) == (mu_RD_RAC)) ; 
}
              if (mu__boolexpr46) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 87;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Src][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Src][mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted.");
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO at remote cluster");
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Cache.mu_State = mu_Locally_Shared;
if ( (mu_Src) == (mu_Aux) )
{
mu_Send_R_Reply ( mu_Aux, mu_Dst, mu_Src, mu_Addr, mu_Cache.mu_Value );
}
else
{
mu_Send_R_Reply ( mu_Aux, mu_Dst, mu_Src, mu_Addr, mu_Cache.mu_Value );
mu_Send_SH_WB_Req ( mu_Src, mu_Dst, mu_Aux, mu_Addr, mu_Cache.mu_Value );
}
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
mu_Send_NAK ( mu_Aux, mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase11 ********************/
class RuleBase11
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle DMA write request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr47;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr47 = FALSE ;
  else {
  mu__boolexpr47 = ((mu_Request) == (mu_DWR_H)) ; 
}
  return mu__boolexpr47;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 96;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 105 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr48;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr48 = FALSE ;
  else {
  mu__boolexpr48 = ((mu_Request) == (mu_DWR_H)) ; 
}
              if (mu__boolexpr48) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 96;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1_Value& mu_v = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Dst][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Dst][mu_Addr];
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
mu_Send_NAK ( mu_Src, mu_Dst, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Cache.mu_Value = mu_v;
break;
case mu_Locally_Shared:
mu_Cache.mu_Value = mu_v;
mu_Mem = mu_v;
break;
case mu_Non_Locally_Cached:
mu_Mem = mu_v;
break;
}
mu_Send_DMA_W_Ack ( mu_Src, mu_Dst, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Shared_Remote:
if ( (mu_Cache.mu_State) == (mu_Locally_Shared) )
{
mu_Cache.mu_Value = mu_v;
}
mu_Mem = mu_v;
{
for(int mu_i = 0; mu_i <= 2; mu_i++) {
if ( (mu_i) < (mu_Dir.mu_SharedCount) )
{
mu_Send_DMA_Update_Req ( mu_Dir.mu_Entries[mu_i], mu_Dst, mu_Src, mu_Addr, mu_v );
}
};
};
mu_Send_DMA_Update_Count ( mu_Src, mu_Dst, mu_Addr, (int)mu_Dir.mu_SharedCount );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Dirty_Remote:
mu_Send_DMA_W_Req_RAC ( mu_Dir.mu_Entries[0], mu_Dst, mu_Src, mu_Addr, mu_v );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase12 ********************/
class RuleBase12
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle DMA read request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr49;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr49 = FALSE ;
  else {
  mu__boolexpr49 = ((mu_Request) == (mu_DRD_H)) ; 
}
  return mu__boolexpr49;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 105;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 114 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr50;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr50 = FALSE ;
  else {
  mu__boolexpr50 = ((mu_Request) == (mu_DRD_H)) ; 
}
              if (mu__boolexpr50) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 105;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Dst][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Dst][mu_Addr];
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
mu_Send_NAK ( mu_Src, mu_Dst, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
if ( (mu_Cache.mu_State) == (mu_Locally_Exmod) )
{
mu_Send_DMA_R_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Cache.mu_Value );
}
else
{
mu_Send_DMA_R_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem );
}
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Shared_Remote:
mu_Send_DMA_R_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Dirty_Remote:
mu_Send_DMA_R_Req_RAC ( mu_Dir.mu_Entries[0], mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase13 ********************/
class RuleBase13
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle writeback request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr51;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr51 = FALSE ;
  else {
  mu__boolexpr51 = ((mu_Request) == (mu_WB)) ; 
}
  return mu__boolexpr51;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 114;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 123 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr52;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr52 = FALSE ;
  else {
  mu__boolexpr52 = ((mu_Request) == (mu_WB)) ; 
}
              if (mu__boolexpr52) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 114;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1_Value& mu_v = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
if ( !((mu_Dir.mu_State) == (mu_Dirty_Remote)) ) Error.Error("Assertion failed: Explicit writeback for non dirty remote");
if ( !((mu_Dir.mu_Entries[0]) == (mu_Src)) ) Error.Error("Assertion failed: Explicit writeback by non owner");
mu_Mem = mu_v;
mu_Dir.mu_State = mu_Uncached;
mu_Dir.mu_SharedCount = 0;
mu_Dir.mu_Entries.undefine();
mu_Consume_Request ( mu_Src, mu_Dst );
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase14 ********************/
class RuleBase14
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle dirty transfer request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr53;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr53 = FALSE ;
  else {
  mu__boolexpr53 = ((mu_Request) == (mu_DXFER)) ; 
}
  return mu__boolexpr53;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 123;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 132 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr54;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr54 = FALSE ;
  else {
  mu__boolexpr54 = ((mu_Request) == (mu_DXFER)) ; 
}
              if (mu__boolexpr54) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 123;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
if ( !((mu_Dir.mu_State) == (mu_Dirty_Remote)) ) Error.Error("Assertion failed: Dirty transfer for non dirty remote memory");
if ( !((mu_Dir.mu_Entries[0]) == (mu_Src)) ) Error.Error("Assertion failed: Dirty transfer by non owner");
mu_Set_Dir_1st_Entry ( (int)mu_Dst, mu_Addr, mu_Aux );
mu_Send_Dirty_Transfer_Ack ( mu_Aux, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase15 ********************/
class RuleBase15
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle Sharing writeback request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr55;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr55 = FALSE ;
  else {
  mu__boolexpr55 = ((mu_Request) == (mu_SHWB)) ; 
}
  return mu__boolexpr55;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 132;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 141 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr56;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr56 = FALSE ;
  else {
  mu__boolexpr56 = ((mu_Request) == (mu_SHWB)) ; 
}
              if (mu__boolexpr56) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 132;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1_Value& mu_v = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Value;
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
if ( !((mu_Dir.mu_State) == (mu_Dirty_Remote)) ) Error.Error("Assertion failed: Writeback to non dirty remote memory");
if ( !((mu_Dir.mu_Entries[0]) == (mu_Src)) ) Error.Error("Assertion failed: Writeback by non owner");
mu_Mem = mu_v;
mu_Dir.mu_State = mu_Shared_Remote;
mu_Add_to_Dir_Entries ( (int)mu_Dst, mu_Addr, mu_Aux );
mu_Consume_Request ( mu_Src, mu_Dst );
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase16 ********************/
class RuleBase16
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle read exclusive request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr57;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr57 = FALSE ;
  else {
  mu__boolexpr57 = ((mu_Request) == (mu_RDX_H)) ; 
}
  return mu__boolexpr57;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 141;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 150 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr58;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr58 = FALSE ;
  else {
  mu__boolexpr58 = ((mu_Request) == (mu_RDX_H)) ; 
}
              if (mu__boolexpr58) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 141;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Dst][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Dst][mu_Addr];
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
mu_Send_NAK ( mu_Src, mu_Dst, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRD)) ) Error.Error("Assertion failed: WRD at home cluster");
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
if ( (mu_Cache.mu_State) == (mu_Locally_Exmod) )
{
mu_Mem = mu_Cache.mu_Value;
}
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
mu_Dir.mu_State = mu_Dirty_Remote;
mu_Dir.mu_SharedCount = 1;
mu_Set_Dir_1st_Entry ( (int)mu_Dst, mu_Addr, mu_Src );
mu_Send_R_Ex_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem, 0 );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Shared_Remote:
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
if ( !(mu_bug1) )
{
{
for(int mu_i = 0; mu_i <= 2; mu_i++) {
if ( (mu_i) < (mu_Dir.mu_SharedCount) )
{
mu_Send_Inv_Req ( mu_Dir.mu_Entries[mu_i], mu_Dst, mu_Src, mu_Addr );
}
};
};
mu_Send_R_Ex_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem, (int)mu_Dir.mu_SharedCount );
}
else
{
{
for(int mu_i = 0; mu_i <= 1; mu_i++) {
bool mu__boolexpr59;
  if (!((mu_i) < (mu_Dir.mu_SharedCount))) mu__boolexpr59 = FALSE ;
  else {
  mu__boolexpr59 = ((mu_Dir.mu_Entries[mu_i]) != (mu_Src)) ; 
}
if ( mu__boolexpr59 )
{
mu_Send_Inv_Req ( mu_Dir.mu_Entries[mu_i], mu_Dst, mu_Src, mu_Addr );
}
};
};
bool mu__quant60; 
mu__quant60 = FALSE;
{
for(int mu_i = 0; mu_i <= 1; mu_i++) {
bool mu__boolexpr61;
  if (!((mu_i) < (mu_Dir.mu_SharedCount))) mu__boolexpr61 = FALSE ;
  else {
  mu__boolexpr61 = ((mu_Dir.mu_Entries[mu_i]) == (mu_Src)) ; 
}
if ( (mu__boolexpr61) )
  { mu__quant60 = TRUE; break; }
};
};
if ( mu__quant60 )
{
mu_Send_R_Ex_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem, (mu_Dir.mu_SharedCount) - (1) );
}
else
{
mu_Send_R_Ex_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem, (int)mu_Dir.mu_SharedCount );
}
}
mu_Dir.mu_State = mu_Dirty_Remote;
mu_Dir.mu_SharedCount = 1;
mu_Set_Dir_1st_Entry ( (int)mu_Dst, mu_Addr, mu_Src );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Dirty_Remote:
mu_Send_R_Ex_Req_RAC ( mu_Dir.mu_Entries[0], mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase17 ********************/
class RuleBase17
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    return tsprintf("handle read request to home, Src:%s, Dst:%s", mu_Src.Name(), mu_Dst.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr62;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr62 = FALSE ;
  else {
  mu__boolexpr62 = ((mu_Request) == (mu_RD_H)) ; 
}
  return mu__boolexpr62;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 150;
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 159 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
bool mu__boolexpr63;
  if (!((mu_ReqChan.mu_Count) > (0))) mu__boolexpr63 = FALSE ;
  else {
  mu__boolexpr63 = ((mu_Request) == (mu_RD_H)) ; 
}
              if (mu__boolexpr63) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 150;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Proc mu_Src;
    mu_Src.unionassign(r % 3);
    r = r / 3;
    static mu_1_Proc mu_Dst;
    mu_Dst.unionassign(r % 3);
    r = r / 3;
  mu_1__type_14& mu_ReqChan = mu_ReqNet[mu_Src][mu_Dst];
  mu_1_RequestType& mu_Request = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Mtype;
  mu_1_Address& mu_Addr = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Addr;
  mu_1_Proc& mu_Aux = mu_ReqNet[mu_Src][mu_Dst].mu_Messages[0].mu_Aux;
{
  mu_1__type_9& mu_RAC = mu_Procs[mu_Dst].mu_RAC[mu_Dst][mu_Addr];
  mu_1__type_6& mu_Cache = mu_Procs[mu_Dst].mu_Cache[mu_Dst][mu_Addr];
  mu_1__type_4& mu_Dir = mu_Homes[mu_Dst].mu_Dir[mu_Addr];
  mu_1_Value& mu_Mem = mu_Homes[mu_Dst].mu_Mem[mu_Addr];
switch ((int) mu_RAC.mu_State) {
case mu_WINV:
mu_Send_NAK ( mu_Src, mu_Dst, mu_Dst, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRD)) ) Error.Error("Assertion failed: WRD at home cluster");
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
if ( (mu_Cache.mu_State) == (mu_Locally_Exmod) )
{
mu_Cache.mu_State = mu_Locally_Shared;
mu_Mem = mu_Cache.mu_Value;
}
mu_Dir.mu_State = mu_Shared_Remote;
mu_Dir.mu_SharedCount = 1;
mu_Set_Dir_1st_Entry ( (int)mu_Dst, mu_Addr, mu_Src );
mu_Send_R_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Shared_Remote:
mu_Add_to_Dir_Entries ( (int)mu_Dst, mu_Addr, mu_Src );
mu_Send_R_Reply ( mu_Src, mu_Dst, mu_Dst, mu_Addr, mu_Mem );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
case mu_Dirty_Remote:
mu_Send_R_Req_RAC ( mu_Dir.mu_Entries[0], mu_Dst, mu_Src, mu_Addr );
mu_Consume_Request ( mu_Src, mu_Dst );
break;
}
break;
}
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase18 ********************/
class RuleBase18
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("DMA Remote Memory Write Request, a:%s, v:%s, h:%s, n:%s", mu_a.Name(), mu_v.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  return (mu_h) != (mu_n);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 159;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 165 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
              if ((mu_h) != (mu_n)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 2;
        }
        else
          what_rule += 2;
    r = what_rule - 159;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Cache.mu_Value = mu_v;
break;
case mu_Locally_Shared:
mu_RAC.mu_State = mu_WDMAW;
mu_RAC.mu_Value = mu_v;
mu_Send_DMA_W_Req_H ( (int)mu_h, mu_n, mu_a, mu_v );
break;
case mu_Non_Locally_Cached:
mu_RAC.mu_State = mu_WDMAW;
mu_RAC.mu_Value = mu_v;
mu_Send_DMA_W_Req_H ( (int)mu_h, mu_n, mu_a, mu_v );
break;
}
break;
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
mu_Cache.mu_Value = mu_v;
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO at remote cluster");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase19 ********************/
class RuleBase19
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("DMA Remote Memory Read Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  return (mu_h) != (mu_n);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 165;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 168 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
              if ((mu_h) != (mu_n)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 165;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
switch ((int) mu_Cache.mu_State) {
case mu_Non_Locally_Cached:
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
mu_RAC.mu_State = mu_WDMAR;
mu_Send_DMA_R_Req_H ( (int)mu_h, mu_n, mu_a );
break;
case mu_WINV:
Error.Error("Error: Inconsistent RAC and Cache");
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRDO)) ) Error.Error("Assertion failed: WRDO at remote cluster");
break;
}
break;
case mu_Locally_Shared:
bool mu__boolexpr64;
bool mu__boolexpr65;
bool mu__boolexpr66;
bool mu__boolexpr67;
  if (!((mu_RAC.mu_State) != (mu_WINV))) mu__boolexpr67 = FALSE ;
  else {
  mu__boolexpr67 = ((mu_RAC.mu_State) != (mu_WRDO)) ; 
}
  if (!(mu__boolexpr67)) mu__boolexpr66 = FALSE ;
  else {
  mu__boolexpr66 = ((mu_RAC.mu_State) != (mu_WRD)) ; 
}
  if (!(mu__boolexpr66)) mu__boolexpr65 = FALSE ;
  else {
  mu__boolexpr65 = ((mu_RAC.mu_State) != (mu_RRD)) ; 
}
  if (!(mu__boolexpr65)) mu__boolexpr64 = FALSE ;
  else {
  mu__boolexpr64 = ((mu_RAC.mu_State) != (mu_WDMAR)) ; 
}
if ( !(mu__boolexpr64) ) Error.Error("Assertion failed: Inconsistent directory");
break;
case mu_Locally_Exmod:
bool mu__boolexpr68;
bool mu__boolexpr69;
bool mu__boolexpr70;
bool mu__boolexpr71;
bool mu__boolexpr72;
  if (!((mu_RAC.mu_State) != (mu_WDMAW))) mu__boolexpr72 = FALSE ;
  else {
  mu__boolexpr72 = ((mu_RAC.mu_State) != (mu_WRDO)) ; 
}
  if (!(mu__boolexpr72)) mu__boolexpr71 = FALSE ;
  else {
  mu__boolexpr71 = ((mu_RAC.mu_State) != (mu_WRD)) ; 
}
  if (!(mu__boolexpr71)) mu__boolexpr70 = FALSE ;
  else {
  mu__boolexpr70 = ((mu_RAC.mu_State) != (mu_WRDX)) ; 
}
  if (!(mu__boolexpr70)) mu__boolexpr69 = FALSE ;
  else {
  mu__boolexpr69 = ((mu_RAC.mu_State) != (mu_RRD)) ; 
}
  if (!(mu__boolexpr69)) mu__boolexpr68 = FALSE ;
  else {
  mu__boolexpr68 = ((mu_RAC.mu_State) != (mu_WDMAR)) ; 
}
if ( !(mu__boolexpr68) ) Error.Error("Assertion failed: Inconsistent directory");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase20 ********************/
class RuleBase20
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("Explicit Writeback request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
bool mu__boolexpr73;
  if (!((mu_h) != (mu_n))) mu__boolexpr73 = FALSE ;
  else {
  mu__boolexpr73 = ((mu_Cache.mu_State) == (mu_Locally_Exmod)) ; 
}
  return mu__boolexpr73;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 168;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 171 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
bool mu__boolexpr74;
  if (!((mu_h) != (mu_n))) mu__boolexpr74 = FALSE ;
  else {
  mu__boolexpr74 = ((mu_Cache.mu_State) == (mu_Locally_Exmod)) ; 
}
              if (mu__boolexpr74) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 168;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
if ( (mu_RAC.mu_State) == (mu_WINV) )
{
}
else
{
bool mu__boolexpr75;
  if ((mu_RAC.mu_State) == (mu_INVAL)) mu__boolexpr75 = TRUE ;
  else {
  mu__boolexpr75 = ((mu_RAC.mu_State) == (mu_WUP)) ; 
}
if ( !(mu__boolexpr75) ) Error.Error("Assertion failed: Inconsistent Directory");
mu_Send_WB_Req ( (int)mu_h, mu_n, mu_a, mu_Cache.mu_Value );
mu_Cache.mu_State = mu_Non_Locally_Cached;
mu_Cache.mu_Value.undefine();
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase21 ********************/
class RuleBase21
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("Remote Memory Read Exclusive Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  return (mu_h) != (mu_n);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 171;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 174 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
              if ((mu_h) != (mu_n)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 171;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
break;
default:
mu_RAC.mu_State = mu_WRDX;
mu_Send_R_Ex_Req_H ( (int)mu_h, mu_n, mu_a );
break;
}
break;
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
break;
case mu_WRDX:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WRDX with Exmod asserted");
break;
case mu_WRD:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WRD with Exmod asserted");
break;
case mu_RRD:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: RRD with Exmod asserted");
break;
case mu_WRDO:
Error.Error("Error: remote cluster with WRDO");
break;
case mu_WDMAR:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WRD with Exmod asserted");
break;
case mu_WUP:
break;
case mu_WDMAW:
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase22 ********************/
class RuleBase22
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("Remote Memory Read Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  return (mu_h) != (mu_n);
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 174;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 177 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
              if ((mu_h) != (mu_n)) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 174;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Cache.mu_State) {
case mu_Non_Locally_Cached:
mu_RAC.mu_State = mu_WRD;
mu_Send_R_Req_H ( (int)mu_h, mu_n, mu_a );
break;
}
break;
case mu_WINV:
if ( !((mu_Cache.mu_State) == (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WINV with Exmod not asserted");
break;
case mu_WRD:
if ( !((mu_Cache.mu_State) == (mu_Non_Locally_Cached)) ) Error.Error("Assertion failed: WRD with data Locally_Cached");
break;
case mu_WRDX:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WRDX with data Locally_Exmod");
break;
case mu_RRD:
if ( !((mu_Cache.mu_State) == (mu_Non_Locally_Cached)) ) Error.Error("Assertion failed: WRDX with funny cache state");
break;
case mu_WRDO:
Error.Error("Error: remote Cluster with WRDO");
break;
case mu_WDMAR:
if ( !((mu_Cache.mu_State) == (mu_Non_Locally_Cached)) ) Error.Error("Assertion failed: WRD with data Locally_Cached");
break;
case mu_WUP:
break;
case mu_WDMAW:
if ( !((mu_Cache.mu_State) != (mu_Locally_Exmod)) ) Error.Error("Assertion failed: WRDX with data Locally_Exmod");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase23 ********************/
class RuleBase23
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("DMA Local Memory Write Request, a:%s, v:%s, h:%s, n:%s", mu_a.Name(), mu_v.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr76;
  if (!((mu_h) == (mu_n))) mu__boolexpr76 = FALSE ;
  else {
  mu__boolexpr76 = (!(mu_nohome)) ; 
}
  return mu__boolexpr76;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 177;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 183 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr77;
  if (!((mu_h) == (mu_n))) mu__boolexpr77 = FALSE ;
  else {
  mu__boolexpr77 = (!(mu_nohome)) ; 
}
              if (mu__boolexpr77) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 2;
        }
        else
          what_rule += 2;
    r = what_rule - 177;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
switch ((int) mu_Cache.mu_State) {
case mu_Non_Locally_Cached:
mu_Mem = mu_v;
break;
case mu_Locally_Shared:
mu_Cache.mu_Value = mu_v;
mu_Mem = mu_v;
break;
case mu_Locally_Exmod:
mu_Cache.mu_Value = mu_v;
break;
}
break;
case mu_Shared_Remote:
switch ((int) mu_Cache.mu_State) {
case mu_Non_Locally_Cached:
break;
case mu_Locally_Shared:
mu_Cache.mu_Value = mu_v;
break;
case mu_Locally_Exmod:
Error.Error("Error: Shared_remote with Exmod asserted");
break;
}
mu_Mem = mu_v;
mu_RAC.mu_State = mu_WUP;
mu_RAC.mu_InvCount = mu_Dir.mu_SharedCount;
{
for(int mu_i = 0; mu_i <= 2; mu_i++) {
if ( (mu_i) < (mu_Dir.mu_SharedCount) )
{
mu_Send_DMA_Update_Req ( mu_Dir.mu_Entries[mu_i], mu_n, mu_n, mu_a, mu_v );
}
};
};
break;
case mu_Dirty_Remote:
mu_RAC.mu_State = mu_WDMAW;
mu_RAC.mu_Value = mu_v;
mu_Send_DMA_W_Req_RAC ( mu_Dir.mu_Entries[0], mu_n, mu_n, mu_a, mu_v );
break;
}
break;
case mu_WINV:
if ( (mu_Cache.mu_State) == (mu_Locally_Exmod) )
{
mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_Value = mu_v;
}
else
{
if ( (mu_Cache.mu_State) == (mu_Locally_Shared) )
{
mu_Cache.mu_Value = mu_v;
mu_Mem = mu_v;
}
}
break;
default:
if ( !((mu_RAC.mu_State) != (mu_WRD)) ) Error.Error("Assertion failed: WRD at home cluster");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase24 ********************/
class RuleBase24
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("DMA Local Memory Read Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr78;
  if (!((mu_h) == (mu_n))) mu__boolexpr78 = FALSE ;
  else {
  mu__boolexpr78 = (!(mu_nohome)) ; 
}
  return mu__boolexpr78;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 183;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 186 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr79;
  if (!((mu_h) == (mu_n))) mu__boolexpr79 = FALSE ;
  else {
  mu__boolexpr79 = (!(mu_nohome)) ; 
}
              if (mu__boolexpr79) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 183;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
break;
case mu_Shared_Remote:
break;
case mu_Dirty_Remote:
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
mu_RAC.mu_State = mu_WDMAR;
mu_Send_DMA_R_Req_RAC ( (int)mu_h, mu_n, mu_n, mu_a );
break;
case mu_WINV:
Error.Error("Error: inconsistent directory");
break;
case mu_WDMAR:
break;
default:
bool mu__boolexpr80;
  if (!((mu_RAC.mu_State) != (mu_WRD))) mu__boolexpr80 = FALSE ;
  else {
  mu__boolexpr80 = ((mu_RAC.mu_State) != (mu_RRD)) ; 
}
if ( !(mu__boolexpr80) ) Error.Error("Assertion failed: Funny RAC state at home cluster");
break;
}
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase25 ********************/
class RuleBase25
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("Local Memory Read Exclusive Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr81;
  if (!((mu_h) == (mu_n))) mu__boolexpr81 = FALSE ;
  else {
  mu__boolexpr81 = (!(mu_nohome)) ; 
}
  return mu__boolexpr81;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 186;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 189 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr82;
  if (!((mu_h) == (mu_n))) mu__boolexpr82 = FALSE ;
  else {
  mu__boolexpr82 = (!(mu_nohome)) ; 
}
              if (mu__boolexpr82) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 186;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
mu_Cache.mu_State = mu_Locally_Exmod;
mu_Cache.mu_Value = mu_Mem;
break;
case mu_Shared_Remote:
mu_Cache.mu_State = mu_Locally_Exmod;
mu_Cache.mu_Value = mu_Mem;
mu_RAC.mu_State = mu_WINV;
mu_RAC.mu_InvCount = mu_Dir.mu_SharedCount;
{
for(int mu_i = 0; mu_i <= 2; mu_i++) {
if ( (mu_i) < (mu_RAC.mu_InvCount) )
{
mu_Send_Inv_Req ( mu_Dir.mu_Entries[mu_i], (int)mu_h, (int)mu_h, mu_a );
}
};
};
mu_Dir.mu_State = mu_Uncached;
mu_Dir.mu_SharedCount = 0;
mu_Dir.mu_Entries.undefine();
break;
case mu_Dirty_Remote:
mu_RAC.mu_State = mu_WRDX;
mu_Send_R_Ex_Req_RAC ( mu_Dir.mu_Entries[0], (int)mu_h, (int)mu_h, mu_a );
break;
}
break;
case mu_WINV:
if ( !((mu_Dir.mu_State) == (mu_Uncached)) ) Error.Error("Assertion failed: Inconsistent Directory");
break;
case mu_WRDX:
switch ((int) mu_Dir.mu_State) {
case mu_Uncached:
break;
case mu_Shared_Remote:
break;
case mu_Dirty_Remote:
break;
}
break;
case mu_WRDO:
break;
default:
bool mu__boolexpr83;
  if (!((mu_RAC.mu_State) != (mu_WRD))) mu__boolexpr83 = FALSE ;
  else {
  mu__boolexpr83 = ((mu_RAC.mu_State) != (mu_RRD)) ; 
}
if ( !(mu__boolexpr83) ) Error.Error("Assertion failed: Funny RAC state at home cluster");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
/******************** RuleBase26 ********************/
class RuleBase26
{
public:
  int Priority()
  {
    return 0;
  }
  char * Name(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    return tsprintf("Local Memory Read Request, a:%s, h:%s, n:%s", mu_a.Name(), mu_h.Name(), mu_n.Name());
  }
  bool Condition(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr84;
  if (!((mu_h) == (mu_n))) mu__boolexpr84 = FALSE ;
  else {
  mu__boolexpr84 = (!(mu_nohome)) ; 
}
  return mu__boolexpr84;
  }

  void NextRule(unsigned & what_rule)
  {
    unsigned r = what_rule - 189;
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
    while (what_rule < 192 )
      {
        if ( ( TRUE  ) ) {
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
bool mu__boolexpr85;
  if (!((mu_h) == (mu_n))) mu__boolexpr85 = FALSE ;
  else {
  mu__boolexpr85 = (!(mu_nohome)) ; 
}
              if (mu__boolexpr85) {
                if ( ( TRUE  ) )
                  return;
                else
                  what_rule++;
              }
              else
                what_rule += 1;
        }
        else
          what_rule += 1;
    r = what_rule - 189;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    mu_n.unionassign(r % 3);
    r = r / 3;
    }
  }

  void Code(unsigned r)
  {
    static mu_1_Address mu_a;
    mu_a.value((r % 1) + 4);
    r = r / 1;
    static mu_1_Home mu_h;
    mu_h.value((r % 1) + 1);
    r = r / 1;
    static mu_1_Proc mu_n;
    mu_n.unionassign(r % 3);
    r = r / 3;
  mu_1__type_9& mu_RAC = mu_Procs[mu_n].mu_RAC[mu_h][mu_a];
  mu_1__type_6& mu_Cache = mu_Procs[mu_n].mu_Cache[mu_h][mu_a];
  mu_1__type_4& mu_Dir = mu_Homes[mu_h].mu_Dir[mu_a];
  mu_1_Value& mu_Mem = mu_Homes[mu_h].mu_Mem[mu_a];
switch ((int) mu_RAC.mu_State) {
case mu_INVAL:
switch ((int) mu_Dir.mu_State) {
case mu_Dirty_Remote:
mu_RAC.mu_State = mu_WRDO;
mu_Send_R_Req_RAC ( mu_Dir.mu_Entries[0], (int)mu_h, (int)mu_h, mu_a );
break;
default:
switch ((int) mu_Cache.mu_State) {
case mu_Locally_Exmod:
mu_Cache.mu_State = mu_Locally_Shared;
mu_Mem = mu_Cache.mu_Value;
break;
case mu_Locally_Shared:
break;
case mu_Non_Locally_Cached:
mu_Cache.mu_State = mu_Locally_Shared;
mu_Cache.mu_Value = mu_Mem;
break;
}
break;
}
break;
case mu_WRDO:
break;
default:
bool mu__boolexpr86;
  if (!((mu_RAC.mu_State) != (mu_WRD))) mu__boolexpr86 = FALSE ;
  else {
  mu__boolexpr86 = ((mu_RAC.mu_State) != (mu_RRD)) ; 
}
if ( !(mu__boolexpr86) ) Error.Error("Assertion failed: Funny RAC state at home cluster");
break;
}
  };

  bool UnFair()
  { return FALSE; }
};
class NextStateGenerator
{
  RuleBase0 R0;
  RuleBase1 R1;
  RuleBase2 R2;
  RuleBase3 R3;
  RuleBase4 R4;
  RuleBase5 R5;
  RuleBase6 R6;
  RuleBase7 R7;
  RuleBase8 R8;
  RuleBase9 R9;
  RuleBase10 R10;
  RuleBase11 R11;
  RuleBase12 R12;
  RuleBase13 R13;
  RuleBase14 R14;
  RuleBase15 R15;
  RuleBase16 R16;
  RuleBase17 R17;
  RuleBase18 R18;
  RuleBase19 R19;
  RuleBase20 R20;
  RuleBase21 R21;
  RuleBase22 R22;
  RuleBase23 R23;
  RuleBase24 R24;
  RuleBase25 R25;
  RuleBase26 R26;
public:
void SetNextEnabledRule(unsigned & what_rule)
{
  category = CONDITION;
  if (what_rule<6)
    { R0.NextRule(what_rule);
      if (what_rule<6) return; }
  if (what_rule>=6 && what_rule<15)
    { R1.NextRule(what_rule);
      if (what_rule<15) return; }
  if (what_rule>=15 && what_rule<24)
    { R2.NextRule(what_rule);
      if (what_rule<24) return; }
  if (what_rule>=24 && what_rule<33)
    { R3.NextRule(what_rule);
      if (what_rule<33) return; }
  if (what_rule>=33 && what_rule<42)
    { R4.NextRule(what_rule);
      if (what_rule<42) return; }
  if (what_rule>=42 && what_rule<51)
    { R5.NextRule(what_rule);
      if (what_rule<51) return; }
  if (what_rule>=51 && what_rule<60)
    { R6.NextRule(what_rule);
      if (what_rule<60) return; }
  if (what_rule>=60 && what_rule<69)
    { R7.NextRule(what_rule);
      if (what_rule<69) return; }
  if (what_rule>=69 && what_rule<78)
    { R8.NextRule(what_rule);
      if (what_rule<78) return; }
  if (what_rule>=78 && what_rule<87)
    { R9.NextRule(what_rule);
      if (what_rule<87) return; }
  if (what_rule>=87 && what_rule<96)
    { R10.NextRule(what_rule);
      if (what_rule<96) return; }
  if (what_rule>=96 && what_rule<105)
    { R11.NextRule(what_rule);
      if (what_rule<105) return; }
  if (what_rule>=105 && what_rule<114)
    { R12.NextRule(what_rule);
      if (what_rule<114) return; }
  if (what_rule>=114 && what_rule<123)
    { R13.NextRule(what_rule);
      if (what_rule<123) return; }
  if (what_rule>=123 && what_rule<132)
    { R14.NextRule(what_rule);
      if (what_rule<132) return; }
  if (what_rule>=132 && what_rule<141)
    { R15.NextRule(what_rule);
      if (what_rule<141) return; }
  if (what_rule>=141 && what_rule<150)
    { R16.NextRule(what_rule);
      if (what_rule<150) return; }
  if (what_rule>=150 && what_rule<159)
    { R17.NextRule(what_rule);
      if (what_rule<159) return; }
  if (what_rule>=159 && what_rule<165)
    { R18.NextRule(what_rule);
      if (what_rule<165) return; }
  if (what_rule>=165 && what_rule<168)
    { R19.NextRule(what_rule);
      if (what_rule<168) return; }
  if (what_rule>=168 && what_rule<171)
    { R20.NextRule(what_rule);
      if (what_rule<171) return; }
  if (what_rule>=171 && what_rule<174)
    { R21.NextRule(what_rule);
      if (what_rule<174) return; }
  if (what_rule>=174 && what_rule<177)
    { R22.NextRule(what_rule);
      if (what_rule<177) return; }
  if (what_rule>=177 && what_rule<183)
    { R23.NextRule(what_rule);
      if (what_rule<183) return; }
  if (what_rule>=183 && what_rule<186)
    { R24.NextRule(what_rule);
      if (what_rule<186) return; }
  if (what_rule>=186 && what_rule<189)
    { R25.NextRule(what_rule);
      if (what_rule<189) return; }
  if (what_rule>=189 && what_rule<192)
    { R26.NextRule(what_rule);
      if (what_rule<192) return; }
}
bool Condition(unsigned r)
{
  category = CONDITION;
  if (r<=5) return R0.Condition(r-0);
  if (r>=6 && r<=14) return R1.Condition(r-6);
  if (r>=15 && r<=23) return R2.Condition(r-15);
  if (r>=24 && r<=32) return R3.Condition(r-24);
  if (r>=33 && r<=41) return R4.Condition(r-33);
  if (r>=42 && r<=50) return R5.Condition(r-42);
  if (r>=51 && r<=59) return R6.Condition(r-51);
  if (r>=60 && r<=68) return R7.Condition(r-60);
  if (r>=69 && r<=77) return R8.Condition(r-69);
  if (r>=78 && r<=86) return R9.Condition(r-78);
  if (r>=87 && r<=95) return R10.Condition(r-87);
  if (r>=96 && r<=104) return R11.Condition(r-96);
  if (r>=105 && r<=113) return R12.Condition(r-105);
  if (r>=114 && r<=122) return R13.Condition(r-114);
  if (r>=123 && r<=131) return R14.Condition(r-123);
  if (r>=132 && r<=140) return R15.Condition(r-132);
  if (r>=141 && r<=149) return R16.Condition(r-141);
  if (r>=150 && r<=158) return R17.Condition(r-150);
  if (r>=159 && r<=164) return R18.Condition(r-159);
  if (r>=165 && r<=167) return R19.Condition(r-165);
  if (r>=168 && r<=170) return R20.Condition(r-168);
  if (r>=171 && r<=173) return R21.Condition(r-171);
  if (r>=174 && r<=176) return R22.Condition(r-174);
  if (r>=177 && r<=182) return R23.Condition(r-177);
  if (r>=183 && r<=185) return R24.Condition(r-183);
  if (r>=186 && r<=188) return R25.Condition(r-186);
  if (r>=189 && r<=191) return R26.Condition(r-189);
Error.Notrace("Internal: NextStateGenerator -- checking condition for nonexisting rule.");
}
void Code(unsigned r)
{
  if (r<=5) { R0.Code(r-0); return; } 
  if (r>=6 && r<=14) { R1.Code(r-6); return; } 
  if (r>=15 && r<=23) { R2.Code(r-15); return; } 
  if (r>=24 && r<=32) { R3.Code(r-24); return; } 
  if (r>=33 && r<=41) { R4.Code(r-33); return; } 
  if (r>=42 && r<=50) { R5.Code(r-42); return; } 
  if (r>=51 && r<=59) { R6.Code(r-51); return; } 
  if (r>=60 && r<=68) { R7.Code(r-60); return; } 
  if (r>=69 && r<=77) { R8.Code(r-69); return; } 
  if (r>=78 && r<=86) { R9.Code(r-78); return; } 
  if (r>=87 && r<=95) { R10.Code(r-87); return; } 
  if (r>=96 && r<=104) { R11.Code(r-96); return; } 
  if (r>=105 && r<=113) { R12.Code(r-105); return; } 
  if (r>=114 && r<=122) { R13.Code(r-114); return; } 
  if (r>=123 && r<=131) { R14.Code(r-123); return; } 
  if (r>=132 && r<=140) { R15.Code(r-132); return; } 
  if (r>=141 && r<=149) { R16.Code(r-141); return; } 
  if (r>=150 && r<=158) { R17.Code(r-150); return; } 
  if (r>=159 && r<=164) { R18.Code(r-159); return; } 
  if (r>=165 && r<=167) { R19.Code(r-165); return; } 
  if (r>=168 && r<=170) { R20.Code(r-168); return; } 
  if (r>=171 && r<=173) { R21.Code(r-171); return; } 
  if (r>=174 && r<=176) { R22.Code(r-174); return; } 
  if (r>=177 && r<=182) { R23.Code(r-177); return; } 
  if (r>=183 && r<=185) { R24.Code(r-183); return; } 
  if (r>=186 && r<=188) { R25.Code(r-186); return; } 
  if (r>=189 && r<=191) { R26.Code(r-189); return; } 
}
int Priority(unsigned short r)
{
  if (r<=5) { return R0.Priority(); } 
  if (r>=6 && r<=14) { return R1.Priority(); } 
  if (r>=15 && r<=23) { return R2.Priority(); } 
  if (r>=24 && r<=32) { return R3.Priority(); } 
  if (r>=33 && r<=41) { return R4.Priority(); } 
  if (r>=42 && r<=50) { return R5.Priority(); } 
  if (r>=51 && r<=59) { return R6.Priority(); } 
  if (r>=60 && r<=68) { return R7.Priority(); } 
  if (r>=69 && r<=77) { return R8.Priority(); } 
  if (r>=78 && r<=86) { return R9.Priority(); } 
  if (r>=87 && r<=95) { return R10.Priority(); } 
  if (r>=96 && r<=104) { return R11.Priority(); } 
  if (r>=105 && r<=113) { return R12.Priority(); } 
  if (r>=114 && r<=122) { return R13.Priority(); } 
  if (r>=123 && r<=131) { return R14.Priority(); } 
  if (r>=132 && r<=140) { return R15.Priority(); } 
  if (r>=141 && r<=149) { return R16.Priority(); } 
  if (r>=150 && r<=158) { return R17.Priority(); } 
  if (r>=159 && r<=164) { return R18.Priority(); } 
  if (r>=165 && r<=167) { return R19.Priority(); } 
  if (r>=168 && r<=170) { return R20.Priority(); } 
  if (r>=171 && r<=173) { return R21.Priority(); } 
  if (r>=174 && r<=176) { return R22.Priority(); } 
  if (r>=177 && r<=182) { return R23.Priority(); } 
  if (r>=183 && r<=185) { return R24.Priority(); } 
  if (r>=186 && r<=188) { return R25.Priority(); } 
  if (r>=189 && r<=191) { return R26.Priority(); } 
}
char * Name(unsigned r)
{
  if (r<=5) return R0.Name(r-0);
  if (r>=6 && r<=14) return R1.Name(r-6);
  if (r>=15 && r<=23) return R2.Name(r-15);
  if (r>=24 && r<=32) return R3.Name(r-24);
  if (r>=33 && r<=41) return R4.Name(r-33);
  if (r>=42 && r<=50) return R5.Name(r-42);
  if (r>=51 && r<=59) return R6.Name(r-51);
  if (r>=60 && r<=68) return R7.Name(r-60);
  if (r>=69 && r<=77) return R8.Name(r-69);
  if (r>=78 && r<=86) return R9.Name(r-78);
  if (r>=87 && r<=95) return R10.Name(r-87);
  if (r>=96 && r<=104) return R11.Name(r-96);
  if (r>=105 && r<=113) return R12.Name(r-105);
  if (r>=114 && r<=122) return R13.Name(r-114);
  if (r>=123 && r<=131) return R14.Name(r-123);
  if (r>=132 && r<=140) return R15.Name(r-132);
  if (r>=141 && r<=149) return R16.Name(r-141);
  if (r>=150 && r<=158) return R17.Name(r-150);
  if (r>=159 && r<=164) return R18.Name(r-159);
  if (r>=165 && r<=167) return R19.Name(r-165);
  if (r>=168 && r<=170) return R20.Name(r-168);
  if (r>=171 && r<=173) return R21.Name(r-171);
  if (r>=174 && r<=176) return R22.Name(r-174);
  if (r>=177 && r<=182) return R23.Name(r-177);
  if (r>=183 && r<=185) return R24.Name(r-183);
  if (r>=186 && r<=188) return R25.Name(r-186);
  if (r>=189 && r<=191) return R26.Name(r-189);
  return NULL;
}
};
const unsigned numrules = 192;

/********************
  parameter
 ********************/
#define RULES_IN_WORLD 192


/********************
  Startstate records
 ********************/
/******************** StartStateBase0 ********************/
class StartStateBase0
{
public:
  char * Name(unsigned short r)
  {
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
    return tsprintf("Startstate 0, v:%s", mu_v.Name());
  }
  void Code(unsigned short r)
  {
    static mu_1_Value mu_v;
    mu_v.value((r % 2) + 5);
    r = r / 2;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
mu_Homes[mu_h].mu_Dir[mu_a].mu_State = mu_Uncached;
mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount = 0;
mu_Homes[mu_h].mu_Mem[mu_a] = mu_v;
mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries.undefine();
};
};
};
};
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State = mu_Non_Locally_Cached;
mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State = mu_INVAL;
mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_Value.undefine();
mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_Value.undefine();
mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_InvCount = 0;
};
};
};
};
};
};
{
for(int mu_Src = 1; mu_Src <= 3; mu_Src++)
  if (( ( mu_Src >= 2 ) && ( mu_Src <= 3 ) )|| ( ( mu_Src >= 1 ) && ( mu_Src <= 1 ) )) {
{
for(int mu_Dst = 1; mu_Dst <= 3; mu_Dst++)
  if (( ( mu_Dst >= 2 ) && ( mu_Dst <= 3 ) )|| ( ( mu_Dst >= 1 ) && ( mu_Dst <= 1 ) )) {
mu_ReqNet[mu_Src][mu_Dst].mu_Count = 0;
mu_ReqNet[mu_Src][mu_Dst].mu_Messages.undefine();
mu_ReplyNet[mu_Src][mu_Dst].mu_Count = 0;
mu_ReplyNet[mu_Src][mu_Dst].mu_Messages.undefine();
};
};
};
};
  };

  bool UnFair()
  { return FALSE; }
};
class StartStateGenerator
{
  StartStateBase0 S0;
public:
void Code(unsigned short r)
{
  if (r<=1) { S0.Code(r-0); return; }
}
char * Name(unsigned short r)
{
  if (r<=1) return S0.Name(r-0);
  return NULL;
}
};
unsigned short StartStateManager::numstartstates = 2;

/********************
  Invariant records
 ********************/
int mu__invariant_87() // Invariant "Adequate invalidations with Read Exclusive request"
{
bool mu__quant88; 
mu__quant88 = TRUE;
{
for(int mu_n1 = 1; mu_n1 <= 3; mu_n1++)
  if (( ( mu_n1 >= 2 ) && ( mu_n1 <= 3 ) )|| ( ( mu_n1 >= 1 ) && ( mu_n1 <= 1 ) )) {
bool mu__quant89; 
mu__quant89 = TRUE;
{
for(int mu_n2 = 1; mu_n2 <= 3; mu_n2++)
  if (( ( mu_n2 >= 2 ) && ( mu_n2 <= 3 ) )|| ( ( mu_n2 >= 1 ) && ( mu_n2 <= 1 ) )) {
bool mu__quant90; 
mu__quant90 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant91; 
mu__quant91 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr92;
  if ((mu_n1) == (mu_n2)) mu__boolexpr92 = TRUE ;
  else {
bool mu__boolexpr93;
bool mu__boolexpr94;
  if (!((mu_Procs[mu_n1].mu_RAC[mu_h][mu_a].mu_State) == (mu_WINV))) mu__boolexpr94 = FALSE ;
  else {
  mu__boolexpr94 = ((mu_Procs[mu_n2].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Shared)) ; 
}
  if (!(mu__boolexpr94)) mu__boolexpr93 = FALSE ;
  else {
bool mu__quant95; 
mu__quant95 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr96;
  if (!((mu_i) < (mu_ReqNet[mu_h][mu_n2].mu_Count))) mu__boolexpr96 = FALSE ;
  else {
  mu__boolexpr96 = ((mu_ReqNet[mu_h][mu_n2].mu_Messages[mu_i].mu_Mtype) == (mu_INV)) ; 
}
if ( (mu__boolexpr96) )
  { mu__quant95 = TRUE; break; }
};
};
  mu__boolexpr93 = (!(mu__quant95)) ; 
}
  mu__boolexpr92 = (!(mu__boolexpr93)) ; 
}
if ( !(mu__boolexpr92) )
  { mu__quant91 = FALSE; break; }
};
};
if ( !(mu__quant91) )
  { mu__quant90 = FALSE; break; }
};
};
if ( !(mu__quant90) )
  { mu__quant89 = FALSE; break; }
};
};
if ( !(mu__quant89) )
  { mu__quant88 = FALSE; break; }
};
};
return mu__quant88;
};

bool mu__condition_97() // Condition for Rule "Adequate invalidations with Read Exclusive request"
{
  return mu__invariant_87( );
}

/**** end rule declaration ****/

int mu__invariant_98() // Invariant "Consistency of data"
{
bool mu__quant99; 
mu__quant99 = TRUE;
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
bool mu__quant100; 
mu__quant100 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant101; 
mu__quant101 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr102;
bool mu__boolexpr103;
bool mu__boolexpr104;
bool mu__boolexpr105;
bool mu__boolexpr106;
bool mu__boolexpr107;
bool mu__boolexpr108;
  if (!((mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Shared))) mu__boolexpr108 = FALSE ;
  else {
  mu__boolexpr108 = ((mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_Value) != (mu_Homes[mu_h].mu_Mem[mu_a])) ; 
}
  if (!(mu__boolexpr108)) mu__boolexpr107 = FALSE ;
  else {
  mu__boolexpr107 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_State) != (mu_Dirty_Remote)) ; 
}
  if (!(mu__boolexpr107)) mu__boolexpr106 = FALSE ;
  else {
bool mu__quant109; 
mu__quant109 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr110;
  if (!((mu_i) < (mu_ReqNet[mu_h][mu_n].mu_Count))) mu__boolexpr110 = FALSE ;
  else {
  mu__boolexpr110 = ((mu_ReqNet[mu_h][mu_n].mu_Messages[mu_i].mu_Mtype) == (mu_INV)) ; 
}
if ( (mu__boolexpr110) )
  { mu__quant109 = TRUE; break; }
};
};
  mu__boolexpr106 = (!(mu__quant109)) ; 
}
  if (!(mu__boolexpr106)) mu__boolexpr105 = FALSE ;
  else {
bool mu__boolexpr111;
bool mu__quant112; 
mu__quant112 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr113;
  if (!((mu_i) < (mu_ReqNet[mu_n][mu_h].mu_Count))) mu__boolexpr113 = FALSE ;
  else {
  mu__boolexpr113 = ((mu_ReqNet[mu_n][mu_h].mu_Messages[mu_i].mu_Mtype) == (mu_SHWB)) ; 
}
if ( (mu__boolexpr113) )
  { mu__quant112 = TRUE; break; }
};
};
  if (mu__quant112) mu__boolexpr111 = TRUE ;
  else {
bool mu__quant114; 
mu__quant114 = FALSE;
{
for(int mu_m = 1; mu_m <= 3; mu_m++)
  if (( ( mu_m >= 2 ) && ( mu_m <= 3 ) )|| ( ( mu_m >= 1 ) && ( mu_m <= 1 ) )) {
bool mu__quant115; 
mu__quant115 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr116;
bool mu__boolexpr117;
  if (!((mu_i) < (mu_ReqNet[mu_m][mu_h].mu_Count))) mu__boolexpr117 = FALSE ;
  else {
  mu__boolexpr117 = ((mu_ReqNet[mu_m][mu_h].mu_Messages[mu_i].mu_Mtype) == (mu_SHWB)) ; 
}
  if (!(mu__boolexpr117)) mu__boolexpr116 = FALSE ;
  else {
  mu__boolexpr116 = ((mu_ReqNet[mu_m][mu_h].mu_Messages[mu_i].mu_Aux) == (mu_n)) ; 
}
if ( (mu__boolexpr116) )
  { mu__quant115 = TRUE; break; }
};
};
if ( (mu__quant115) )
  { mu__quant114 = TRUE; break; }
};
};
  mu__boolexpr111 = (mu__quant114) ; 
}
  mu__boolexpr105 = (!(mu__boolexpr111)) ; 
}
  if (!(mu__boolexpr105)) mu__boolexpr104 = FALSE ;
  else {
bool mu__boolexpr118;
bool mu__quant119; 
mu__quant119 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr120;
  if (!((mu_i) < (mu_ReplyNet[mu_n][mu_h].mu_Count))) mu__boolexpr120 = FALSE ;
  else {
  mu__boolexpr120 = ((mu_ReplyNet[mu_n][mu_h].mu_Messages[mu_i].mu_Mtype) == (mu_ACK)) ; 
}
if ( (mu__boolexpr120) )
  { mu__quant119 = TRUE; break; }
};
};
  if (mu__quant119) mu__boolexpr118 = TRUE ;
  else {
bool mu__quant121; 
mu__quant121 = FALSE;
{
for(int mu_m = 1; mu_m <= 3; mu_m++)
  if (( ( mu_m >= 2 ) && ( mu_m <= 3 ) )|| ( ( mu_m >= 1 ) && ( mu_m <= 1 ) )) {
bool mu__quant122; 
mu__quant122 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr123;
bool mu__boolexpr124;
  if (!((mu_i) < (mu_ReplyNet[mu_n][mu_h].mu_Count))) mu__boolexpr124 = FALSE ;
  else {
  mu__boolexpr124 = ((mu_ReplyNet[mu_m][mu_h].mu_Messages[mu_i].mu_Mtype) == (mu_ACK)) ; 
}
  if (!(mu__boolexpr124)) mu__boolexpr123 = FALSE ;
  else {
  mu__boolexpr123 = ((mu_ReplyNet[mu_m][mu_h].mu_Messages[mu_i].mu_Aux) == (mu_n)) ; 
}
if ( (mu__boolexpr123) )
  { mu__quant122 = TRUE; break; }
};
};
if ( (mu__quant122) )
  { mu__quant121 = TRUE; break; }
};
};
  mu__boolexpr118 = (mu__quant121) ; 
}
  mu__boolexpr104 = (!(mu__boolexpr118)) ; 
}
  if (!(mu__boolexpr104)) mu__boolexpr103 = FALSE ;
  else {
  mu__boolexpr103 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) != (mu_WDMAW)) ; 
}
  if (!(mu__boolexpr103)) mu__boolexpr102 = FALSE ;
  else {
bool mu__quant125; 
mu__quant125 = FALSE;
{
for(int mu_i = 0; mu_i <= 5; mu_i++) {
bool mu__boolexpr126;
  if (!((mu_i) < (mu_ReqNet[mu_h][mu_n].mu_Count))) mu__boolexpr126 = FALSE ;
  else {
  mu__boolexpr126 = ((mu_ReqNet[mu_h][mu_n].mu_Messages[mu_i].mu_Mtype) == (mu_DUP)) ; 
}
if ( (mu__boolexpr126) )
  { mu__quant125 = TRUE; break; }
};
};
  mu__boolexpr102 = (!(mu__quant125)) ; 
}
if ( !(!(mu__boolexpr102)) )
  { mu__quant101 = FALSE; break; }
};
};
if ( !(mu__quant101) )
  { mu__quant100 = FALSE; break; }
};
};
if ( !(mu__quant100) )
  { mu__quant99 = FALSE; break; }
};
};
return mu__quant99;
};

bool mu__condition_127() // Condition for Rule "Consistency of data"
{
  return mu__invariant_98( );
}

/**** end rule declaration ****/

int mu__invariant_128() // Invariant "Condition for existance of master copy of data"
{
bool mu__quant129; 
mu__quant129 = TRUE;
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
bool mu__quant130; 
mu__quant130 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant131; 
mu__quant131 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr132;
bool mu__boolexpr133;
  if ((mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State) != (mu_Locally_Exmod)) mu__boolexpr133 = TRUE ;
  else {
  mu__boolexpr133 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) == (mu_INVAL)) ; 
}
  if (mu__boolexpr133) mu__boolexpr132 = TRUE ;
  else {
  mu__boolexpr132 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) == (mu_WINV)) ; 
}
if ( !(mu__boolexpr132) )
  { mu__quant131 = FALSE; break; }
};
};
if ( !(mu__quant131) )
  { mu__quant130 = FALSE; break; }
};
};
if ( !(mu__quant130) )
  { mu__quant129 = FALSE; break; }
};
};
return mu__quant129;
};

bool mu__condition_134() // Condition for Rule "Condition for existance of master copy of data"
{
  return mu__invariant_128( );
}

/**** end rule declaration ****/

int mu__invariant_135() // Invariant "Consistency within Directory"
{
bool mu__quant136; 
mu__quant136 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant137; 
mu__quant137 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr138;
bool mu__boolexpr139;
bool mu__boolexpr140;
  if (!((mu_Homes[mu_h].mu_Dir[mu_a].mu_State) == (mu_Uncached))) mu__boolexpr140 = FALSE ;
  else {
  mu__boolexpr140 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount) == (0)) ; 
}
  if (mu__boolexpr140) mu__boolexpr139 = TRUE ;
  else {
bool mu__boolexpr141;
  if (!((mu_Homes[mu_h].mu_Dir[mu_a].mu_State) == (mu_Dirty_Remote))) mu__boolexpr141 = FALSE ;
  else {
  mu__boolexpr141 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount) == (1)) ; 
}
  mu__boolexpr139 = (mu__boolexpr141) ; 
}
  if (mu__boolexpr139) mu__boolexpr138 = TRUE ;
  else {
bool mu__boolexpr142;
bool mu__boolexpr143;
  if (!((mu_Homes[mu_h].mu_Dir[mu_a].mu_State) == (mu_Shared_Remote))) mu__boolexpr143 = FALSE ;
  else {
  mu__boolexpr143 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount) != (0)) ; 
}
  if (!(mu__boolexpr143)) mu__boolexpr142 = FALSE ;
  else {
bool mu__quant144; 
mu__quant144 = TRUE;
{
for(int mu_i = 0; mu_i <= 1; mu_i++) {
bool mu__quant145; 
mu__quant145 = TRUE;
{
for(int mu_j = 0; mu_j <= 1; mu_j++) {
bool mu__boolexpr146;
bool mu__boolexpr147;
bool mu__boolexpr148;
  if (!((mu_i) != (mu_j))) mu__boolexpr148 = FALSE ;
  else {
  mu__boolexpr148 = ((mu_i) < (mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount)) ; 
}
  if (!(mu__boolexpr148)) mu__boolexpr147 = FALSE ;
  else {
  mu__boolexpr147 = ((mu_j) < (mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount)) ; 
}
  if (!(mu__boolexpr147)) mu__boolexpr146 = TRUE ;
  else {
  mu__boolexpr146 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_i]) != (mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_j])) ; 
}
if ( !(mu__boolexpr146) )
  { mu__quant145 = FALSE; break; }
};
};
if ( !(mu__quant145) )
  { mu__quant144 = FALSE; break; }
};
};
  mu__boolexpr142 = (mu__quant144) ; 
}
  mu__boolexpr138 = (mu__boolexpr142) ; 
}
if ( !(mu__boolexpr138) )
  { mu__quant137 = FALSE; break; }
};
};
if ( !(mu__quant137) )
  { mu__quant136 = FALSE; break; }
};
};
return mu__quant136;
};

bool mu__condition_149() // Condition for Rule "Consistency within Directory"
{
  return mu__invariant_135( );
}

/**** end rule declaration ****/

int mu__invariant_150() // Invariant "Irrelevant data is set to zero"
{
bool mu__quant151; 
mu__quant151 = TRUE;
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
bool mu__quant152; 
mu__quant152 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant153; 
mu__quant153 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr154;
bool mu__boolexpr155;
bool mu__boolexpr156;
bool mu__boolexpr157;
  if ((mu_Homes[mu_h].mu_Dir[mu_a].mu_State) != (mu_Uncached)) mu__boolexpr157 = TRUE ;
  else {
  mu__boolexpr157 = ((mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount) == (0)) ; 
}
  if (!(mu__boolexpr157)) mu__boolexpr156 = FALSE ;
  else {
bool mu__quant158; 
mu__quant158 = TRUE;
{
for(int mu_i = 0; mu_i <= 1; mu_i++) {
bool mu__boolexpr159;
  if (!((mu_i) >= (mu_Homes[mu_h].mu_Dir[mu_a].mu_SharedCount))) mu__boolexpr159 = TRUE ;
  else {
  mu__boolexpr159 = (mu_Homes[mu_h].mu_Dir[mu_a].mu_Entries[mu_i].isundefined()) ; 
}
if ( !(mu__boolexpr159) )
  { mu__quant158 = FALSE; break; }
};
};
  mu__boolexpr156 = (mu__quant158) ; 
}
  if (!(mu__boolexpr156)) mu__boolexpr155 = FALSE ;
  else {
bool mu__boolexpr160;
  if (!((mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_State) == (mu_Non_Locally_Cached))) mu__boolexpr160 = TRUE ;
  else {
  mu__boolexpr160 = (mu_Procs[mu_n].mu_Cache[mu_h][mu_a].mu_Value.isundefined()) ; 
}
  mu__boolexpr155 = (mu__boolexpr160) ; 
}
  if (!(mu__boolexpr155)) mu__boolexpr154 = FALSE ;
  else {
bool mu__boolexpr161;
  if (!((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) == (mu_INVAL))) mu__boolexpr161 = TRUE ;
  else {
bool mu__boolexpr162;
  if (!(mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_Value.isundefined())) mu__boolexpr162 = FALSE ;
  else {
  mu__boolexpr162 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_InvCount) == (0)) ; 
}
  mu__boolexpr161 = (mu__boolexpr162) ; 
}
  mu__boolexpr154 = (mu__boolexpr161) ; 
}
if ( !(mu__boolexpr154) )
  { mu__quant153 = FALSE; break; }
};
};
if ( !(mu__quant153) )
  { mu__quant152 = FALSE; break; }
};
};
if ( !(mu__quant152) )
  { mu__quant151 = FALSE; break; }
};
};
return mu__quant151;
};

bool mu__condition_163() // Condition for Rule "Irrelevant data is set to zero"
{
  return mu__invariant_150( );
}

/**** end rule declaration ****/

int mu__invariant_164() // Invariant "Only a single master copy exist"
{
bool mu__quant165; 
mu__quant165 = TRUE;
{
for(int mu_n1 = 1; mu_n1 <= 3; mu_n1++)
  if (( ( mu_n1 >= 2 ) && ( mu_n1 <= 3 ) )|| ( ( mu_n1 >= 1 ) && ( mu_n1 <= 1 ) )) {
bool mu__quant166; 
mu__quant166 = TRUE;
{
for(int mu_n2 = 1; mu_n2 <= 3; mu_n2++)
  if (( ( mu_n2 >= 2 ) && ( mu_n2 <= 3 ) )|| ( ( mu_n2 >= 1 ) && ( mu_n2 <= 1 ) )) {
bool mu__quant167; 
mu__quant167 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant168; 
mu__quant168 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr169;
bool mu__boolexpr170;
  if (!((mu_n1) != (mu_n2))) mu__boolexpr170 = FALSE ;
  else {
  mu__boolexpr170 = ((mu_Procs[mu_n1].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Exmod)) ; 
}
  if (!(mu__boolexpr170)) mu__boolexpr169 = FALSE ;
  else {
  mu__boolexpr169 = ((mu_Procs[mu_n2].mu_Cache[mu_h][mu_a].mu_State) == (mu_Locally_Exmod)) ; 
}
if ( !(!(mu__boolexpr169)) )
  { mu__quant168 = FALSE; break; }
};
};
if ( !(mu__quant168) )
  { mu__quant167 = FALSE; break; }
};
};
if ( !(mu__quant167) )
  { mu__quant166 = FALSE; break; }
};
};
if ( !(mu__quant166) )
  { mu__quant165 = FALSE; break; }
};
};
return mu__quant165;
};

bool mu__condition_171() // Condition for Rule "Only a single master copy exist"
{
  return mu__invariant_164( );
}

/**** end rule declaration ****/

int mu__invariant_172() // Invariant "Gobally invalid RAC state at Local Cluster"
{
bool mu__quant173; 
mu__quant173 = TRUE;
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
bool mu__quant174; 
mu__quant174 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant175; 
mu__quant175 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr176;
  if ((mu_h) == (mu_n)) mu__boolexpr176 = TRUE ;
  else {
  mu__boolexpr176 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) != (mu_WRDO)) ; 
}
if ( !(mu__boolexpr176) )
  { mu__quant175 = FALSE; break; }
};
};
if ( !(mu__quant175) )
  { mu__quant174 = FALSE; break; }
};
};
if ( !(mu__quant174) )
  { mu__quant173 = FALSE; break; }
};
};
return mu__quant173;
};

bool mu__condition_177() // Condition for Rule "Gobally invalid RAC state at Local Cluster"
{
  return mu__invariant_172( );
}

/**** end rule declaration ****/

int mu__invariant_178() // Invariant "Globally invalid RAC state at Home Cluster"
{
bool mu__quant179; 
mu__quant179 = TRUE;
{
for(int mu_n = 1; mu_n <= 3; mu_n++)
  if (( ( mu_n >= 2 ) && ( mu_n <= 3 ) )|| ( ( mu_n >= 1 ) && ( mu_n <= 1 ) )) {
bool mu__quant180; 
mu__quant180 = TRUE;
{
for(int mu_h = 1; mu_h <= 1; mu_h++) {
bool mu__quant181; 
mu__quant181 = TRUE;
{
for(int mu_a = 4; mu_a <= 4; mu_a++) {
bool mu__boolexpr182;
  if ((mu_h) != (mu_n)) mu__boolexpr182 = TRUE ;
  else {
bool mu__boolexpr183;
  if (!((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) != (mu_WRD))) mu__boolexpr183 = FALSE ;
  else {
  mu__boolexpr183 = ((mu_Procs[mu_n].mu_RAC[mu_h][mu_a].mu_State) != (mu_RRD)) ; 
}
  mu__boolexpr182 = (mu__boolexpr183) ; 
}
if ( !(mu__boolexpr182) )
  { mu__quant181 = FALSE; break; }
};
};
if ( !(mu__quant181) )
  { mu__quant180 = FALSE; break; }
};
};
if ( !(mu__quant180) )
  { mu__quant179 = FALSE; break; }
};
};
return mu__quant179;
};

bool mu__condition_184() // Condition for Rule "Globally invalid RAC state at Home Cluster"
{
  return mu__invariant_178( );
}

/**** end rule declaration ****/

const rulerec invariants[] = {
{"Globally invalid RAC state at Home Cluster", &mu__condition_184, NULL, FALSE},
{"Gobally invalid RAC state at Local Cluster", &mu__condition_177, NULL, FALSE},
{"Only a single master copy exist", &mu__condition_171, NULL, FALSE},
{"Irrelevant data is set to zero", &mu__condition_163, NULL, FALSE},
{"Consistency within Directory", &mu__condition_149, NULL, FALSE},
{"Condition for existance of master copy of data", &mu__condition_134, NULL, FALSE},
{"Consistency of data", &mu__condition_127, NULL, FALSE},
{"Adequate invalidations with Read Exclusive request", &mu__condition_97, NULL, FALSE},
};
const unsigned short numinvariants = 8;

/******************/
bool mu__true_live() { return TRUE; }
/******************/

/********************
  Liveness records
 ********************/
const liverec livenesses[] = {
{ NULL, NULL, NULL, NULL, E }};
const unsigned short numlivenesses = 0;

/********************
  Fairstates records
 ********************/
const rulerec fairnesses[] = {
{ NULL, NULL, NULL, FALSE }};
const unsigned short numfairnesses = 0;

/********************
  Normal/Canonicalization for scalarset
 ********************/
/*
Homes:ScalarsetVariable
Procs:ScalarsetArrayOfScalarset
ReqNet:Complex
ReplyNet:Complex
*/

/********************
Code for symmetry
 ********************/

/********************
 Permutation Set Class
 ********************/
class PermSet
{
public:
  // book keeping
  enum PresentationType {Simple, Explicit};
  PresentationType Presentation;

  void ResetToSimple();
  void ResetToExplicit();
  void SimpleToExplicit();
  void SimpleToOne();
  bool NextPermutation();

  void Print_in_size()
  { int ret=0; for (int i=0; i<count; i++) if (in[i]) ret++; cout << "in_size:" << ret << "\n"; }


  /********************
   Simple and efficient representation
   ********************/
  int class_mu_1_Value[2];
  int undefined_class_mu_1_Value;// has the highest class number

  void Print_class_mu_1_Value();
  bool OnlyOneRemain_mu_1_Value;
  bool MTO_class_mu_1_Value()
  {
    int i,j;
    if (OnlyOneRemain_mu_1_Value)
      return FALSE;
    for (i=0; i<2; i++)
      for (j=0; j<2; j++)
        if (i!=j && class_mu_1_Value[i]== class_mu_1_Value[j])
	    return TRUE;
    OnlyOneRemain_mu_1_Value = TRUE;
    return FALSE;
  }
  int class_mu_1_Remote[2];
  int undefined_class_mu_1_Remote;// has the highest class number

  void Print_class_mu_1_Remote();
  bool OnlyOneRemain_mu_1_Remote;
  bool MTO_class_mu_1_Remote()
  {
    int i,j;
    if (OnlyOneRemain_mu_1_Remote)
      return FALSE;
    for (i=0; i<2; i++)
      for (j=0; j<2; j++)
        if (i!=j && class_mu_1_Remote[i]== class_mu_1_Remote[j])
	    return TRUE;
    OnlyOneRemain_mu_1_Remote = TRUE;
    return FALSE;
  }
  bool AlreadyOnlyOneRemain;
  bool MoreThanOneRemain();


  /********************
   Explicit representation
  ********************/
  unsigned long size;
  unsigned long count;
  // in will be of product of factorial sizes for fast canonicalize
  // in will be of size 1 for reduced local memory canonicalize
  bool * in;

  // auxiliary for explicit representation

  // in/perm/revperm will be of factorial size for fast canonicalize
  // they will be of size 1 for reduced local memory canonicalize
  // second range will be size of the scalarset
  int * in_mu_1_Value;
  typedef int arr_mu_1_Value[2];
  arr_mu_1_Value * perm_mu_1_Value;
  arr_mu_1_Value * revperm_mu_1_Value;

  int size_mu_1_Value[2];
  bool reversed_sorted_mu_1_Value(int start, int end);
  void reverse_reversed_mu_1_Value(int start, int end);

  int * in_mu_1_Remote;
  typedef int arr_mu_1_Remote[2];
  arr_mu_1_Remote * perm_mu_1_Remote;
  arr_mu_1_Remote * revperm_mu_1_Remote;

  int size_mu_1_Remote[2];
  bool reversed_sorted_mu_1_Remote(int start, int end);
  void reverse_reversed_mu_1_Remote(int start, int end);

  // procedure for explicit representation
  bool ok0(mu_1_Value* perm, int size, mu_1_Value k);
  void GenPerm0(mu_1_Value* perm, int size, unsigned long& index);

  bool ok1(mu_1_Remote* perm, int size, mu_1_Remote k);
  void GenPerm1(mu_1_Remote* perm, int size, unsigned long& index);

  // General procedure
  PermSet();
  bool In(int i) const { return in[i]; };
  void Add(int i) { for (int j=0; j<i; j++) in[j] = FALSE;};
  void Remove(int i) { in[i] = FALSE; };
};
void PermSet::Print_class_mu_1_Value()
{
  cout << "class_mu_1_Value:\t";
  for (int i=0; i<2; i++)
    cout << class_mu_1_Value[i];
  cout << " " << undefined_class_mu_1_Value << "\n";
}
void PermSet::Print_class_mu_1_Remote()
{
  cout << "class_mu_1_Remote:\t";
  for (int i=0; i<2; i++)
    cout << class_mu_1_Remote[i];
  cout << " " << undefined_class_mu_1_Remote << "\n";
}
bool PermSet::MoreThanOneRemain()
{
  int i,j;
  if (AlreadyOnlyOneRemain)
    return FALSE;
  else {
    for (i=0; i<2; i++)
      for (j=0; j<2; j++)
        if (i!=j && class_mu_1_Value[i]== class_mu_1_Value[j])
	    return TRUE;
    for (i=0; i<2; i++)
      for (j=0; j<2; j++)
        if (i!=j && class_mu_1_Remote[i]== class_mu_1_Remote[j])
	    return TRUE;
  }
  AlreadyOnlyOneRemain = TRUE;
  return FALSE;
}
PermSet::PermSet()
: Presentation(Simple)
{
  int i,j,k;
  if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize
     || args->sym_alg.mode == argsym_alg::Heuristic_Fast_Canonicalize) {
    mu_1_Value Perm0[2];
    mu_1_Remote Perm1[2];

  /********************
   declaration of class variables
  ********************/
  in = new bool[4];
 in_mu_1_Value = new int[4];
 perm_mu_1_Value = new arr_mu_1_Value[2];
 revperm_mu_1_Value = new arr_mu_1_Value[2];
 in_mu_1_Remote = new int[4];
 perm_mu_1_Remote = new arr_mu_1_Remote[2];
 revperm_mu_1_Remote = new arr_mu_1_Remote[2];

    // Set perm and revperm
    count = 0;
    for (i=5; i<=6; i++)
      {
        Perm0[0].value(i);
        GenPerm0(Perm0, 1, count);
      }
    if (count!=2)
      Error.Error( "unable to initialize PermSet");
    for (i=0; i<2; i++)
      for (j=5; j<=6; j++)
        for (k=5; k<=6; k++)
          if (revperm_mu_1_Value[i][k-5]==j)   // k - base 
            perm_mu_1_Value[i][j-5]=k; // j - base 
    count = 0;
    for (i=2; i<=3; i++)
      {
        Perm1[0].value(i);
        GenPerm1(Perm1, 1, count);
      }
    if (count!=2)
      Error.Error( "unable to initialize PermSet");
    for (i=0; i<2; i++)
      for (j=2; j<=3; j++)
        for (k=2; k<=3; k++)
          if (revperm_mu_1_Remote[i][k-2]==j)   // k - base 
            perm_mu_1_Remote[i][j-2]=k; // j - base 

    // setting up combination of permutations
    // for different scalarset
    int carry;
    int i_mu_1_Value = 0;
    int i_mu_1_Remote = 0;
    size = 4;
    count = 4;
    for (i=0; i<4; i++)
      {
        carry = 1;
        in[i]= TRUE;
      in_mu_1_Value[i] = i_mu_1_Value;
      i_mu_1_Value += carry;
      if (i_mu_1_Value >= 2) { i_mu_1_Value = 0; carry = 1; } 
      else { carry = 0; } 
      in_mu_1_Remote[i] = i_mu_1_Remote;
      i_mu_1_Remote += carry;
      if (i_mu_1_Remote >= 2) { i_mu_1_Remote = 0; carry = 1; } 
      else { carry = 0; } 
    }
  }
  else
  {

  /********************
   declaration of class variables
  ********************/
  in = new bool[1];
 in_mu_1_Value = new int[1];
 perm_mu_1_Value = new arr_mu_1_Value[1];
 revperm_mu_1_Value = new arr_mu_1_Value[1];
 in_mu_1_Remote = new int[1];
 perm_mu_1_Remote = new arr_mu_1_Remote[1];
 revperm_mu_1_Remote = new arr_mu_1_Remote[1];
  in[0] = TRUE;
    in_mu_1_Value[0] = 0;
    in_mu_1_Remote[0] = 0;
  }
}
void PermSet::ResetToSimple()
{
  int i;
  for (i=0; i<2; i++)
    class_mu_1_Value[i]=0;
  undefined_class_mu_1_Value=0;
  OnlyOneRemain_mu_1_Value = FALSE;
  for (i=0; i<2; i++)
    class_mu_1_Remote[i]=0;
  undefined_class_mu_1_Remote=0;
  OnlyOneRemain_mu_1_Remote = FALSE;

  AlreadyOnlyOneRemain = FALSE;
  Presentation = Simple;
}
void PermSet::ResetToExplicit()
{
  for (int i=0; i<4; i++) in[i] = TRUE;
  Presentation = Explicit;
}
void PermSet::SimpleToExplicit()
{
  int i,j,k;
  int start, class_size;
  int start_mu_1_Value[2];
  int size_mu_1_Value[2];
  bool should_be_in_mu_1_Value[2];
  int start_mu_1_Remote[2];
  int size_mu_1_Remote[2];
  bool should_be_in_mu_1_Remote[2];

  // Setup range for mapping
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Value; j++) // class number
    {
      class_size = 0;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Value[k]==j)
	  class_size++;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Value[k]==j)
	  {
	    size_mu_1_Value[k] = class_size;
	    start_mu_1_Value[k] = start;
	  }
      start+=class_size;
    }
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Remote; j++) // class number
    {
      class_size = 0;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Remote[k]==j)
	  class_size++;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Remote[k]==j)
	  {
	    size_mu_1_Remote[k] = class_size;
	    start_mu_1_Remote[k] = start;
	  }
      start+=class_size;
    }

  // To be In or not to be
  for (i=0; i<2; i++) // set up
    should_be_in_mu_1_Value[i] = TRUE;
  for (i=0; i<2; i++) // to be in or not to be
    for (k=0; k<2; k++) // step through class_mu_1_pid[k]
      if (! (perm_mu_1_Value[i][k]-5 >=start_mu_1_Value[k] 
	     && perm_mu_1_Value[i][k]-5 < start_mu_1_Value[k] + size_mu_1_Value[k]) )
  	    {
	      should_be_in_mu_1_Value[i] = FALSE;
	      break;
	    }
  for (i=0; i<2; i++) // set up
    should_be_in_mu_1_Remote[i] = TRUE;
  for (i=0; i<2; i++) // to be in or not to be
    for (k=0; k<2; k++) // step through class_mu_1_pid[k]
      if (! (perm_mu_1_Remote[i][k]-2 >=start_mu_1_Remote[k] 
	     && perm_mu_1_Remote[i][k]-2 < start_mu_1_Remote[k] + size_mu_1_Remote[k]) )
  	    {
	      should_be_in_mu_1_Remote[i] = FALSE;
	      break;
	    }

  // setup explicit representation 
  // Set perm and revperm
  for (i=0; i<4; i++)
    {
      in[i] = TRUE;
      if (in[i] && !should_be_in_mu_1_Value[in_mu_1_Value[i]]) in[i] = FALSE;
      if (in[i] && !should_be_in_mu_1_Remote[in_mu_1_Remote[i]]) in[i] = FALSE;
    }
  Presentation = Explicit;
  if (args->test_parameter1.value==0) Print_in_size();
}
void PermSet::SimpleToOne()
{
  int i,j,k;
  int class_size;
  int start;


  // Setup range for mapping
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Value; j++) // class number
    {
      class_size = 0;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Value[k]==j)
	  class_size++;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Value[k]==j)
	  {
	    size_mu_1_Value[k] = class_size;
	  }
      start+=class_size;
    }
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Remote; j++) // class number
    {
      class_size = 0;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Remote[k]==j)
	  class_size++;
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	if (class_mu_1_Remote[k]==j)
	  {
	    size_mu_1_Remote[k] = class_size;
	  }
      start+=class_size;
    }
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Value; j++) // class number
    {
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	    if (class_mu_1_Value[k]==j)
	      revperm_mu_1_Value[0][start++] = k+5;
    }
  for (j=0; j<2; j++)
    for (k=0; k<2; k++)
      if (revperm_mu_1_Value[0][k]==j+5)
        perm_mu_1_Value[0][j]=k+5;
  start = 0;
  for (j=0; j<=undefined_class_mu_1_Remote; j++) // class number
    {
      for (k=0; k<2; k++) // step through class_mu_1_pid[k]
	    if (class_mu_1_Remote[k]==j)
	      revperm_mu_1_Remote[0][start++] = k+2;
    }
  for (j=0; j<2; j++)
    for (k=0; k<2; k++)
      if (revperm_mu_1_Remote[0][k]==j+2)
        perm_mu_1_Remote[0][j]=k+2;
  Presentation = Explicit;
}
bool PermSet::ok0(mu_1_Value* Perm, int size, mu_1_Value k)
{
  for (int i=0; i<size; i++)
    if(Perm[i].value()==k)
      return FALSE;
  return TRUE;
}
void PermSet::GenPerm0(mu_1_Value* Perm,int size, unsigned long& count)
{
  int i;
  if (size!=2)
    {
      for (i=5; i<=6; i++)
        if(ok0(Perm,size,i))
          {
            Perm[size].value(i);
            GenPerm0(Perm, size+1, count);
          }
    }
  else
    {
      for (i=5; i<=6; i++)
        revperm_mu_1_Value[count][i-5]=Perm[i-5].value();// i - base
      count++;
    }
}
bool PermSet::ok1(mu_1_Remote* Perm, int size, mu_1_Remote k)
{
  for (int i=0; i<size; i++)
    if(Perm[i].value()==k)
      return FALSE;
  return TRUE;
}
void PermSet::GenPerm1(mu_1_Remote* Perm,int size, unsigned long& count)
{
  int i;
  if (size!=2)
    {
      for (i=2; i<=3; i++)
        if(ok1(Perm,size,i))
          {
            Perm[size].value(i);
            GenPerm1(Perm, size+1, count);
          }
    }
  else
    {
      for (i=2; i<=3; i++)
        revperm_mu_1_Remote[count][i-2]=Perm[i-2].value();// i - base
      count++;
    }
}
bool PermSet::reversed_sorted_mu_1_Value(int start, int end)
{
  int i,j;

  for (i=start; i<end; i++)
    if (revperm_mu_1_Value[0][i]<revperm_mu_1_Value[0][i+1])
      return FALSE;
  return TRUE;
}
void PermSet::reverse_reversed_mu_1_Value(int start, int end)
{
  int i,j;
  int temp;

  for (i=start, j=end; i<j; i++,j--) 
    {
      temp = revperm_mu_1_Value[0][j];
      revperm_mu_1_Value[0][j] = revperm_mu_1_Value[0][i];
      revperm_mu_1_Value[0][i] = temp;
    }
}
bool PermSet::reversed_sorted_mu_1_Remote(int start, int end)
{
  int i,j;

  for (i=start; i<end; i++)
    if (revperm_mu_1_Remote[0][i]<revperm_mu_1_Remote[0][i+1])
      return FALSE;
  return TRUE;
}
void PermSet::reverse_reversed_mu_1_Remote(int start, int end)
{
  int i,j;
  int temp;

  for (i=start, j=end; i<j; i++,j--) 
    {
      temp = revperm_mu_1_Remote[0][j];
      revperm_mu_1_Remote[0][j] = revperm_mu_1_Remote[0][i];
      revperm_mu_1_Remote[0][i] = temp;
    }
}
bool PermSet::NextPermutation()
{
  bool nexted = FALSE;
  int start, end; 
  int class_size;
  int temp;
  int j,k;

  // algorithm
  // for each class
  //   if forall in the same class reverse_sorted, 
  //     { sort again; goto next class }
  //   else
  //     {
  //       nexted = TRUE;
  //       for (j from l to r)
  // 	       if (for all j+ are reversed sorted)
  // 	         {
  // 	           swap j, j+1
  // 	           sort all j+ again
  // 	           break;
  // 	         }
  //     }
  for (start = 0; start < 2; )
    {
      end = start-1+size_mu_1_Value[revperm_mu_1_Value[0][start]-5];
      if (reversed_sorted_mu_1_Value(start,end))
	       {
	  reverse_reversed_mu_1_Value(start,end);
	  start = end+1;
	}
      else
	{
	  nexted = TRUE;
	  for (j = start; j<end; j++)
	    {
	      if (reversed_sorted_mu_1_Value(j+1,end))
		{
		  for (k = end; k>j; k--)
		    {
		      if (revperm_mu_1_Value[0][j]<revperm_mu_1_Value[0][k])
			{
			  // swap j, k
			  temp = revperm_mu_1_Value[0][j];
			  revperm_mu_1_Value[0][j] = revperm_mu_1_Value[0][k];
			  revperm_mu_1_Value[0][k] = temp;
			  break;
			}
		    }
		  reverse_reversed_mu_1_Value(j+1,end);
		  break;
		}
	    }
	  break;
	}
    }
if (!nexted) {
  for (start = 0; start < 2; )
    {
      end = start-1+size_mu_1_Remote[revperm_mu_1_Remote[0][start]-2];
      if (reversed_sorted_mu_1_Remote(start,end))
	       {
	  reverse_reversed_mu_1_Remote(start,end);
	  start = end+1;
	}
      else
	{
	  nexted = TRUE;
	  for (j = start; j<end; j++)
	    {
	      if (reversed_sorted_mu_1_Remote(j+1,end))
		{
		  for (k = end; k>j; k--)
		    {
		      if (revperm_mu_1_Remote[0][j]<revperm_mu_1_Remote[0][k])
			{
			  // swap j, k
			  temp = revperm_mu_1_Remote[0][j];
			  revperm_mu_1_Remote[0][j] = revperm_mu_1_Remote[0][k];
			  revperm_mu_1_Remote[0][k] = temp;
			  break;
			}
		    }
		  reverse_reversed_mu_1_Remote(j+1,end);
		  break;
		}
	    }
	  break;
	}
    }
}
if (!nexted) return FALSE;
  for (j=0; j<2; j++)
    for (k=0; k<2; k++)
      if (revperm_mu_1_Value[0][k]==j+5)   // k - base 
	perm_mu_1_Value[0][j]=k+5; // j - base 
  for (j=0; j<2; j++)
    for (k=0; k<2; k++)
      if (revperm_mu_1_Remote[0][k]==j+2)   // k - base 
	perm_mu_1_Remote[0][j]=k+2; // j - base 
  return TRUE;
}

/********************
 Symmetry Class
 ********************/
class SymmetryClass
{
  PermSet Perm;
  bool BestInitialized;
  state BestPermutedState;

  // utilities
  void SetBestResult(int i, state* temp);
  void ResetBestResult() {BestInitialized = FALSE;};

public:
  // initializer
  SymmetryClass() : Perm(), BestInitialized(FALSE) {};
  ~SymmetryClass() {};

  void Normalize(state* s);

  void Exhaustive_Fast_Canonicalize(state *s);
  void Heuristic_Fast_Canonicalize(state *s);
  void Heuristic_Small_Mem_Canonicalize(state *s);
  void Heuristic_Fast_Normalize(state *s);

  void MultisetSort(state* s);
};


/********************
 Symmetry Class Members
 ********************/
void SymmetryClass::MultisetSort(state* s)
{
        mu_Homes.MultisetSort();
        mu_Procs.MultisetSort();
        mu_ReqNet.MultisetSort();
        mu_ReplyNet.MultisetSort();
}
void SymmetryClass::Normalize(state* s)
{
  switch (args->sym_alg.mode) {
  case argsym_alg::Exhaustive_Fast_Canonicalize:
    Exhaustive_Fast_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Fast_Canonicalize:
    Heuristic_Fast_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Small_Mem_Canonicalize:
    Heuristic_Small_Mem_Canonicalize(s);
    break;
  case argsym_alg::Heuristic_Fast_Normalize:
    Heuristic_Fast_Normalize(s);
    break;
  default:
    Heuristic_Fast_Canonicalize(s);
  }
}

/********************
 Permute and Canonicalize function for different types
 ********************/
void mu_1_Home::Permute(PermSet& Perm, int i) {}
void mu_1_Home::SimpleCanonicalize(PermSet& Perm) {}
void mu_1_Home::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_Home::SimpleLimit(PermSet& Perm) {}
void mu_1_Home::ArrayLimit(PermSet& Perm) {}
void mu_1_Home::Limit(PermSet& Perm) {}
void mu_1_Home::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset type.\n"); };
void mu_1_Remote::Permute(PermSet& Perm, int i)
{
  if (Perm.Presentation != PermSet::Explicit)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined())
    value(Perm.perm_mu_1_Remote[Perm.in_mu_1_Remote[i]][value()-2]); // value - base
};
void mu_1_Remote::SimpleCanonicalize(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_Remote[value()-2]==Perm.undefined_class_mu_1_Remote) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<2; i++)
          if (Perm.class_mu_1_Remote[i] == Perm.undefined_class_mu_1_Remote && i!=value()-2)
            Perm.class_mu_1_Remote[i]++;
        value(2 + Perm.undefined_class_mu_1_Remote++);
      }
    else 
      {
        value(Perm.class_mu_1_Remote[value()-2]+2);
      }
}
void mu_1_Remote::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_Remote::SimpleLimit(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_Remote[value()-2]==Perm.undefined_class_mu_1_Remote) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<2; i++)
          if (Perm.class_mu_1_Remote[i] == Perm.undefined_class_mu_1_Remote && i!=value()-2)
            Perm.class_mu_1_Remote[i]++;
        Perm.undefined_class_mu_1_Remote++;
      }
}
void mu_1_Remote::ArrayLimit(PermSet& Perm) {}
void mu_1_Remote::Limit(PermSet& Perm) {}
void mu_1_Remote::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset type.\n"); };
void mu_1_Proc::Permute(PermSet& Perm, int i)
{
  if (Perm.Presentation != PermSet::Explicit)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined()) {
    if ( ( value() >= 2 ) && ( value() <= 3 ) )
      value(Perm.perm_mu_1_Remote[Perm.in_mu_1_Remote[i]][value()-2]+(0)); // value - base
  }
}
void mu_1_Proc::SimpleCanonicalize(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined()) {
    if ( ( value() >= 2 ) && ( value() <= 3 ) )
      {
        if (Perm.class_mu_1_Remote[value()-2]==Perm.undefined_class_mu_1_Remote) // value - base
          {
            // it has not been mapped to any particular value
            for (i=0; i<2; i++)
              if (Perm.class_mu_1_Remote[i] == Perm.undefined_class_mu_1_Remote && i!=value()-2)
                Perm.class_mu_1_Remote[i]++;
            value(2 + Perm.undefined_class_mu_1_Remote++);
          }
        else 
          {
            value(Perm.class_mu_1_Remote[value()-2]+2);
          }
      }
  }
}
void mu_1_Proc::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_Proc::SimpleLimit(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined()) {
    if ( ( value() >= 2 ) && ( value() <= 3 ) )
      if (Perm.class_mu_1_Remote[value()-2]==Perm.undefined_class_mu_1_Remote) // value - base
        {
          // it has not been mapped to any particular value
          for (i=0; i<2; i++)
            if (Perm.class_mu_1_Remote[i] == Perm.undefined_class_mu_1_Remote && i!=value()-2)
              Perm.class_mu_1_Remote[i]++;
          Perm.undefined_class_mu_1_Remote++;
        }
  }
}
void mu_1_Proc::ArrayLimit(PermSet& Perm) {}
void mu_1_Proc::Limit(PermSet& Perm) {}
void mu_1_Proc::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for union type.\n"); };
void mu_1_DirIndex::Permute(PermSet& Perm, int i) {};
void mu_1_DirIndex::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_DirIndex::Canonicalize(PermSet& Perm) {};
void mu_1_DirIndex::SimpleLimit(PermSet& Perm) {};
void mu_1_DirIndex::ArrayLimit(PermSet& Perm) {};
void mu_1_DirIndex::Limit(PermSet& Perm) {};
void mu_1_DirIndex::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1_NodeCount::Permute(PermSet& Perm, int i) {};
void mu_1_NodeCount::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_NodeCount::Canonicalize(PermSet& Perm) {};
void mu_1_NodeCount::SimpleLimit(PermSet& Perm) {};
void mu_1_NodeCount::ArrayLimit(PermSet& Perm) {};
void mu_1_NodeCount::Limit(PermSet& Perm) {};
void mu_1_NodeCount::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1_Address::Permute(PermSet& Perm, int i) {}
void mu_1_Address::SimpleCanonicalize(PermSet& Perm) {}
void mu_1_Address::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_Address::SimpleLimit(PermSet& Perm) {}
void mu_1_Address::ArrayLimit(PermSet& Perm) {}
void mu_1_Address::Limit(PermSet& Perm) {}
void mu_1_Address::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset type.\n"); };
void mu_1_Value::Permute(PermSet& Perm, int i)
{
  if (Perm.Presentation != PermSet::Explicit)
    Error.Error("Internal Error: Wrong Sequence of Normalization");
  if (defined())
    value(Perm.perm_mu_1_Value[Perm.in_mu_1_Value[i]][value()-5]); // value - base
};
void mu_1_Value::SimpleCanonicalize(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_Value[value()-5]==Perm.undefined_class_mu_1_Value) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<2; i++)
          if (Perm.class_mu_1_Value[i] == Perm.undefined_class_mu_1_Value && i!=value()-5)
            Perm.class_mu_1_Value[i]++;
        value(5 + Perm.undefined_class_mu_1_Value++);
      }
    else 
      {
        value(Perm.class_mu_1_Value[value()-5]+5);
      }
}
void mu_1_Value::Canonicalize(PermSet& Perm)
{
  Error.Error("Calling canonicalize() for Scalarset.");
}
void mu_1_Value::SimpleLimit(PermSet& Perm)
{
  int i, class_number;
  if (Perm.Presentation != PermSet::Simple)
    Error.Error("Internal Error: Wrong Sequence of Normalization");

  if (defined())
    if (Perm.class_mu_1_Value[value()-5]==Perm.undefined_class_mu_1_Value) // value - base
      {
        // it has not been mapped to any particular value
        for (i=0; i<2; i++)
          if (Perm.class_mu_1_Value[i] == Perm.undefined_class_mu_1_Value && i!=value()-5)
            Perm.class_mu_1_Value[i]++;
        Perm.undefined_class_mu_1_Value++;
      }
}
void mu_1_Value::ArrayLimit(PermSet& Perm) {}
void mu_1_Value::Limit(PermSet& Perm) {}
void mu_1_Value::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset type.\n"); };
void mu_1_RequestType::Permute(PermSet& Perm, int i) {};
void mu_1_RequestType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_RequestType::Canonicalize(PermSet& Perm) {};
void mu_1_RequestType::SimpleLimit(PermSet& Perm) {};
void mu_1_RequestType::ArrayLimit(PermSet& Perm) {};
void mu_1_RequestType::Limit(PermSet& Perm) {};
void mu_1_RequestType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_ReplyType::Permute(PermSet& Perm, int i) {};
void mu_1_ReplyType::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_ReplyType::Canonicalize(PermSet& Perm) {};
void mu_1_ReplyType::SimpleLimit(PermSet& Perm) {};
void mu_1_ReplyType::ArrayLimit(PermSet& Perm) {};
void mu_1_ReplyType::Limit(PermSet& Perm) {};
void mu_1_ReplyType::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_Request::Permute(PermSet& Perm, int i)
{
  mu_Aux.Permute(Perm,i);
  mu_Value.Permute(Perm,i);
};
void mu_1_Request::SimpleCanonicalize(PermSet& Perm)
{
  mu_Aux.SimpleCanonicalize(Perm);
  mu_Value.SimpleCanonicalize(Perm);
};
void mu_1_Request::Canonicalize(PermSet& Perm)
{
};
void mu_1_Request::SimpleLimit(PermSet& Perm)
{
  mu_Aux.SimpleLimit(Perm);
  mu_Value.SimpleLimit(Perm);
};
void mu_1_Request::ArrayLimit(PermSet& Perm){}
void mu_1_Request::Limit(PermSet& Perm)
{
};
void mu_1_Request::MultisetLimit(PermSet& Perm)
{
};
void mu_1_Reply::Permute(PermSet& Perm, int i)
{
  mu_Aux.Permute(Perm,i);
  mu_Value.Permute(Perm,i);
};
void mu_1_Reply::SimpleCanonicalize(PermSet& Perm)
{
  mu_Aux.SimpleCanonicalize(Perm);
  mu_Value.SimpleCanonicalize(Perm);
};
void mu_1_Reply::Canonicalize(PermSet& Perm)
{
};
void mu_1_Reply::SimpleLimit(PermSet& Perm)
{
  mu_Aux.SimpleLimit(Perm);
  mu_Value.SimpleLimit(Perm);
};
void mu_1_Reply::ArrayLimit(PermSet& Perm){}
void mu_1_Reply::Limit(PermSet& Perm)
{
};
void mu_1_Reply::MultisetLimit(PermSet& Perm)
{
};
void mu_1_RAC_State::Permute(PermSet& Perm, int i) {};
void mu_1_RAC_State::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_RAC_State::Canonicalize(PermSet& Perm) {};
void mu_1_RAC_State::SimpleLimit(PermSet& Perm) {};
void mu_1_RAC_State::ArrayLimit(PermSet& Perm) {};
void mu_1_RAC_State::Limit(PermSet& Perm) {};
void mu_1_RAC_State::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1_CacheState::Permute(PermSet& Perm, int i) {};
void mu_1_CacheState::SimpleCanonicalize(PermSet& Perm) {};
void mu_1_CacheState::Canonicalize(PermSet& Perm) {};
void mu_1_CacheState::SimpleLimit(PermSet& Perm) {};
void mu_1_CacheState::ArrayLimit(PermSet& Perm) {};
void mu_1_CacheState::Limit(PermSet& Perm) {};
void mu_1_CacheState::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1__type_0::Permute(PermSet& Perm, int i)
{
  static mu_1__type_0 temp("Permute_mu_1__type_0",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_0::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_0::Canonicalize(PermSet& Perm){};
void mu_1__type_0::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_0::ArrayLimit(PermSet& Perm) {}
void mu_1__type_0::Limit(PermSet& Perm){}
void mu_1__type_0::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_1::Permute(PermSet& Perm, int i) {};
void mu_1__type_1::SimpleCanonicalize(PermSet& Perm) {};
void mu_1__type_1::Canonicalize(PermSet& Perm) {};
void mu_1__type_1::SimpleLimit(PermSet& Perm) {};
void mu_1__type_1::ArrayLimit(PermSet& Perm) {};
void mu_1__type_1::Limit(PermSet& Perm) {};
void mu_1__type_1::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for enum type.\n"); };
void mu_1__type_2::Permute(PermSet& Perm, int i) {};
void mu_1__type_2::SimpleCanonicalize(PermSet& Perm) {};
void mu_1__type_2::Canonicalize(PermSet& Perm) {};
void mu_1__type_2::SimpleLimit(PermSet& Perm) {};
void mu_1__type_2::ArrayLimit(PermSet& Perm) {};
void mu_1__type_2::Limit(PermSet& Perm) {};
void mu_1__type_2::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1__type_3::Permute(PermSet& Perm, int i)
{
  static mu_1__type_3 temp("Permute_mu_1__type_3",-1);
  int j;
  for (j=0; j<2; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_3::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<2; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_3::Canonicalize(PermSet& Perm){};
void mu_1__type_3::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<2; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_3::ArrayLimit(PermSet& Perm) {}
void mu_1__type_3::Limit(PermSet& Perm){}
void mu_1__type_3::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_4::Permute(PermSet& Perm, int i)
{
  mu_Entries.Permute(Perm,i);
};
void mu_1__type_4::SimpleCanonicalize(PermSet& Perm)
{
  mu_Entries.SimpleCanonicalize(Perm);
};
void mu_1__type_4::Canonicalize(PermSet& Perm)
{
};
void mu_1__type_4::SimpleLimit(PermSet& Perm)
{
  mu_Entries.SimpleLimit(Perm);
};
void mu_1__type_4::ArrayLimit(PermSet& Perm){}
void mu_1__type_4::Limit(PermSet& Perm)
{
};
void mu_1__type_4::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_5::Permute(PermSet& Perm, int i)
{
  static mu_1__type_5 temp("Permute_mu_1__type_5",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_5::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_5::Canonicalize(PermSet& Perm){};
void mu_1__type_5::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_5::ArrayLimit(PermSet& Perm) {}
void mu_1__type_5::Limit(PermSet& Perm){}
void mu_1__type_5::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1_HomeState::Permute(PermSet& Perm, int i)
{
  mu_Mem.Permute(Perm,i);
  mu_Dir.Permute(Perm,i);
};
void mu_1_HomeState::SimpleCanonicalize(PermSet& Perm)
{
  mu_Mem.SimpleCanonicalize(Perm);
  mu_Dir.SimpleCanonicalize(Perm);
};
void mu_1_HomeState::Canonicalize(PermSet& Perm)
{
};
void mu_1_HomeState::SimpleLimit(PermSet& Perm)
{
  mu_Mem.SimpleLimit(Perm);
  mu_Dir.SimpleLimit(Perm);
};
void mu_1_HomeState::ArrayLimit(PermSet& Perm){}
void mu_1_HomeState::Limit(PermSet& Perm)
{
};
void mu_1_HomeState::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_6::Permute(PermSet& Perm, int i)
{
  mu_Value.Permute(Perm,i);
};
void mu_1__type_6::SimpleCanonicalize(PermSet& Perm)
{
  mu_Value.SimpleCanonicalize(Perm);
};
void mu_1__type_6::Canonicalize(PermSet& Perm)
{
};
void mu_1__type_6::SimpleLimit(PermSet& Perm)
{
  mu_Value.SimpleLimit(Perm);
};
void mu_1__type_6::ArrayLimit(PermSet& Perm){}
void mu_1__type_6::Limit(PermSet& Perm)
{
};
void mu_1__type_6::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_7::Permute(PermSet& Perm, int i)
{
  static mu_1__type_7 temp("Permute_mu_1__type_7",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_7::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_7::Canonicalize(PermSet& Perm){};
void mu_1__type_7::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_7::ArrayLimit(PermSet& Perm) {}
void mu_1__type_7::Limit(PermSet& Perm){}
void mu_1__type_7::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_8::Permute(PermSet& Perm, int i)
{
  static mu_1__type_8 temp("Permute_mu_1__type_8",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_8::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_8::Canonicalize(PermSet& Perm){};
void mu_1__type_8::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_8::ArrayLimit(PermSet& Perm) {}
void mu_1__type_8::Limit(PermSet& Perm){}
void mu_1__type_8::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_9::Permute(PermSet& Perm, int i)
{
  mu_Value.Permute(Perm,i);
};
void mu_1__type_9::SimpleCanonicalize(PermSet& Perm)
{
  mu_Value.SimpleCanonicalize(Perm);
};
void mu_1__type_9::Canonicalize(PermSet& Perm)
{
};
void mu_1__type_9::SimpleLimit(PermSet& Perm)
{
  mu_Value.SimpleLimit(Perm);
};
void mu_1__type_9::ArrayLimit(PermSet& Perm){}
void mu_1__type_9::Limit(PermSet& Perm)
{
};
void mu_1__type_9::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_10::Permute(PermSet& Perm, int i)
{
  static mu_1__type_10 temp("Permute_mu_1__type_10",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_10::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_10::Canonicalize(PermSet& Perm){};
void mu_1__type_10::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_10::ArrayLimit(PermSet& Perm) {}
void mu_1__type_10::Limit(PermSet& Perm){}
void mu_1__type_10::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_11::Permute(PermSet& Perm, int i)
{
  static mu_1__type_11 temp("Permute_mu_1__type_11",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_11::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_11::Canonicalize(PermSet& Perm){};
void mu_1__type_11::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_11::ArrayLimit(PermSet& Perm) {}
void mu_1__type_11::Limit(PermSet& Perm){}
void mu_1__type_11::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1_ProcState::Permute(PermSet& Perm, int i)
{
  mu_Cache.Permute(Perm,i);
  mu_RAC.Permute(Perm,i);
};
void mu_1_ProcState::SimpleCanonicalize(PermSet& Perm)
{
  mu_Cache.SimpleCanonicalize(Perm);
  mu_RAC.SimpleCanonicalize(Perm);
};
void mu_1_ProcState::Canonicalize(PermSet& Perm)
{
};
void mu_1_ProcState::SimpleLimit(PermSet& Perm)
{
  mu_Cache.SimpleLimit(Perm);
  mu_RAC.SimpleLimit(Perm);
};
void mu_1_ProcState::ArrayLimit(PermSet& Perm){}
void mu_1_ProcState::Limit(PermSet& Perm)
{
};
void mu_1_ProcState::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_12::Permute(PermSet& Perm, int i) {};
void mu_1__type_12::SimpleCanonicalize(PermSet& Perm) {};
void mu_1__type_12::Canonicalize(PermSet& Perm) {};
void mu_1__type_12::SimpleLimit(PermSet& Perm) {};
void mu_1__type_12::ArrayLimit(PermSet& Perm) {};
void mu_1__type_12::Limit(PermSet& Perm) {};
void mu_1__type_12::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_30::Permute(PermSet& Perm, int i) {};
void mu__subrange_30::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_30::Canonicalize(PermSet& Perm) {};
void mu__subrange_30::SimpleLimit(PermSet& Perm) {};
void mu__subrange_30::ArrayLimit(PermSet& Perm) {};
void mu__subrange_30::Limit(PermSet& Perm) {};
void mu__subrange_30::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1__type_13::Permute(PermSet& Perm, int i)
{
  static mu_1__type_13 temp("Permute_mu_1__type_13",-1);
  int j;
  for (j=0; j<6; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_13::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<6; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_13::Canonicalize(PermSet& Perm){};
void mu_1__type_13::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<6; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_13::ArrayLimit(PermSet& Perm) {}
void mu_1__type_13::Limit(PermSet& Perm){}
void mu_1__type_13::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_14::Permute(PermSet& Perm, int i)
{
  mu_Messages.Permute(Perm,i);
};
void mu_1__type_14::SimpleCanonicalize(PermSet& Perm)
{
  mu_Messages.SimpleCanonicalize(Perm);
};
void mu_1__type_14::Canonicalize(PermSet& Perm)
{
};
void mu_1__type_14::SimpleLimit(PermSet& Perm)
{
  mu_Messages.SimpleLimit(Perm);
};
void mu_1__type_14::ArrayLimit(PermSet& Perm){}
void mu_1__type_14::Limit(PermSet& Perm)
{
};
void mu_1__type_14::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_15::Permute(PermSet& Perm, int i)
{
  static mu_1__type_15 temp("Permute_mu_1__type_15",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=2; j<=3; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_Remote[Perm.in_mu_1_Remote[i]][j-2]];
};
void mu_1__type_15::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_15::Canonicalize(PermSet& Perm){};
void mu_1__type_15::SimpleLimit(PermSet& Perm){}
void mu_1__type_15::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int compare;
  static mu_1__type_14 value[3];
  // limit
  bool exists;
  bool split;
  int count_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // sorting mu_1_Remote
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          pos_mu_1_Remote[i][j]=FALSE;
      count_mu_1_Remote = 0;
      for (i=0; i<2; i++)
        {
          for (j=0; j<count_mu_1_Remote; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+2]);
              if (compare==0)
                {
                  pos_mu_1_Remote[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_Remote; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<2; z++)
                        pos_mu_1_Remote[k][z] = pos_mu_1_Remote[k-1][z];
                    }
                  value[j] = (*this)[i+2];
                  for (z=0; z<2; z++)
                    pos_mu_1_Remote[j][z] = FALSE;
                  pos_mu_1_Remote[j][i] = TRUE;
                  count_mu_1_Remote++;
                  break;
                }
            }
          if (j==count_mu_1_Remote)
            {
              value[j] = (*this)[i+2];
              for (z=0; z<2; z++)
                pos_mu_1_Remote[j][z] = FALSE;
              pos_mu_1_Remote[j][i] = TRUE;
              count_mu_1_Remote++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote>1)
    {
      // limit
      for (j=0; j<2; j++) // class priority
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] && Perm.class_mu_1_Remote[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                    pos_mu_1_Remote[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<2; k++)
                        if (Perm.class_mu_1_Remote[k]>j
                            || ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) )
                          Perm.class_mu_1_Remote[k]++;
                      Perm.undefined_class_mu_1_Remote++;
                    }
                }
            }
        }
    }
}
void mu_1__type_15::Limit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // while guard
  bool while_guard, while_guard_temp;
  // sorting
  static mu_1__type_14 value[3];
  // limit
  bool exists;
  bool split;
  int i0;
  int count_mu_1_Value, oldcount_mu_1_Value;
  bool pos_mu_1_Value[2][2];
  bool goodset_mu_1_Value[2];
  int count_mu_1_Remote, oldcount_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Value[i][j]=FALSE;
  count_mu_1_Value = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Value[i] == count_mu_1_Value)
         {
           pos_mu_1_Value[count_mu_1_Value][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Value++;
      else break;
    }
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Remote[i][j]=FALSE;
  count_mu_1_Remote = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Remote[i] == count_mu_1_Remote)
         {
           pos_mu_1_Remote[count_mu_1_Remote][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Remote++;
      else break;
    }
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

  // refinement -- check selfloop
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
    {
      exists = FALSE;
      split = FALSE;
      // if there exists both self loop and non-self loop
      for (k=0; k<2; k++) // step through class
        if ((*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
            ||(*this)[k+2].mu_Messages[i0].mu_Aux!=k+2)
          exists = TRUE;
        else
          split = TRUE;
      if (exists && split)
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] 
                    && !(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                    && (*this)[k+2].mu_Messages[i0].mu_Aux==k+2)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] ) 
                          split= TRUE;
                  if (split)
                    {
                      for (j=count_mu_1_Remote; j>i; j--)
                        for (k=0; k<2; k++)
                          pos_mu_1_Remote[j][k] = pos_mu_1_Remote[j-1][k];
                      for (k=0; k<2; k++)
                        {
                          if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                            pos_mu_1_Remote[i][k] = FALSE;
                          if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                            pos_mu_1_Remote[i+1][k] = FALSE;
                        }
                      count_mu_1_Remote++; i++;
                    }
                }
            }
        }
    }
  }

  // refinement -- checking priority in general
  while_guard = FALSE;
  while_guard = while_guard || (Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2);
  while_guard = while_guard || (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2);
  while ( while_guard )
    {
      oldcount_mu_1_Value = count_mu_1_Value;
      oldcount_mu_1_Remote = count_mu_1_Remote;
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

      // refinement -- graph structure for a single scalarset
      //               as in array S1 of S1
      // only if there is more than 1 permutation in class
      if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if (!(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                &&(*this)[k+2].mu_Messages[i0].mu_Aux!=k+2
                &&(*this)[k+2].mu_Messages[i0].mu_Aux>=2
                &&(*this)[k+2].mu_Messages[i0].mu_Aux<=3)
              exists = TRUE;
          if (exists)
            {
              for (i=0; i<count_mu_1_Remote; i++) // for value priority
                {
                  for (j=0; j<count_mu_1_Remote; j++) // for value priority
                    {
                      exists = FALSE;
                      for (k=0; k<2; k++) // step through class
                        goodset_mu_1_Remote[k] = FALSE;
                      for (k=0; k<2; k++) // step through class
                        if (pos_mu_1_Remote[i][k] 
                            && !(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                            && (*this)[k+2].mu_Messages[i0].mu_Aux!=k+2
                            && (*this)[k+2].mu_Messages[i0].mu_Aux>=2
                            && (*this)[k+2].mu_Messages[i0].mu_Aux<=3
                            && pos_mu_1_Remote[j][(*this)[k+2].mu_Messages[i0].mu_Aux-2])
                          {
                            exists = TRUE;
                            goodset_mu_1_Remote[k] = TRUE;
                          }
                      if (exists)
                        {
                          split=FALSE;
                          for (k=0; k<2; k++)
                            if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] ) 
                              split= TRUE;
                          if (split)
                            {
                              for (j=count_mu_1_Remote; j>i; j--)
                                for (k=0; k<2; k++)
                                  pos_mu_1_Remote[j][k] = pos_mu_1_Remote[j-1][k];
                              for (k=0; k<2; k++)
                                {
                                  if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                    pos_mu_1_Remote[i][k] = FALSE;
                                  if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                    pos_mu_1_Remote[i+1][k] = FALSE;                  
                                }
                              count_mu_1_Remote++;
                            }
                        }
                    }
                }
            }
        }
  }
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

      // refinement -- graph structure for two scalarsets
      //               as in array S1 of S2
      // only if there is more than 1 permutation in class
      if ( (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
           || ( Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2) )
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if ((*this)[k+2].mu_Messages[i0].mu_Value.isundefined())
              exists = TRUE;
            else
              split = TRUE;
          if (exists && split)
            {
              for (i=0; i<count_mu_1_Remote; i++) // scan through array index priority
                for (j=0; j<count_mu_1_Value; j++) //scan through element priority
                  {
                    exists = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Remote[k] = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Value[k] = FALSE;
                    for (k=0; k<2; k++) // scan array index
                      // set goodsets
                      if (pos_mu_1_Remote[i][k] 
                          && !(*this)[k+2].mu_Messages[i0].mu_Value.isundefined()
                          && pos_mu_1_Value[j][(*this)[k+2].mu_Messages[i0].mu_Value-5])
                        {
                          exists = TRUE;
                          goodset_mu_1_Remote[k] = TRUE;
                          goodset_mu_1_Value[(*this)[k+2].mu_Messages[i0].mu_Value-5] = TRUE;
                        }
                    if (exists)
                      {
                        // set split for the array index type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Remote; z>i; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Remote[z][k] = pos_mu_1_Remote[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i][k] = FALSE;
                                if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i+1][k] = FALSE;
                              }
                            count_mu_1_Remote++;
                          }
                        // set split for the element type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Value; z>j; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Value[z][k] = pos_mu_1_Value[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j][k] = FALSE;
                                if (pos_mu_1_Value[j+1][k] && goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j+1][k] = FALSE;
                              }
                            count_mu_1_Value++;
                          }
                      }
                  }
            }
        }
  }
      while_guard = FALSE;
      while_guard = while_guard || (oldcount_mu_1_Value!=count_mu_1_Value);
      while_guard = while_guard || (oldcount_mu_1_Remote!=count_mu_1_Remote);
      while_guard_temp = while_guard;
      while_guard = FALSE;
      while_guard = while_guard || count_mu_1_Value<2;
      while_guard = while_guard || count_mu_1_Remote<2;
      while_guard = while_guard && while_guard_temp;
    } // end while
  // enter the result into class
  if (Perm.MTO_class_mu_1_Value())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Value[i][j])
            Perm.class_mu_1_Value[j] = i;
      Perm.undefined_class_mu_1_Value=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Value[j]>Perm.undefined_class_mu_1_Value)
          Perm.undefined_class_mu_1_Value=Perm.class_mu_1_Value[j];
    }
  // enter the result into class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Remote[i][j])
            Perm.class_mu_1_Remote[j] = i;
      Perm.undefined_class_mu_1_Remote=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Remote[j]>Perm.undefined_class_mu_1_Remote)
          Perm.undefined_class_mu_1_Remote=Perm.class_mu_1_Remote[j];
    }
}
void mu_1__type_15::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_16::Permute(PermSet& Perm, int i)
{
  static mu_1__type_16 temp("Permute_mu_1__type_16",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=2; j<=3; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_Remote[Perm.in_mu_1_Remote[i]][j-2]];
};
void mu_1__type_16::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_16::Canonicalize(PermSet& Perm){};
void mu_1__type_16::SimpleLimit(PermSet& Perm){}
void mu_1__type_16::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int compare;
  static mu_1__type_15 value[3];
  // limit
  bool exists;
  bool split;
  int count_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // sorting mu_1_Remote
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          pos_mu_1_Remote[i][j]=FALSE;
      count_mu_1_Remote = 0;
      for (i=0; i<2; i++)
        {
          for (j=0; j<count_mu_1_Remote; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+2]);
              if (compare==0)
                {
                  pos_mu_1_Remote[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_Remote; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<2; z++)
                        pos_mu_1_Remote[k][z] = pos_mu_1_Remote[k-1][z];
                    }
                  value[j] = (*this)[i+2];
                  for (z=0; z<2; z++)
                    pos_mu_1_Remote[j][z] = FALSE;
                  pos_mu_1_Remote[j][i] = TRUE;
                  count_mu_1_Remote++;
                  break;
                }
            }
          if (j==count_mu_1_Remote)
            {
              value[j] = (*this)[i+2];
              for (z=0; z<2; z++)
                pos_mu_1_Remote[j][z] = FALSE;
              pos_mu_1_Remote[j][i] = TRUE;
              count_mu_1_Remote++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote>1)
    {
      // limit
      for (j=0; j<2; j++) // class priority
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] && Perm.class_mu_1_Remote[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                    pos_mu_1_Remote[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<2; k++)
                        if (Perm.class_mu_1_Remote[k]>j
                            || ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) )
                          Perm.class_mu_1_Remote[k]++;
                      Perm.undefined_class_mu_1_Remote++;
                    }
                }
            }
        }
    }
}
void mu_1__type_16::Limit(PermSet& Perm){}
void mu_1__type_16::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_17::Permute(PermSet& Perm, int i) {};
void mu_1__type_17::SimpleCanonicalize(PermSet& Perm) {};
void mu_1__type_17::Canonicalize(PermSet& Perm) {};
void mu_1__type_17::SimpleLimit(PermSet& Perm) {};
void mu_1__type_17::ArrayLimit(PermSet& Perm) {};
void mu_1__type_17::Limit(PermSet& Perm) {};
void mu_1__type_17::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_36::Permute(PermSet& Perm, int i) {};
void mu__subrange_36::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_36::Canonicalize(PermSet& Perm) {};
void mu__subrange_36::SimpleLimit(PermSet& Perm) {};
void mu__subrange_36::ArrayLimit(PermSet& Perm) {};
void mu__subrange_36::Limit(PermSet& Perm) {};
void mu__subrange_36::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu_1__type_18::Permute(PermSet& Perm, int i)
{
  static mu_1__type_18 temp("Permute_mu_1__type_18",-1);
  int j;
  for (j=0; j<6; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_18::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<6; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_18::Canonicalize(PermSet& Perm){};
void mu_1__type_18::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<6; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_18::ArrayLimit(PermSet& Perm) {}
void mu_1__type_18::Limit(PermSet& Perm){}
void mu_1__type_18::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_19::Permute(PermSet& Perm, int i)
{
  mu_Messages.Permute(Perm,i);
};
void mu_1__type_19::SimpleCanonicalize(PermSet& Perm)
{
  mu_Messages.SimpleCanonicalize(Perm);
};
void mu_1__type_19::Canonicalize(PermSet& Perm)
{
};
void mu_1__type_19::SimpleLimit(PermSet& Perm)
{
  mu_Messages.SimpleLimit(Perm);
};
void mu_1__type_19::ArrayLimit(PermSet& Perm){}
void mu_1__type_19::Limit(PermSet& Perm)
{
};
void mu_1__type_19::MultisetLimit(PermSet& Perm)
{
};
void mu_1__type_20::Permute(PermSet& Perm, int i)
{
  static mu_1__type_20 temp("Permute_mu_1__type_20",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=2; j<=3; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_Remote[Perm.in_mu_1_Remote[i]][j-2]];
};
void mu_1__type_20::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_20::Canonicalize(PermSet& Perm){};
void mu_1__type_20::SimpleLimit(PermSet& Perm){}
void mu_1__type_20::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int compare;
  static mu_1__type_19 value[3];
  // limit
  bool exists;
  bool split;
  int count_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // sorting mu_1_Remote
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          pos_mu_1_Remote[i][j]=FALSE;
      count_mu_1_Remote = 0;
      for (i=0; i<2; i++)
        {
          for (j=0; j<count_mu_1_Remote; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+2]);
              if (compare==0)
                {
                  pos_mu_1_Remote[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_Remote; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<2; z++)
                        pos_mu_1_Remote[k][z] = pos_mu_1_Remote[k-1][z];
                    }
                  value[j] = (*this)[i+2];
                  for (z=0; z<2; z++)
                    pos_mu_1_Remote[j][z] = FALSE;
                  pos_mu_1_Remote[j][i] = TRUE;
                  count_mu_1_Remote++;
                  break;
                }
            }
          if (j==count_mu_1_Remote)
            {
              value[j] = (*this)[i+2];
              for (z=0; z<2; z++)
                pos_mu_1_Remote[j][z] = FALSE;
              pos_mu_1_Remote[j][i] = TRUE;
              count_mu_1_Remote++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote>1)
    {
      // limit
      for (j=0; j<2; j++) // class priority
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] && Perm.class_mu_1_Remote[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                    pos_mu_1_Remote[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<2; k++)
                        if (Perm.class_mu_1_Remote[k]>j
                            || ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) )
                          Perm.class_mu_1_Remote[k]++;
                      Perm.undefined_class_mu_1_Remote++;
                    }
                }
            }
        }
    }
}
void mu_1__type_20::Limit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // while guard
  bool while_guard, while_guard_temp;
  // sorting
  static mu_1__type_19 value[3];
  // limit
  bool exists;
  bool split;
  int i0;
  int count_mu_1_Value, oldcount_mu_1_Value;
  bool pos_mu_1_Value[2][2];
  bool goodset_mu_1_Value[2];
  int count_mu_1_Remote, oldcount_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Value[i][j]=FALSE;
  count_mu_1_Value = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Value[i] == count_mu_1_Value)
         {
           pos_mu_1_Value[count_mu_1_Value][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Value++;
      else break;
    }
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Remote[i][j]=FALSE;
  count_mu_1_Remote = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Remote[i] == count_mu_1_Remote)
         {
           pos_mu_1_Remote[count_mu_1_Remote][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Remote++;
      else break;
    }
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

  // refinement -- check selfloop
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
    {
      exists = FALSE;
      split = FALSE;
      // if there exists both self loop and non-self loop
      for (k=0; k<2; k++) // step through class
        if ((*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
            ||(*this)[k+2].mu_Messages[i0].mu_Aux!=k+2)
          exists = TRUE;
        else
          split = TRUE;
      if (exists && split)
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] 
                    && !(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                    && (*this)[k+2].mu_Messages[i0].mu_Aux==k+2)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] ) 
                          split= TRUE;
                  if (split)
                    {
                      for (j=count_mu_1_Remote; j>i; j--)
                        for (k=0; k<2; k++)
                          pos_mu_1_Remote[j][k] = pos_mu_1_Remote[j-1][k];
                      for (k=0; k<2; k++)
                        {
                          if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                            pos_mu_1_Remote[i][k] = FALSE;
                          if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                            pos_mu_1_Remote[i+1][k] = FALSE;
                        }
                      count_mu_1_Remote++; i++;
                    }
                }
            }
        }
    }
  }

  // refinement -- checking priority in general
  while_guard = FALSE;
  while_guard = while_guard || (Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2);
  while_guard = while_guard || (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2);
  while ( while_guard )
    {
      oldcount_mu_1_Value = count_mu_1_Value;
      oldcount_mu_1_Remote = count_mu_1_Remote;
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

      // refinement -- graph structure for a single scalarset
      //               as in array S1 of S1
      // only if there is more than 1 permutation in class
      if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if (!(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                &&(*this)[k+2].mu_Messages[i0].mu_Aux!=k+2
                &&(*this)[k+2].mu_Messages[i0].mu_Aux>=2
                &&(*this)[k+2].mu_Messages[i0].mu_Aux<=3)
              exists = TRUE;
          if (exists)
            {
              for (i=0; i<count_mu_1_Remote; i++) // for value priority
                {
                  for (j=0; j<count_mu_1_Remote; j++) // for value priority
                    {
                      exists = FALSE;
                      for (k=0; k<2; k++) // step through class
                        goodset_mu_1_Remote[k] = FALSE;
                      for (k=0; k<2; k++) // step through class
                        if (pos_mu_1_Remote[i][k] 
                            && !(*this)[k+2].mu_Messages[i0].mu_Aux.isundefined()
                            && (*this)[k+2].mu_Messages[i0].mu_Aux!=k+2
                            && (*this)[k+2].mu_Messages[i0].mu_Aux>=2
                            && (*this)[k+2].mu_Messages[i0].mu_Aux<=3
                            && pos_mu_1_Remote[j][(*this)[k+2].mu_Messages[i0].mu_Aux-2])
                          {
                            exists = TRUE;
                            goodset_mu_1_Remote[k] = TRUE;
                          }
                      if (exists)
                        {
                          split=FALSE;
                          for (k=0; k<2; k++)
                            if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] ) 
                              split= TRUE;
                          if (split)
                            {
                              for (j=count_mu_1_Remote; j>i; j--)
                                for (k=0; k<2; k++)
                                  pos_mu_1_Remote[j][k] = pos_mu_1_Remote[j-1][k];
                              for (k=0; k<2; k++)
                                {
                                  if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                    pos_mu_1_Remote[i][k] = FALSE;
                                  if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                    pos_mu_1_Remote[i+1][k] = FALSE;                  
                                }
                              count_mu_1_Remote++;
                            }
                        }
                    }
                }
            }
        }
  }
  // loop through elements of a array indexed by subrange
  for (i0 = 0; i0 <= 5; i0++)
  {

      // refinement -- graph structure for two scalarsets
      //               as in array S1 of S2
      // only if there is more than 1 permutation in class
      if ( (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
           || ( Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2) )
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if ((*this)[k+2].mu_Messages[i0].mu_Value.isundefined())
              exists = TRUE;
            else
              split = TRUE;
          if (exists && split)
            {
              for (i=0; i<count_mu_1_Remote; i++) // scan through array index priority
                for (j=0; j<count_mu_1_Value; j++) //scan through element priority
                  {
                    exists = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Remote[k] = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Value[k] = FALSE;
                    for (k=0; k<2; k++) // scan array index
                      // set goodsets
                      if (pos_mu_1_Remote[i][k] 
                          && !(*this)[k+2].mu_Messages[i0].mu_Value.isundefined()
                          && pos_mu_1_Value[j][(*this)[k+2].mu_Messages[i0].mu_Value-5])
                        {
                          exists = TRUE;
                          goodset_mu_1_Remote[k] = TRUE;
                          goodset_mu_1_Value[(*this)[k+2].mu_Messages[i0].mu_Value-5] = TRUE;
                        }
                    if (exists)
                      {
                        // set split for the array index type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Remote; z>i; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Remote[z][k] = pos_mu_1_Remote[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i][k] = FALSE;
                                if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i+1][k] = FALSE;
                              }
                            count_mu_1_Remote++;
                          }
                        // set split for the element type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Value; z>j; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Value[z][k] = pos_mu_1_Value[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j][k] = FALSE;
                                if (pos_mu_1_Value[j+1][k] && goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j+1][k] = FALSE;
                              }
                            count_mu_1_Value++;
                          }
                      }
                  }
            }
        }
  }
      while_guard = FALSE;
      while_guard = while_guard || (oldcount_mu_1_Value!=count_mu_1_Value);
      while_guard = while_guard || (oldcount_mu_1_Remote!=count_mu_1_Remote);
      while_guard_temp = while_guard;
      while_guard = FALSE;
      while_guard = while_guard || count_mu_1_Value<2;
      while_guard = while_guard || count_mu_1_Remote<2;
      while_guard = while_guard && while_guard_temp;
    } // end while
  // enter the result into class
  if (Perm.MTO_class_mu_1_Value())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Value[i][j])
            Perm.class_mu_1_Value[j] = i;
      Perm.undefined_class_mu_1_Value=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Value[j]>Perm.undefined_class_mu_1_Value)
          Perm.undefined_class_mu_1_Value=Perm.class_mu_1_Value[j];
    }
  // enter the result into class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Remote[i][j])
            Perm.class_mu_1_Remote[j] = i;
      Perm.undefined_class_mu_1_Remote=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Remote[j]>Perm.undefined_class_mu_1_Remote)
          Perm.undefined_class_mu_1_Remote=Perm.class_mu_1_Remote[j];
    }
}
void mu_1__type_20::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_21::Permute(PermSet& Perm, int i)
{
  static mu_1__type_21 temp("Permute_mu_1__type_21",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=2; j<=3; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_Remote[Perm.in_mu_1_Remote[i]][j-2]];
};
void mu_1__type_21::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_21::Canonicalize(PermSet& Perm){};
void mu_1__type_21::SimpleLimit(PermSet& Perm){}
void mu_1__type_21::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int compare;
  static mu_1__type_20 value[3];
  // limit
  bool exists;
  bool split;
  int count_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // sorting mu_1_Remote
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          pos_mu_1_Remote[i][j]=FALSE;
      count_mu_1_Remote = 0;
      for (i=0; i<2; i++)
        {
          for (j=0; j<count_mu_1_Remote; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+2]);
              if (compare==0)
                {
                  pos_mu_1_Remote[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_Remote; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<2; z++)
                        pos_mu_1_Remote[k][z] = pos_mu_1_Remote[k-1][z];
                    }
                  value[j] = (*this)[i+2];
                  for (z=0; z<2; z++)
                    pos_mu_1_Remote[j][z] = FALSE;
                  pos_mu_1_Remote[j][i] = TRUE;
                  count_mu_1_Remote++;
                  break;
                }
            }
          if (j==count_mu_1_Remote)
            {
              value[j] = (*this)[i+2];
              for (z=0; z<2; z++)
                pos_mu_1_Remote[j][z] = FALSE;
              pos_mu_1_Remote[j][i] = TRUE;
              count_mu_1_Remote++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote>1)
    {
      // limit
      for (j=0; j<2; j++) // class priority
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] && Perm.class_mu_1_Remote[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                    pos_mu_1_Remote[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<2; k++)
                        if (Perm.class_mu_1_Remote[k]>j
                            || ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) )
                          Perm.class_mu_1_Remote[k]++;
                      Perm.undefined_class_mu_1_Remote++;
                    }
                }
            }
        }
    }
}
void mu_1__type_21::Limit(PermSet& Perm){}
void mu_1__type_21::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_22::Permute(PermSet& Perm, int i)
{
  static mu_1__type_22 temp("Permute_mu_1__type_22",-1);
  int j;
  for (j=0; j<3; j++)
    array[j].Permute(Perm, i);
  temp = *this;
  for (j=2; j<=3; j++)
    (*this)[j] = temp[Perm.revperm_mu_1_Remote[Perm.in_mu_1_Remote[i]][j-2]];
};
void mu_1__type_22::SimpleCanonicalize(PermSet& Perm)
{ Error.Error("Internal: Simple Canonicalization of Scalarset Array\n"); };
void mu_1__type_22::Canonicalize(PermSet& Perm){};
void mu_1__type_22::SimpleLimit(PermSet& Perm){}
void mu_1__type_22::ArrayLimit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // sorting
  int compare;
  static mu_1_ProcState value[3];
  // limit
  bool exists;
  bool split;
  int count_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  // sorting mu_1_Remote
  // only if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          pos_mu_1_Remote[i][j]=FALSE;
      count_mu_1_Remote = 0;
      for (i=0; i<2; i++)
        {
          for (j=0; j<count_mu_1_Remote; j++)
            {
              compare = CompareWeight(value[j],(*this)[i+2]);
              if (compare==0)
                {
                  pos_mu_1_Remote[j][i]= TRUE;
                  break;
                }
              else if (compare>0)
                {
                  for (k=count_mu_1_Remote; k>j; k--)
                    {
                      value[k] = value[k-1];
                      for (z=0; z<2; z++)
                        pos_mu_1_Remote[k][z] = pos_mu_1_Remote[k-1][z];
                    }
                  value[j] = (*this)[i+2];
                  for (z=0; z<2; z++)
                    pos_mu_1_Remote[j][z] = FALSE;
                  pos_mu_1_Remote[j][i] = TRUE;
                  count_mu_1_Remote++;
                  break;
                }
            }
          if (j==count_mu_1_Remote)
            {
              value[j] = (*this)[i+2];
              for (z=0; z<2; z++)
                pos_mu_1_Remote[j][z] = FALSE;
              pos_mu_1_Remote[j][i] = TRUE;
              count_mu_1_Remote++;
            }
        }
    }
  // if there is more than 1 permutation in class
  if (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote>1)
    {
      // limit
      for (j=0; j<2; j++) // class priority
        {
          for (i=0; i<count_mu_1_Remote; i++) // for value priority
            {
              exists = FALSE;
              for (k=0; k<2; k++) // step through class
                goodset_mu_1_Remote[k] = FALSE;
              for (k=0; k<2; k++) // step through class
                if (pos_mu_1_Remote[i][k] && Perm.class_mu_1_Remote[k] == j)
                  {
                    exists = TRUE;
                    goodset_mu_1_Remote[k] = TRUE;
                    pos_mu_1_Remote[i][k] = FALSE;
                  }
              if (exists)
                {
                  split=FALSE;
                  for (k=0; k<2; k++)
                    if ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) 
                      split= TRUE;
                  if (split)
                    {
                      for (k=0; k<2; k++)
                        if (Perm.class_mu_1_Remote[k]>j
                            || ( Perm.class_mu_1_Remote[k] == j && !goodset_mu_1_Remote[k] ) )
                          Perm.class_mu_1_Remote[k]++;
                      Perm.undefined_class_mu_1_Remote++;
                    }
                }
            }
        }
    }
}
void mu_1__type_22::Limit(PermSet& Perm)
{
  // indexes
  int i,j,k,z;
  // while guard
  bool while_guard, while_guard_temp;
  // sorting
  static mu_1_ProcState value[3];
  // limit
  bool exists;
  bool split;
  int i0,i1;
  int count_mu_1_Remote, oldcount_mu_1_Remote;
  bool pos_mu_1_Remote[2][2];
  bool goodset_mu_1_Remote[2];
  int count_mu_1_Value, oldcount_mu_1_Value;
  bool pos_mu_1_Value[2][2];
  bool goodset_mu_1_Value[2];
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Remote[i][j]=FALSE;
  count_mu_1_Remote = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Remote[i] == count_mu_1_Remote)
         {
           pos_mu_1_Remote[count_mu_1_Remote][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Remote++;
      else break;
    }
  // initializing pos array
  for (i=0; i<2; i++)
    for (j=0; j<2; j++)
      pos_mu_1_Value[i][j]=FALSE;
  count_mu_1_Value = 0;
  while (1)
    {
      exists = FALSE;
      for (i=0; i<2; i++)
       if (Perm.class_mu_1_Value[i] == count_mu_1_Value)
         {
           pos_mu_1_Value[count_mu_1_Value][i]=TRUE;
           exists = TRUE;
         }
      if (exists) count_mu_1_Value++;
      else break;
    }

  // refinement -- checking priority in general
  while_guard = FALSE;
  while_guard = while_guard || (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2);
  while_guard = while_guard || (Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2);
  while ( while_guard )
    {
      oldcount_mu_1_Remote = count_mu_1_Remote;
      oldcount_mu_1_Value = count_mu_1_Value;

      // refinement -- graph structure for two scalarsets
      //               as in array S1 of S2
      // only if there is more than 1 permutation in class
      if ( (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
           || ( Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2) )
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if ((*this)[k+2].mu_Cache[mu_Home_1][mu_Address_1].mu_Value.isundefined())
              exists = TRUE;
            else
              split = TRUE;
          if (exists && split)
            {
              for (i=0; i<count_mu_1_Remote; i++) // scan through array index priority
                for (j=0; j<count_mu_1_Value; j++) //scan through element priority
                  {
                    exists = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Remote[k] = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Value[k] = FALSE;
                    for (k=0; k<2; k++) // scan array index
                      // set goodsets
                      if (pos_mu_1_Remote[i][k] 
                          && !(*this)[k+2].mu_Cache[mu_Home_1][mu_Address_1].mu_Value.isundefined()
                          && pos_mu_1_Value[j][(*this)[k+2].mu_Cache[mu_Home_1][mu_Address_1].mu_Value-5])
                        {
                          exists = TRUE;
                          goodset_mu_1_Remote[k] = TRUE;
                          goodset_mu_1_Value[(*this)[k+2].mu_Cache[mu_Home_1][mu_Address_1].mu_Value-5] = TRUE;
                        }
                    if (exists)
                      {
                        // set split for the array index type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Remote; z>i; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Remote[z][k] = pos_mu_1_Remote[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i][k] = FALSE;
                                if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i+1][k] = FALSE;
                              }
                            count_mu_1_Remote++;
                          }
                        // set split for the element type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Value; z>j; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Value[z][k] = pos_mu_1_Value[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j][k] = FALSE;
                                if (pos_mu_1_Value[j+1][k] && goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j+1][k] = FALSE;
                              }
                            count_mu_1_Value++;
                          }
                      }
                  }
            }
        }

      // refinement -- graph structure for two scalarsets
      //               as in array S1 of S2
      // only if there is more than 1 permutation in class
      if ( (Perm.MTO_class_mu_1_Remote() && count_mu_1_Remote<2)
           || ( Perm.MTO_class_mu_1_Value() && count_mu_1_Value<2) )
        {
          exists = FALSE;
          split = FALSE;
          for (k=0; k<2; k++) // step through class
            if ((*this)[k+2].mu_RAC[mu_Home_1][mu_Address_1].mu_Value.isundefined())
              exists = TRUE;
            else
              split = TRUE;
          if (exists && split)
            {
              for (i=0; i<count_mu_1_Remote; i++) // scan through array index priority
                for (j=0; j<count_mu_1_Value; j++) //scan through element priority
                  {
                    exists = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Remote[k] = FALSE;
                    for (k=0; k<2; k++) // initialize goodset
                      goodset_mu_1_Value[k] = FALSE;
                    for (k=0; k<2; k++) // scan array index
                      // set goodsets
                      if (pos_mu_1_Remote[i][k] 
                          && !(*this)[k+2].mu_RAC[mu_Home_1][mu_Address_1].mu_Value.isundefined()
                          && pos_mu_1_Value[j][(*this)[k+2].mu_RAC[mu_Home_1][mu_Address_1].mu_Value-5])
                        {
                          exists = TRUE;
                          goodset_mu_1_Remote[k] = TRUE;
                          goodset_mu_1_Value[(*this)[k+2].mu_RAC[mu_Home_1][mu_Address_1].mu_Value-5] = TRUE;
                        }
                    if (exists)
                      {
                        // set split for the array index type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Remote; z>i; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Remote[z][k] = pos_mu_1_Remote[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Remote[i][k] && !goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i][k] = FALSE;
                                if (pos_mu_1_Remote[i+1][k] && goodset_mu_1_Remote[k])
                                  pos_mu_1_Remote[i+1][k] = FALSE;
                              }
                            count_mu_1_Remote++;
                          }
                        // set split for the element type
                        split=FALSE;
                        for (k=0; k<2; k++)
                          if ( pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k] )
                            split= TRUE;
                        if (split)
                          {
                            // move following pos entries down 1 step
                            for (z=count_mu_1_Value; z>j; z--)
                              for (k=0; k<2; k++)
                                pos_mu_1_Value[z][k] = pos_mu_1_Value[z-1][k];
                            // split pos
                            for (k=0; k<2; k++)
                              {
                                if (pos_mu_1_Value[j][k] && !goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j][k] = FALSE;
                                if (pos_mu_1_Value[j+1][k] && goodset_mu_1_Value[k])
                                  pos_mu_1_Value[j+1][k] = FALSE;
                              }
                            count_mu_1_Value++;
                          }
                      }
                  }
            }
        }
      while_guard = FALSE;
      while_guard = while_guard || (oldcount_mu_1_Remote!=count_mu_1_Remote);
      while_guard = while_guard || (oldcount_mu_1_Value!=count_mu_1_Value);
      while_guard_temp = while_guard;
      while_guard = FALSE;
      while_guard = while_guard || count_mu_1_Remote<2;
      while_guard = while_guard || count_mu_1_Value<2;
      while_guard = while_guard && while_guard_temp;
    } // end while
  // enter the result into class
  if (Perm.MTO_class_mu_1_Remote())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Remote[i][j])
            Perm.class_mu_1_Remote[j] = i;
      Perm.undefined_class_mu_1_Remote=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Remote[j]>Perm.undefined_class_mu_1_Remote)
          Perm.undefined_class_mu_1_Remote=Perm.class_mu_1_Remote[j];
    }
  // enter the result into class
  if (Perm.MTO_class_mu_1_Value())
    {
      for (i=0; i<2; i++)
        for (j=0; j<2; j++)
          if (pos_mu_1_Value[i][j])
            Perm.class_mu_1_Value[j] = i;
      Perm.undefined_class_mu_1_Value=0;
      for (j=0; j<2; j++)
        if (Perm.class_mu_1_Value[j]>Perm.undefined_class_mu_1_Value)
          Perm.undefined_class_mu_1_Value=Perm.class_mu_1_Value[j];
    }
}
void mu_1__type_22::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu_1__type_23::Permute(PermSet& Perm, int i)
{
  static mu_1__type_23 temp("Permute_mu_1__type_23",-1);
  int j;
  for (j=0; j<1; j++)
    array[j].Permute(Perm, i);
};
void mu_1__type_23::SimpleCanonicalize(PermSet& Perm)
{
  for (int j=0; j<1; j++)
    array[j].SimpleCanonicalize(Perm);
}
void mu_1__type_23::Canonicalize(PermSet& Perm){};
void mu_1__type_23::SimpleLimit(PermSet& Perm)
{
  for (int j=0; j<1; j++) {
    array[j].SimpleLimit(Perm);
  }
}
void mu_1__type_23::ArrayLimit(PermSet& Perm) {}
void mu_1__type_23::Limit(PermSet& Perm){}
void mu_1__type_23::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for scalarset array.\n"); };
void mu__subrange_43::Permute(PermSet& Perm, int i) {};
void mu__subrange_43::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_43::Canonicalize(PermSet& Perm) {};
void mu__subrange_43::SimpleLimit(PermSet& Perm) {};
void mu__subrange_43::ArrayLimit(PermSet& Perm) {};
void mu__subrange_43::Limit(PermSet& Perm) {};
void mu__subrange_43::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_44::Permute(PermSet& Perm, int i) {};
void mu__subrange_44::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_44::Canonicalize(PermSet& Perm) {};
void mu__subrange_44::SimpleLimit(PermSet& Perm) {};
void mu__subrange_44::ArrayLimit(PermSet& Perm) {};
void mu__subrange_44::Limit(PermSet& Perm) {};
void mu__subrange_44::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_45::Permute(PermSet& Perm, int i) {};
void mu__subrange_45::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_45::Canonicalize(PermSet& Perm) {};
void mu__subrange_45::SimpleLimit(PermSet& Perm) {};
void mu__subrange_45::ArrayLimit(PermSet& Perm) {};
void mu__subrange_45::Limit(PermSet& Perm) {};
void mu__subrange_45::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_46::Permute(PermSet& Perm, int i) {};
void mu__subrange_46::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_46::Canonicalize(PermSet& Perm) {};
void mu__subrange_46::SimpleLimit(PermSet& Perm) {};
void mu__subrange_46::ArrayLimit(PermSet& Perm) {};
void mu__subrange_46::Limit(PermSet& Perm) {};
void mu__subrange_46::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_47::Permute(PermSet& Perm, int i) {};
void mu__subrange_47::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_47::Canonicalize(PermSet& Perm) {};
void mu__subrange_47::SimpleLimit(PermSet& Perm) {};
void mu__subrange_47::ArrayLimit(PermSet& Perm) {};
void mu__subrange_47::Limit(PermSet& Perm) {};
void mu__subrange_47::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_48::Permute(PermSet& Perm, int i) {};
void mu__subrange_48::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_48::Canonicalize(PermSet& Perm) {};
void mu__subrange_48::SimpleLimit(PermSet& Perm) {};
void mu__subrange_48::ArrayLimit(PermSet& Perm) {};
void mu__subrange_48::Limit(PermSet& Perm) {};
void mu__subrange_48::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_49::Permute(PermSet& Perm, int i) {};
void mu__subrange_49::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_49::Canonicalize(PermSet& Perm) {};
void mu__subrange_49::SimpleLimit(PermSet& Perm) {};
void mu__subrange_49::ArrayLimit(PermSet& Perm) {};
void mu__subrange_49::Limit(PermSet& Perm) {};
void mu__subrange_49::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_50::Permute(PermSet& Perm, int i) {};
void mu__subrange_50::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_50::Canonicalize(PermSet& Perm) {};
void mu__subrange_50::SimpleLimit(PermSet& Perm) {};
void mu__subrange_50::ArrayLimit(PermSet& Perm) {};
void mu__subrange_50::Limit(PermSet& Perm) {};
void mu__subrange_50::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_51::Permute(PermSet& Perm, int i) {};
void mu__subrange_51::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_51::Canonicalize(PermSet& Perm) {};
void mu__subrange_51::SimpleLimit(PermSet& Perm) {};
void mu__subrange_51::ArrayLimit(PermSet& Perm) {};
void mu__subrange_51::Limit(PermSet& Perm) {};
void mu__subrange_51::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_52::Permute(PermSet& Perm, int i) {};
void mu__subrange_52::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_52::Canonicalize(PermSet& Perm) {};
void mu__subrange_52::SimpleLimit(PermSet& Perm) {};
void mu__subrange_52::ArrayLimit(PermSet& Perm) {};
void mu__subrange_52::Limit(PermSet& Perm) {};
void mu__subrange_52::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };
void mu__subrange_53::Permute(PermSet& Perm, int i) {};
void mu__subrange_53::SimpleCanonicalize(PermSet& Perm) {};
void mu__subrange_53::Canonicalize(PermSet& Perm) {};
void mu__subrange_53::SimpleLimit(PermSet& Perm) {};
void mu__subrange_53::ArrayLimit(PermSet& Perm) {};
void mu__subrange_53::Limit(PermSet& Perm) {};
void mu__subrange_53::MultisetLimit(PermSet& Perm)
{ Error.Error("Internal: calling MultisetLimit for subrange type.\n"); };

/********************
 Auxiliary function for error trace printing
 ********************/
bool match(state* ns, StatePtr p)
{
  int i;
  static PermSet Perm;
  static state temp;
  StateCopy(&temp, ns);
  if (args->symmetry_reduction.value)
    {
      if (  args->sym_alg.mode == argsym_alg::Exhaustive_Fast_Canonicalize
         || args->sym_alg.mode == argsym_alg::Heuristic_Fast_Canonicalize) {
        Perm.ResetToExplicit();
        for (i=0; i<Perm.count; i++)
          if (Perm.In(i))
            {
              if (ns != workingstate)
                  StateCopy(workingstate, ns);
              
              mu_Homes.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_Homes.MultisetSort();
              mu_Procs.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_Procs.MultisetSort();
              mu_ReqNet.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_ReqNet.MultisetSort();
              mu_ReplyNet.Permute(Perm,i);
              if (args->multiset_reduction.value)
                mu_ReplyNet.MultisetSort();
            if (p.compare(workingstate)) {
              StateCopy(workingstate,&temp); return TRUE; }
          }
        StateCopy(workingstate,&temp);
        return FALSE;
      }
      else {
        Perm.ResetToSimple();
        Perm.SimpleToOne();
        if (ns != workingstate)
          StateCopy(workingstate, ns);

          mu_Homes.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_Homes.MultisetSort();
          mu_Procs.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_Procs.MultisetSort();
          mu_ReqNet.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_ReqNet.MultisetSort();
          mu_ReplyNet.Permute(Perm,0);
          if (args->multiset_reduction.value)
            mu_ReplyNet.MultisetSort();
        if (p.compare(workingstate)) {
          StateCopy(workingstate,&temp); return TRUE; }

        while (Perm.NextPermutation())
          {
            if (ns != workingstate)
              StateCopy(workingstate, ns);
              
              mu_Homes.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_Homes.MultisetSort();
              mu_Procs.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_Procs.MultisetSort();
              mu_ReqNet.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_ReqNet.MultisetSort();
              mu_ReplyNet.Permute(Perm,0);
              if (args->multiset_reduction.value)
                mu_ReplyNet.MultisetSort();
            if (p.compare(workingstate)) {
              StateCopy(workingstate,&temp); return TRUE; }
          }
        StateCopy(workingstate,&temp);
        return FALSE;
      }
    }
  if (!args->symmetry_reduction.value
      && args->multiset_reduction.value)
    {
      if (ns != workingstate)
          StateCopy(workingstate, ns);
      mu_Homes.MultisetSort();
      mu_Procs.MultisetSort();
      mu_ReqNet.MultisetSort();
      mu_ReplyNet.MultisetSort();
      if (p.compare(workingstate)) {
        StateCopy(workingstate,&temp); return TRUE; }
      StateCopy(workingstate,&temp);
      return FALSE;
    }
  return (p.compare(ns));
}

/********************
 Canonicalization by fast exhaustive generation of
 all permutations
 ********************/
void SymmetryClass::Exhaustive_Fast_Canonicalize(state* s)
{
  int i;
  static state temp;
  Perm.ResetToExplicit();

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_Homes.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_Homes.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_Procs.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_Procs.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_ReqNet.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_ReqNet.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_ReplyNet.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_ReplyNet.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

};

/********************
 Canonicalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and fast exhaustive generation of
 all permutations for other variables
 ********************/
void SymmetryClass::Heuristic_Fast_Canonicalize(state* s)
{
  int i;
  static state temp;

  Perm.ResetToSimple();

  mu_Homes.SimpleCanonicalize(Perm);

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.ArrayLimit(Perm);
  }

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.Limit(Perm);
  }

  Perm.SimpleToExplicit();

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_Procs.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_Procs.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_ReqNet.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_ReqNet.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

  StateCopy(&temp, workingstate);
  ResetBestResult();
  for (i=0; i<Perm.count; i++)
    if (Perm.In(i))
      {
        StateCopy(workingstate, &temp);
        mu_ReplyNet.Permute(Perm,i);
        if (args->multiset_reduction.value)
          mu_ReplyNet.MultisetSort();
        SetBestResult(i, workingstate);
      }
  StateCopy(workingstate, &BestPermutedState);

};

/********************
 Canonicalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and fast exhaustive generation of
 all permutations for other variables
 and use less local memory
 ********************/
void SymmetryClass::Heuristic_Small_Mem_Canonicalize(state* s)
{
  unsigned long cycle;
  static state temp;

  Perm.ResetToSimple();

  mu_Homes.SimpleCanonicalize(Perm);

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.ArrayLimit(Perm);
  }

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.Limit(Perm);
  }

  Perm.SimpleToOne();

  StateCopy(&temp, workingstate);
  ResetBestResult();
  mu_Procs.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_Procs.MultisetSort();
  mu_ReqNet.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_ReqNet.MultisetSort();
  mu_ReplyNet.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_ReplyNet.MultisetSort();
  BestPermutedState = *workingstate;
  BestInitialized = TRUE;

  cycle=1;
  while (Perm.NextPermutation())
    {
      if (args->perm_limit.value != 0
          && cycle++ >= args->perm_limit.value) break;
      StateCopy(workingstate, &temp);
      mu_Procs.Permute(Perm,0);
      if (args->multiset_reduction.value)
        mu_Procs.MultisetSort();
      mu_ReqNet.Permute(Perm,0);
      if (args->multiset_reduction.value)
        mu_ReqNet.MultisetSort();
      mu_ReplyNet.Permute(Perm,0);
      if (args->multiset_reduction.value)
        mu_ReplyNet.MultisetSort();
      switch (StateCmp(workingstate, &BestPermutedState)) {
      case -1:
        BestPermutedState = *workingstate;
        break;
      case 1:
        break;
      case 0:
        break;
      default:
        Error.Error("funny return value from StateCmp");
      }
    }
  StateCopy(workingstate, &BestPermutedState);

};

/********************
 Normalization by fast simple variable canonicalization,
 fast simple scalarset array canonicalization,
 fast restriction on permutation set with simple scalarset array of scalarset,
 and for all other variables, pick any remaining permutation
 ********************/
void SymmetryClass::Heuristic_Fast_Normalize(state* s)
{
  int i;
  static state temp;

  Perm.ResetToSimple();

  mu_Homes.SimpleCanonicalize(Perm);

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.ArrayLimit(Perm);
  }

  if (Perm.MoreThanOneRemain()) {
    mu_Procs.Limit(Perm);
  }

  Perm.SimpleToOne();

  mu_Procs.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_Procs.MultisetSort();

  mu_ReqNet.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_ReqNet.MultisetSort();

  mu_ReplyNet.Permute(Perm,0);
  if (args->multiset_reduction.value)
    mu_ReplyNet.MultisetSort();

};

/********************
  Include
 ********************/
#include "mu_epilog.inc"
