
#   Makefile for Murphi Compiler 
#   (using "y.tab.c", "y.tab.h" and "lex.yy.c"
#    from precompiled "mu.l" and "mu.y")
#
#   *** warning ***
#   if you have changed "mu.l" and "mu.y",
#   please use "Makefile.full"
#
#   please make sure that you have set the 
#   appropriate path for c++ compiler.
#
#   options:
#   1) "make" or "make mu"  
#      --> compile mu
#   2) "make install"
#      --> compile mu and copy to bin directory
#   3) "make clean"
#      --> remove all intermediate files
#
#   Author : C. Norris Ip
#            modifications by Uli Stern
#   Created : March 94
########################################

# choice of the compiler with REQUIRED options
# remark: The C++ compilers based on cfront (CC, DCC, OCC) can only generate 
#         very simple inline code. To avoid an error message, one has to dis-
#         able all inlining.
# remark: The xlC C++ compiler by default only accepts C++ files with the
#         suffix ".C". It has to be started with the option "-+" to avoid 
#         error messages when compiling lex.yy.c and y.tab.c. In addition,
#         one should turn off inlining when using optimization
#CPLUSPLUS = CC +d
#CPLUSPLUS = DCC +d
#CPLUSPLUS = OCC +d
CPLUSPLUS = g++
#CPLUSPLUS = xlC -+ -Q!

HOSTTYPE = x86_64

# choice of the options
CFLAGS = -O4 -Wno-write-strings -m32        # options for g++
#CFLAGS = -O4 -pipe -static #
#CFLAGS = -static #
#CFLAGS = -w #                              # options for OCC
#CFLAGS = -w -O3 #                          # options for DCC

OBJS = 	\
	cpp_code.o \
	cpp_code_as.o \
	lex.yy.o \
	y.tab.o \
  	cpp_sym.o \
  	cpp_sym_aux.o \
  	cpp_sym_decl.o \
	decl.o \
	error.o \
	expr.o \
	lextable.o \
	mu.o \
	parse.o \
	rule.o \
	stmt.o \
	symtab.o \
	util.o
########################################
# Makefile grammar

mu: $(OBJS)
	$(CPLUSPLUS) -o mu $(CFLAGS) $(OBJS)

mu.o: mu.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) mu.C

cpp_code.o: cpp_code.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) cpp_code.C

cpp_code_as.o: cpp_code_as.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) cpp_code_as.C

cpp_sym.o: cpp_sym.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) cpp_sym.C

cpp_sym_aux.o: cpp_sym_aux.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) cpp_sym_aux.C

cpp_sym_decl.o: cpp_sym_decl.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) cpp_sym_decl.C

decl.o: decl.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) decl.C

error.o: error.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) error.C

expr.o: expr.C mu.h y.tab.h
	$(CPLUSPLUS) -c $(CFLAGS) expr.C

lextable.o: lextable.C mu.h lextable.h y.tab.h
	$(CPLUSPLUS) -c $(CFLAGS) lextable.C

lextable.h: y.tab.h

no_code: no_code.o $(OBJS)
	$(CPLUSPLUS) -o no_code $(CFLAGS) no_code.o \
	decl.o \
	error.o \
	expr.o \
	lextable.o \
	mu.o \
	parse.o \
	rule.o \
	stmt.o \
	symtab.o \
	util.o \
	lex.yy.o \
	y.tab.o

no_code.o: no_code.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) no_code.C

rule.o: rule.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) rule.C

stmt.o: stmt.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) stmt.C

symtab.o: symtab.C mu.h lextable.h y.tab.h
	$(CPLUSPLUS) -c $(CFLAGS) symtab.C

util.o: util.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) util.C

parse.o: parse.C mu.h
	$(CPLUSPLUS) -c $(CFLAGS) parse.C

lex.yy.o: lex.yy.c mu.h lextable.h
	$(CPLUSPLUS) -c $(CFLAGS) lex.yy.c

y.tab.o: y.tab.c mu.h
	$(CPLUSPLUS) -c -DYYDEBUG=0 $(CFLAGS) y.tab.c

mu.h: decl.h expr.h stmt.h rule.h lextable.h y.tab.h cpp_sym.h
	touch -c mu.h

clean:
	rm -f  *.o no_code mu

remove: 
	rm -f ../bin/mu.$(HOSTTYPE)

install: remove mu
	mv mu ../bin/mu.$(HOSTTYPE)

