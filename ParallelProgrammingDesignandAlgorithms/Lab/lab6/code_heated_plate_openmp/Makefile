CC = gcc
CFLAGS = -Wall -O2
PTHREAD_FLAGS = -pthread

LIB_NAME = parallel_for
LIB_SONAME_VERSION = 1
LIB_VERSION = 1.0

LIB_REAL = lib$(LIB_NAME).so.$(LIB_VERSION)
LIB_SONAME = lib$(LIB_NAME).so.$(LIB_SONAME_VERSION)
LIB_LINKER = lib$(LIB_NAME).so

.PHONY: all clean run_benchmark

all: $(LIB_REAL) symlinks heated_plate_pthreads heated_plate_openmp

# Build shared library
$(LIB_REAL): parallel_for.o
	$(CC) -shared -Wl,-soname,$(LIB_SONAME) -o $@ $^ $(PTHREAD_FLAGS)

# Create symlinks
symlinks:
	ln -sf $(LIB_REAL) $(LIB_SONAME)
	ln -sf $(LIB_SONAME) $(LIB_LINKER)

# Build heated_plate_pthreads
heated_plate_pthreads: heated_plate_pthreads.o
	$(CC) -o $@ $< -L. -l$(LIB_NAME) -Wl,-rpath,. $(PTHREAD_FLAGS) -lm

# Compile C sources
%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $<

# Compile heated_plate_pthreads (no -fPIC needed)
heated_plate_pthreads.o: heated_plate_pthreads.c parallel_for.h
	$(CC) $(CFLAGS) -c $<

# Build original OpenMP version
heated_plate_openmp: heated_plate_openmp.c
	$(CC) -fopenmp -O2 heated_plate_openmp.c -o heated_plate_openmp -lm

clean:
	rm -f *.o $(LIB_REAL) $(LIB_SONAME) $(LIB_LINKER)
	rm -f heated_plate_pthreads heated_plate_openmp
