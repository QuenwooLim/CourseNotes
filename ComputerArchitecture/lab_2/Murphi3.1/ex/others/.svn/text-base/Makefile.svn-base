#
# Makefile for compiling the Murphi verifier for a given protocol 
#          from its .C file
#
# Make sure that you set INCLUDEPATH and CXX according to your system
#
# Author:               Ulrich Stern
# Version:              1
# Creation Date:        Sat May 25 15:13:39 PDT 1996
# Filename:             Makefile
# History:
#
# Experiences compiling the Murphi verifier:
#  There are two things that caused problems in this Muphi release:
#  - Some compilers - especially those based on cfront - can only generate
#   very simple inline code. One has to turn off inlining in this case. The
#   options are typically +d (CC, OCC, DCC) or -fno-default-inline (g++).
#   The compilation is much faster then as well.
#  - The "signal" system call used in Murphi for defining an own handler
#   to catch division by zero seems to be system dependent. Two different
#   ways to use this call can be selected by defining or not-defining
#   CATCH_DIV. See below for when defining CATCH_DIV was needed.
#
# Tested on the following machines:
#  beet        SGI INDY    running IRIX 5.3
#  cabbage     Sun SPARC20 running SunOS 4.1.3_U1
#  dawn        Sun SPARC20 running SunOS 5.4
#  elaine      Sun SPARC20 running SunOS 4.1.4
#
# Compiler versions:
#  g++    2.6.0 (cabbage), 2.6.3 (beet), 2.7.2 (dawn, elaine)
#  CC     3.0 or higher (elaine)
#  OCC    3.2.1 (beet)
#  DCC    4.0 (beet)
#

# path for including mu_verifier.C etc.
INCLUDEPATH = ../../include

# choice of compiler (with REQUIRED options)
#CXX = g++ #                                 # for beet, elaines, dawns, cabbage
#CXX = CC +d -DCATCH_DIV #                   # for elaines
CXX = OCC +d #                              # for beet
#CXX = DCC #                                 # for beet

# options (really OPTIONAL)
CFLAGS = -Wno-write-strings -m32
#CFLAGS = -fno-default-inline #              # for g++, only on beet and cabbage
#CFLAGS = +d -w #                            # for DCC

# optimization
#OFLAGS = -O3

# rules for compiling
abp: abp.C
	${CXX} ${CFLAGS} ${OFLAGS} -o abp abp.C -I${INCLUDEPATH} -lm

arbiter: arbiter.C
	${CXX} ${CFLAGS} ${OFLAGS} -o arbiter arbiter.C -I${INCLUDEPATH} -lm

cache3: cache3.C
	${CXX} ${CFLAGS} ${OFLAGS} -o cache3 cache3.C -I${INCLUDEPATH} -lm

dp4: dp4.C
	${CXX} ${CFLAGS} ${OFLAGS} -o dp4 dp4.C -I${INCLUDEPATH} -lm

dpnew: dpnew.C
	${CXX} ${CFLAGS} ${OFLAGS} -o dpnew dpnew.C -I${INCLUDEPATH} -lm

newcache3: newcache3.C
	${CXX} ${CFLAGS} ${OFLAGS} -o newcache3 newcache3.C -I${INCLUDEPATH} -lm
