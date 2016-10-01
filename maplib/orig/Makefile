# Normal compile options
#OPT_COMP = -O2 -fomit-frame-pointer

# Compile options for profiling using gprof
OPT_COMP = -O3 -march=native -fwhole-program

#OPT_PROF = -pg

# Warning level
OPT_WARN = -Wall -Wextra

# Match optimization
OPT_MATH = -ffast-math -msse2


CXXFLAGS = $(OPT_WARN) $(OPT_PROF) $(OPT_COMP) $(OPT_MATH) $(OPT_OMP)

LDFLAGS  = $(OPT_PROF) $(OPT_OMP) -lm -lbz2
CC = g++


OBJS = splat.o

all: splat

.PHONY: clean
clean:
	rm -f $(OBJS) splat *.cache *.txt

splat.o: splat.cpp itm.cpp

splat: $(OBJS)



SPLAT_DEF = -metric
test_area: splat
	bash -c "time ./splat \
		-d . \
		-t dh3hs \
		-s hessen.dat \
		-R 20 \
		$(SPLAT_DEF) -erp 0 -L 1.4 -o x"

test_p2p: splat
	bash -c "time ./splat \
		-d . \
		-t dh3hs \
		-r db0ft \
		-s hessen.dat \
		$(SPLAT_DEF)"

#SPLAT_TST = -d .. -t ../dh3hs.qth -r ../db0ft.qth
SPLAT_TST = -d .. -t ../dh3hs.qth -r ../db0ft.qth -p profile
#SPLAT_TST = -d .. -t ../dh3hs.qth -s ../hessen.dat -R 20 -erp 0 -L 1.4 -o x

test:
	rm -f tst-*/*
	cd tst-new; ../splat  $(SPLAT_DEF) $(SPLAT_TST)
	cd tst-old; splat     $(SPLAT_DEF) $(SPLAT_TST)
