CXX = mpicxx
CXXFLAGS = -O3 -Wall
LDFLAGS = -lm

TARGETS = fft_serial fft_mpi fft_bench

.PHONY: all clean

all: $(TARGETS)

fft_serial: fft_serial.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fft_mpi: fft_mpi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fft_bench: fft_bench.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS) massif.out.*
