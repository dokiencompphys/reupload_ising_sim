.POSIX:

TOPDIR	= .
SRC		= $(TOPDIR)/src
SRCMAIN = $(SRC)/main
SRCM	= $(SRC)/modules
SRCT	= $(SRC)/tests
OUT		= $(TOPDIR)/executables
CC		= gcc
CFLAGS	= -Wall -iquote $(SRCM) -iquote $(SRCT) $(ADD_CFLAGS)
LDFLAGS	=
LDLIBS	= -lm -lgsl -lgslcblas

.PHONY: all cleanall clean

# shortcuts to executables, so you can use target without paths
ising-sim: $(OUT)/ising-sim;
auto: $(OUT)/auto;
test_monte_carlo: $(OUT)/test_monte_carlo;
test_nneighbour: $(OUT)/test_nneighbour;
test_energy: $(OUT)/test_energy;


# executables
$(OUT)/ising-sim: $(SRCMAIN)/ising-sim.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o $@ $(SRCMAIN)/ising-sim.o $(SRCM)/geometry.o $(SRCM)/functions.o $(LDLIBS)
$(OUT)/auto: $(SRCMAIN)/auto.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o  $@ $(SRCMAIN)/auto.o $(SRCM)/geometry.o $(SRCM)/functions.o $(LDLIBS)

$(OUT)/test_energy: $(SRCT)/test_energy.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_energy.o $(SRCM)/geometry.o $(SRCM)/functions.o $(LDLIBS)
$(OUT)/test_monte_carlo: $(SRCT)/test_monte_carlo.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_monte_carlo.o $(SRCM)/geometry.o $(SRCM)/functions.o $(LDLIBS)
$(OUT)/test_nneighbour: $(SRCT)/test_nneighbour.o $(SRCM)/geometry.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_nneighbour.o $(SRCM)/geometry.o $(LDLIBS)


# creating object files for testing
$(SRCMAIN)/ising-sim.o: $(SRCMAIN)/ising-sim.c $(SRCM)/global.h $(SRCM)/functions.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCMAIN)/ising-sim.c -o $@

$(SRCMAIN)/auto.o: $(SRCMAIN)/autocorr.c $(SRCM)/global.h $(SRCM)/functions.h $(SRCM)/geometry.h		
	$(CC) -c $(CFLAGS) $(SRCMAIN)/autocorr.c -o $@

$(SRCT)/test_monte_carlo.o: $(SRCT)/test_monte_carlo.c $(SRCM)/global.h $(SRCM)/functions.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_monte_carlo.c -o $@
$(SRCT)/test_nneighbour.o: $(SRCT)/test_nneighbour.c $(SRCM)/global.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_nneighbour.c -o $@
$(SRCT)/test_energy.o: $(SRCT)/test_energy.c $(SRCM)/global.h $(SRCM)/functions.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_energy.c -o $@


# creating modules object files
$(SRCM)/geometry.o: $(SRCM)/geometry.c $(SRCM)/global.h
	$(CC) -c $(CFLAGS) $(SRCM)/geometry.c -o $@
$(SRCM)/functions.o: $(SRCM)/functions.c $(SRCM)/global.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCM)/functions.c -o $@

# phony recipes
all: ising-sim auto test_monte_carlo test_nneighbour test_energy
cleanall: clean
	rm -f $(OUT)/*
clean:
	rm -f $(SRCM)/*.o $(SRCT)/*.o $(SRCMAIN)/*.o
