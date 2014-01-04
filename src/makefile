# Generated automatically from Makefile.in by configure.
ALL: default
##### User configurable options #####

SHELL       = /bin/sh
ARCH        = solaris
#COMM        = ch_p4
MPIR_HOME   = /export/home/jzhou/mpich-install
CC          = /export/home/jzhou/mpich-install/bin/mpicc
CLINKER     = $(CC)
OPTFLAGS    = -L/export/home/jzhou/mpich-install/lib -DREAL_RANDOM

### End User configurable options ###

CFLAGS	  = $(OPTFLAGS) 
CCFLAGS	  = $(CFLAGS)
EXECS	  =  run 

default: $(EXECS)

all     : run

#---------------------------------------------------------------------
#		SOURCES AND OBJECTS FILES :
#---------------------------------------------------------------------
OBJECTS  = sp_input.o sp_row.o sp_scat.o sp_main.o \
           sp_outg.o  sp_rowm.o sp_sermv.o

CFLAGS = $(OPTFLAGS)

run: $(OBJECTS) $(MPIR_HOME)/include/mpi.h
	$(CLINKER) $(OPTFLAGS) -o run $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean : 
	$(RM) *.o core run 
